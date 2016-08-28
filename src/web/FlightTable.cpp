// flugbuch
#include "FlightTable.h"
#include "FormatStr.h"
#include "SystemInformation.h"
#include "inout_igc.h"
#include "igc_storage.h"
// witty
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/DateField>
#include <Wt/Ext/NumberField>
#include <Wt/Ext/MessageBox>
#include <Wt/Ext/Button>
#include <Wt/WDate>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WHBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WApplication>
#include <Wt/WGoogleMap>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/checked_delete.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/range/algorithm.hpp>
// std lib
#include <stdexcept>
#include <iomanip>

using namespace flbwt;
using namespace flb;
namespace brng = boost::range;
namespace bfs = boost::filesystem;


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
LocationField::LocationField(const boost::shared_ptr<flb::FlightDatabase>  flightDb, const flb::Location::UseAs useAs, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), useAs_(useAs),
   impl_(new Wt::WContainerWidget()),
   cbArea_(new Wt::Ext::ComboBox(impl_)), cbLocation_(new Wt::Ext::ComboBox(impl_))
{
    setImplementation(impl_);
    impl_->setLayout(new Wt::WHBoxLayout());
    impl_->layout()->addWidget(cbArea_);
    impl_->layout()->addWidget(cbLocation_);

    cbArea_->activated().connect(SLOT(this, LocationField::fillLocations));

	fillAreas();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationField::fillAreas()
{
    while(cbArea_->count())
        cbArea_->removeItem(0);

    const auto areas = flightDb_->getFlightAreasEx(useAs_);
    for(const auto& area : areas)
        cbArea_->addItem(area->name());
    cbArea_->setCurrentIndex(0);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
size_t LocationField::selectArea(const std::string &area)
{
    for(int i=0; i<cbArea_->count(); ++i)
    {
        if(area == cbArea_->itemText(i).narrow())
        {
            cbArea_->setCurrentIndex(i);
            return i;
        }
    }
    return 0;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationField::fillLocations(int idx)
{
    while(cbLocation_->count())
        cbLocation_->removeItem(0);

    const std::string area = cbArea_->text().narrow();
    for(const auto& location : flightDb_->getLocationsEx(flightDb_->getArea(area), useAs_))
        cbLocation_->addItem(location->name());

    cbLocation_->setCurrentIndex(0);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
size_t LocationField::selectLocation(const std::string &loc)
{
    for(int i=0; i<cbLocation_->count(); ++i)
        try
        {
            if(loc == cbLocation_->itemText(i).narrow())
            {
                cbLocation_->setCurrentIndex(i);
                return i;
            }
        }
        catch(std::exception &ex)
        {
            std::cout << "Exception in LocationField::selectLocation() : " << ex.what();
//                      << " with " << cbLocation_->itemText(i) << std::endl;
        }
    return 0;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bool LocationField::hasLocation() const
{
    return (cbArea_->currentText().narrow().length() && cbLocation_->currentText().narrow().length());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationField::setLocation(const flb::Location &loc)
{
    fillAreas();
    selectArea(loc.area()->name());
    fillLocations(0);
    selectLocation(loc.name());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
const boost::shared_ptr<flb::Location> LocationField::getLocation() const
{
    auto area = flightDb_->getArea(cbArea_->currentText().narrow());
    auto locations = flightDb_->getLocationsEx(area, useAs_);
    auto fit = brng::find_if(locations, [this](const boost::shared_ptr<Location>& loc)
        { return loc->name() == cbLocation_->currentText(); });
    if(fit == locations.end())
        throw std::invalid_argument("Unknown location");
    return *fit;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTableRow::clearRow()
{
    // set the edit field pointers to null
    nbrEdit_ = ndDuration_ = NULL;
	dateEdit_ = NULL;
	cbGlider_ = NULL;
	lfTakeoff_ = lfLanding_ = NULL;
	taStory_ = NULL;

	// clear the previous content from the row
	for(int i=0; i<9; ++i)
	{
		Wt::WTableCell *cell = table_->elementAt(rowNr_, i);
		cell->clear();
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTableRow::show()
{
	clearRow();

    // the edit image
	Wt::WImage *wiEdit = new Wt::WImage("img/edit.png");
	wiEdit->setToolTip("Flug bearbeiten");
	wiEdit->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiEdit);
	wiEdit->clicked().connect(SLOT(this, FlightTableRow::edit));
    // the delete image
	Wt::WImage *wiDelete = new Wt::WImage("img/delete.png");
	wiDelete->setToolTip("Flug löschen");
	wiDelete->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiDelete);
	wiDelete->clicked().connect(SLOT(this, FlightTableRow::remove));
	// the map image
	Wt::WImage *wiMap = new Wt::WImage("img/map.png");
	wiMap->setToolTip("track");
	wiMap->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiMap);
    wiMap->clicked().connect(SLOT(this, FlightTableRow::map));
	// prepare the text
	std::vector<std::string> vsText;
	vsText.push_back(boost::lexical_cast<std::string>(flight_->number()));
	try
	{
        const std::string sloc = Wt::WApplication::instance()->locale();
	    std::locale loc(sloc.c_str());
	    vsText.push_back(FormatStr(loc) << flight_->date());
	}
	catch(std::exception &ex)
	{
        vsText.push_back(FormatStr() << flight_->date());
    }
	vsText.push_back(flight_->glider()->identity());
	vsText.push_back(flight_->takeoff()->identity());
	vsText.push_back(flight_->landing()->identity());
	vsText.push_back(FormatStr() << flight_->duration());
	vsText.push_back(FormatStr() << (flight_->takeoff()->height() - flight_->landing()->height()) << "m");
	vsText.push_back(FormatStr() << std::fixed << std::setprecision(2) << flight_->distance() << " km");
	// add the text widgets
	for(size_t i=0; i<vsText.size(); ++i)
	{
		Wt::WText *wtxt = new Wt::WText(vsText[i]);
		wtxt->setStyleClass("tableContent");
		table_->elementAt(rowNr_, i + 1)->addWidget(wtxt);
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTableRow::edit()
{
	size_t idx = 0;
	clearRow();

 	// the save image
	Wt::WImage *wiSave = new Wt::WImage("img/save.png");
	wiSave->setToolTip("speichern");
	wiSave->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiSave);
	wiSave->clicked().connect(SLOT(this, FlightTableRow::save));
	// the cancel image
	Wt::WImage *wiCancel = new Wt::WImage("img/undo.png");
	wiCancel->setToolTip("abbrechen");
	wiCancel->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiCancel);
	if(isNewEntry_)
        wiCancel->clicked().connect(SLOT(this, FlightTableRow::remove));
    else
        wiCancel->clicked().connect(SLOT(this, FlightTableRow::show));
    // the map image
	Wt::WImage *wiMap = new Wt::WImage("img/map.png");
	wiMap->setToolTip("track");
	wiMap->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiMap);
    wiMap->clicked().connect(SLOT(this, FlightTableRow::map));
	// flight number
	nbrEdit_ = new Wt::Ext::NumberField();
	nbrEdit_->setValue(flight_->number());
	nbrEdit_->resize(50, nbrEdit_->height());
	table_->elementAt(rowNr_, colFlightNbr)->addWidget(nbrEdit_);
	// date
	dateEdit_ = new Wt::Ext::DateField();
	const boost::gregorian::date &bgd = flight_->date();
	int year  = bgd.year();
	int month = bgd.month();
	int day   = bgd.day();
	Wt::WDate wdate(year, month, day);
//	Wt::WDate wdate(bgd.year(), bgd.month(), bgd.day());
	dateEdit_->setDate(wdate);
	table_->elementAt(rowNr_, colDate)->addWidget(dateEdit_);
	// glider
	cbGlider_ = new Wt::Ext::ComboBox();
	idx = 0;
	for(const auto& gld : flightDb_->Gliders)
	{
        cbGlider_->insertItem(idx, gld->identity());
        if(gld == flight_->glider())
            cbGlider_->setCurrentIndex(idx);
        ++idx;
    }
    cbGlider_->resize(100, cbGlider_->height());
	table_->elementAt(rowNr_, colGlider)->addWidget(cbGlider_);
	// takeoff
	lfTakeoff_ = new LocationField(flightDb_, Location::UA_TAKEOFF);
//	lfTakeoff_->selectArea(flight_->getTakeoff()->getArea()->getName());
	lfTakeoff_->fillLocations(lfTakeoff_->selectArea(flight_->takeoff()->area()->name()));
	lfTakeoff_->selectLocation(flight_->takeoff()->name());
	table_->elementAt(rowNr_, colTakeoff)->addWidget(lfTakeoff_);
	// waypoints
	Wt::WText *txtWpt = new Wt::WText("Wegpunkte");
	table_->elementAt(rowNr_, colTakeoff)->addWidget(txtWpt);
	vlfWaypoints_.clear();
	for(Flight::Waypoints::const_iterator it = flight_->Waypoints.begin(); it != flight_->Waypoints.end(); ++it)
	{
        LocationField *lfld = new LocationField(flightDb_, Location::UA_WAYPNT);
        lfld->fillLocations(lfld->selectArea((*it)->area()->name()));
        lfld->selectLocation((*it)->name());
        table_->elementAt(rowNr_, colTakeoff)->addWidget(lfld);
        vlfWaypoints_.push_back(lfld);
	}
	LocationField *lfld = new LocationField(flightDb_, Location::UA_WAYPNT);
//    lfld->fillLocations(lfld->selectArea((*it)->area()->name()));
//    lfld->selectLocation((*it)->name());
    table_->elementAt(rowNr_, colTakeoff)->addWidget(lfld);
    vlfWaypoints_.push_back(lfld);
	// landing
	lfLanding_ = new LocationField(flightDb_, Location::UA_LANDING);
	lfLanding_->fillLocations(lfLanding_->selectArea(flight_->landing()->area()->name()));
	lfLanding_->selectLocation(flight_->landing()->name());
	table_->elementAt(rowNr_, colLanding)->addWidget(lfLanding_);
	// story
	taStory_ = new Wt::WTextArea("");
	taStory_->setText(flight_->story());
	taStory_->setColumns(50);
	taStory_->setRows(5);
	table_->elementAt(rowNr_, colLanding)->addWidget(taStory_);
	// duration
	ndDuration_ = new Wt::Ext::NumberField();
	ndDuration_->setValue(flight_->duration().total_seconds() / 60);
	ndDuration_->resize(40, ndDuration_->height());
	table_->elementAt(rowNr_, colDuration)->addWidget(ndDuration_);
//	Wt::WText *txtMin = new Wt::WText("min");
//	table_->elementAt(row, colDuration)->addWidget(txtMin);
	// height difference
	std::string sHeight(FormatStr() << (flight_->takeoff()->height() - flight_->landing()->height()) << "m");
	Wt::WText *txtHeight = new Wt::WText(sHeight);
	table_->elementAt(rowNr_, colHeightDiff)->addWidget(txtHeight);
	// distance
	std::string sDist(FormatStr() << flight_->distance() << " km");
	Wt::WText *txtDist = new Wt::WText(sDist);
	table_->elementAt(rowNr_, colDistance)->addWidget(txtDist);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTableRow::save()
{
	// save the information in the edit fields back to the internal representation
    try
    {
        // flight number
        assert(nbrEdit_);
//        if(nbrEdit_->value() != row)
//            if(find_if(flightDb_->getFlights().begin(), flightDb_->getFlights().end(), nbrEdit_->value == bind(&Flight::getNumber, *_1)) != flightDb_->getFlights().begin())
//                throw std::invalid_argument("Flugnummer bereits vergeben");
        flight_->setNumber(nbrEdit_->value());
        // date
        assert(dateEdit_);
        Wt::WDate wdate = dateEdit_->date();
        flight_->setDate(boost::gregorian::date(wdate.year(), wdate.month(), wdate.day()));
        // glider
        assert(cbGlider_);
//        const string glddesc = cbGlider_->currentText().narrow();
//        FlightDatabase::SeqGliders::const_iterator fitGld = std::find_if(flightDb_->getGliders().begin(), flightDb_->getGliders().end(),
//                bind(&Glider::getDescription, *bll::_1) == glddesc);
//        if(fitGld == flightDb_->getGliders().end())
//            throw std::invalid_argument("Unknown glider");
//        flight_->setGlider(*fitGld);

        for(const auto& gld : flightDb_->Gliders)
            if(cbGlider_->text() == gld.get()->identity())
                flight_->setGlider(gld);
        // takeoff
        assert(lfTakeoff_);
        flight_->setTakeoff(lfTakeoff_->getLocation());
        // waypoints
        flight_->clearWaypoints();
        for(const auto& locFld : vlfWaypoints_)
            if(locFld->hasLocation())
                flight_->addWaypoint(locFld->getLocation());
        // landing
        assert(lfLanding_);
        flight_->setLanding(lfLanding_->getLocation());
        // story
        assert(taStory_);
        flight_->setStory(taStory_->text().narrow());
        // duration
        assert(ndDuration_);
        flight_->setDuration(boost::posix_time::minutes(ndDuration_->value()));

        // recalculate the distance
        flight_->setDistance(flight_->calcDistance());


    	// display the information as text without the edit fields
        show();

        isNewEntry_ = false;
    }
    catch(std::exception &ex)
    {
		Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Ok, true);

    }

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTableRow::remove()
{
    try
    {
        flightDb_->deleteFlight(flight_);
        clearRow();
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Ok, true);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTableRow::map()
{
    mapDlg_ = new Wt::Ext::Dialog("View the flight");
    mapDlg_->resize(500, 400);
    mapDlg_->setSizeGripEnabled(false);
    Wt::WGoogleMap *gmap = new Wt::WGoogleMap();
    mapDlg_->contents()->addWidget(gmap);
	gmap->resize(500, 400);
	gmap->enableScrollWheelZoom();
	gmap->disableDoubleClickZoom();
	gmap->enableDragging();
	gmap->setMapTypeControl(Wt::WGoogleMap::HierarchicalControl);

	// get the igc file if we have one
	std::string igcBaseDir;
	if(!Wt::WApplication::readConfigurationProperty("igcBaseDir", igcBaseDir))
        igcBaseDir = (flb::SystemInformation::homeDir() / "gipsy").string();
    igc_storage igcstore(bfs::path(igcBaseDir) / flightDb_->pilotName(), true);
    bfs::path igcfile = igcstore.make_igc_filename(flight_->track(), flight_->date().year());

	gmap->clearOverlays();
    // draw the flight
    std::vector<Wt::WGoogleMap::Coordinate> points;
    std::pair<Wt::WGoogleMap::Coordinate, Wt::WGoogleMap::Coordinate> bbox = std::make_pair(Wt::WGoogleMap::Coordinate(90, 180), Wt::WGoogleMap::Coordinate(-90, -180));

    if(bfs::exists(igcfile))
    {
        flb::inout_igc ioigc(flightDb_);
        ioigc.read(igcfile);
        for(flb::inout_igc::Trackpoints::const_iterator itp = ioigc.Trackpoints.begin(); itp != ioigc.Trackpoints.end(); ++itp)
            points.push_back(Wt::WGoogleMap::Coordinate(itp->pos_.first, itp->pos_.second));
        gmap->addPolyline(points, Wt::WColor("#EE4444"), 3, 0.4);

        for(const auto coord : points)
        {
            bbox.first.setLatitude(  std::min(bbox.first.latitude(),   coord.latitude()));
            bbox.first.setLongitude( std::min(bbox.first.longitude(),  coord.longitude()));
            bbox.second.setLatitude( std::max(bbox.second.latitude(),  coord.latitude()));
            bbox.second.setLongitude(std::max(bbox.second.longitude(), coord.longitude()));
        }
    }
    // draw the legs
    points.clear();
    points.push_back(Wt::WGoogleMap::Coordinate(flight_->takeoff()->pos().first, flight_->takeoff()->pos().second));

    for(const auto& waypoint : flight_->Waypoints)
        points.push_back(Wt::WGoogleMap::Coordinate(waypoint->pos().first, waypoint->pos().second));
    points.push_back(Wt::WGoogleMap::Coordinate(flight_->landing()->pos().first, flight_->landing()->pos().second));

    gmap->addPolyline(points, Wt::WColor("#FF0000"), 2, 0.9);
    // bounding box
    for(const auto& coord : points)
    {
        bbox.first.setLatitude(  std::min(bbox.first.latitude(),   coord.latitude()));
        bbox.first.setLongitude( std::min(bbox.first.longitude(),  coord.longitude()));
        bbox.second.setLatitude( std::max(bbox.second.latitude(),  coord.latitude()));
        bbox.second.setLongitude(std::max(bbox.second.longitude(), coord.longitude()));
    }
    gmap->zoomWindow(bbox);


    Wt::Ext::Button *btnCancel = new Wt::Ext::Button("Cancel");
    mapDlg_->addButton(btnCancel);
    btnCancel->clicked().connect(SLOT(this, FlightTableRow::closeDlg));

    mapDlg_->show();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTableRow::closeDlg()
{
    mapDlg_->accept();
    delete mapDlg_;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
FlightTable::FlightTable(boost::shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : DetailTableBase(flightDb, parent)
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTable::filter(const std::string &area)
{
    flights_.clear();

    if(area == "alle")
        brng::copy(flightDb_->flights(), back_inserter(flights_));
    else
        for(const auto& fl : flightDb_->flights())
            if(area == fl->takeoff()->area()->name())
                flights_.push_back(fl);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTable::loadPage(unsigned int page)
{
    pageNr_ = page;

    clear();
    createHeaderRow();
    const size_t nFirst = (pageNr_ - 1) * entriesPerPage_;
    if(nFirst < flights_.size())
    {
        auto ibeg = flights_.begin();
        std::advance(ibeg, (pageNr_ - 1) * entriesPerPage_);
        auto iend = ibeg;
        const int nLast = std::min<int>(flights_.size(), nFirst + entriesPerPage_);
        std::advance(iend, nLast - nFirst);
        for(size_t i=1; ibeg != iend; ++ibeg, ++i)
            addFlight(*ibeg, i);
        insertRowNr_ = nLast - nFirst + 1;
    }
    else
        insertRowNr_ = flights_.size() + 1;
    createFooterRow();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTable::createHeaderRow()
{
    static const std::string captions[8] = {"Nr",         "Datum", "Gleitschirm", "Startplatz",
                                            "Landeplatz", "Dauer", "Hoehendiff",  "Strecke"};
    for(size_t i=0; i<sizeof(captions) / sizeof(std::string); ++i)
    {
        Wt::WText *labelText = new Wt::WText(captions[i]);
        //labelText_->setFormatting(labelFormatting);
        labelText->setStyleClass("tableHeader");
        elementAt(0, i + 1)->addWidget(labelText);
    }
    rowAt(0)->setStyleClass("title");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTable::createFooterRow()
{
    Wt::WImage *wiAdd = new Wt::WImage("img/add.png");
    wiAdd->setAlternateText("add new flight");
    wiAdd->setToolTip("Flug hinzufuegen");
	elementAt(insertRowNr_++, 0)->addWidget(wiAdd);
	wiAdd->clicked().connect(SLOT(this, FlightTable::addNewFlight));
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
FlightTableRow* FlightTable::addFlight(boost::shared_ptr<Flight> fl, size_t row, bool newEntry)
{
	FlightTableRow *flr = new FlightTableRow(fl, this, flightDb_, row, newEntry);
	flr->show();
	rows_.push_back(flr);
	insertRowNr_ = row + 1;
	return flr;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightTable::addNewFlight()
{
    if(flightDb_->flights().size())
    {
        // create a new flight by copying the last flight and modify some fields
        boost::shared_ptr<Flight> newFlight(new Flight(**flightDb_->flights().rbegin()));
        newFlight->setNumber(newFlight->number() + 1);
        newFlight->setDate(boost::gregorian::day_clock::local_day());
        newFlight->setDuration(boost::posix_time::time_duration(0, 0, 0));
        newFlight->clearWaypoints();
        flightDb_->addFlight(newFlight);
        FlightTableRow *newRow = addFlight(newFlight, --insertRowNr_, true);
        newRow->edit();
    }
    else
    {
        // first make sure, we have gliders and locations defined
        if(!flightDb_->Gliders.size())
        {
            Wt::Ext::MessageBox::show("Error", "Bitte erfassen Sie zuerst ein Fluggeraet.", Wt::Ok, true);
            return;
        }
        // find the first takeoff and the first landing place
        const auto itTo = brng::find_if(flightDb_->Locations, [](const boost::shared_ptr<Location>& loc)
            { return loc->usage() & Location::UA_TAKEOFF; });
        const auto itLa = brng::find_if(flightDb_->Locations, [](const boost::shared_ptr<Location>& loc)
            { return loc->usage() & Location::UA_LANDING; });
        if(itTo == flightDb_->Locations.end())
        {
            Wt::Ext::MessageBox::show("Error", "Bitte erfassen Sie zuerst einen Startplatz.", Wt::Ok, true);
            return;
        }
        if(itLa == flightDb_->Locations.end())
        {
            Wt::Ext::MessageBox::show("Error", "Bitte erfassen Sie zuerst einen Landeplatz.", Wt::Ok, true);
            return;
        }
        // create the new flight
        boost::shared_ptr<Flight> newFlight(new Flight(1,                              // number
                                                boost::gregorian::day_clock::local_day(),  // date
                                                boost::posix_time::time_duration(0, 0, 0), // airtime
                                                *flightDb_->Gliders.begin(),    // glider
                                                *itTo,                          // takeoff
                                                *itLa));                        // landing
        flightDb_->addFlight(newFlight);
        FlightTableRow *newRow = addFlight(newFlight, --insertRowNr_, true);
        newRow->edit();
    }
    createFooterRow();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
FlightPanel::FlightPanel(boost::shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), impl_(new Wt::WContainerWidget())
{
    setImplementation(impl_);
    setStyleClass("flb_detail_panel");
    impl_->setStyleClass("flb_detail_panel");

    cbArea_ = new Wt::Ext::ComboBox(impl_);
    table_  = new FlightTable(flightDb, impl_);
    pglist_ = new PagesList(table_);
    // signals
    cbArea_->activated().connect(SLOT(this, FlightPanel::filter));

    // header
    Wt::WTable *topBar = new Wt::WTable();
    topBar->setStyleClass("FilterBar");
    Wt::WText *wtFilt = new Wt::WText("Filter : ");
    wtFilt->setStyleClass("FilterTitle");
    Wt::WText *wtArea = new Wt::WText("Fluggebiet");
    wtArea->setStyleClass("FilterSubTitle");
    topBar->elementAt(0, 0)->addWidget(wtFilt);
    topBar->elementAt(0, 1)->addWidget(wtArea);
    topBar->elementAt(0, 2)->addWidget(cbArea_);
    topBar->elementAt(0, 3)->addWidget(pglist_);

    Wt::WBorderLayout *borderLayout = new Wt::WBorderLayout();
    borderLayout->setLayoutHint("table-layout", "fixed");
    impl_->setLayout(borderLayout);
    borderLayout->addWidget(topBar, Wt::WBorderLayout::North);
    borderLayout->addWidget(table_, Wt::WBorderLayout::Center);

    load();
    filter();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightPanel::load()
{
    cbArea_->clear();
    cbArea_->addItem("alle");
    std::vector<std::string> areaNames;
    brng::transform(flightDb_->FlightAreas, back_inserter(areaNames), [](const boost::shared_ptr<FlightArea>& area)
        {
            return area->name();
        });
    std::sort(areaNames.begin(), areaNames.end());
    for(const auto& area : areaNames)
        cbArea_->addItem(area);
    cbArea_->setCurrentIndex(0);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightPanel::filter()
{
    const std::string area = cbArea_->currentText().narrow();

    table_->filter(area);
    table_->loadPage(1);
    pglist_->load();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A


