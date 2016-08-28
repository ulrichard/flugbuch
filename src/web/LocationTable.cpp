// flugbuch
#include "LocationTable.h"
#include "FormatStr.h"
// witty
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/CheckBox>
#include <Wt/Ext/NumberField>
#include <Wt/Ext/MessageBox>
#include <Wt/Ext/Dialog>
#include <Wt/Ext/Button>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WGridLayout>
#include <Wt/WSignalMapper>
#include <Wt/WGoogleMap>
#include <Wt/WGeoPosEdit>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/checked_delete.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/array.hpp>
#include <boost/range/algorithm.hpp>
// std lib
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <cmath>

using namespace flbwt;
using namespace flb;
namespace brng = boost::range;

typedef std::pair<double, double> point_ll_deg;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTableRow::clearRow()
{
    // set the edit field pointers to null
    cbArea_     = NULL;
	edName_     = NULL;
	nfHeight_   = NULL;
	pfPosition_ = NULL;
	cbTakeoff_  = cbLanding_ = cbWayPnt_ = NULL;

	// clear the previous content from the row
	for(int i=0; i<8; ++i)
	{
		Wt::WTableCell *cell = table_->elementAt(rowNr_, i);
		cell->clear();
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTableRow::show()
{
	clearRow();

    // the edit image
	Wt::WImage *wiEdit = new Wt::WImage("img/edit.png");
	wiEdit->setToolTip("Ort bearbeiten");
	wiEdit->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiEdit);
	wiEdit->clicked().connect(SLOT(this, LocationTableRow::edit));
    // the delete image
	Wt::WImage *wiDelete = new Wt::WImage("img/delete.png");
	wiDelete->setToolTip("Ort löschen");
	wiDelete->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiDelete);
	wiDelete->clicked().connect(SLOT(this, LocationTableRow::remove));
	// the map image
//	WImage *wiMap = new WImage("img/map.png");
//	wiMap->setToolTip("position anschauen");
//    wiMap->setStyleClass("operationImg");
//	table_->elementAt(rowNr_, colOp)->addWidget(wiMap);
//	wiMap->clicked.connect(SLOT(this, LocationTableRow::map));

	// prepare the text
	std::vector<std::string> vsText;
	vsText.push_back(location_->area()->name());
	vsText.push_back(location_->name());
	vsText.push_back(boost::lexical_cast<std::string>(location_->height()));
	vsText.push_back(Wt::WGeoPosEdit::format(std::make_pair(location_->pos().first, location_->pos().second), Wt::WGeoPosEdit::WGS84_SEC));
	vsText.push_back(location_->usage() & Location::UA_TAKEOFF ? "x" : "_");
	vsText.push_back(location_->usage() & Location::UA_LANDING ? "x" : "_");
	vsText.push_back(location_->usage() & Location::UA_WAYPNT  ? "x" : "_");
	// add the text widgets
	for(size_t i=0; i<vsText.size(); ++i)
	{
		Wt::WText *wtxt = new Wt::WText(vsText[i]);
		wtxt->setStyleClass("tableContent");
		table_->elementAt(rowNr_, i + 1)->addWidget(wtxt);
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTableRow::edit()
{
	clearRow();

	// the save image
	Wt::WImage *wiSave = new Wt::WImage("img/save.png");
	wiSave->setToolTip("speichern");
    wiSave->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiSave);
	wiSave->clicked().connect(SLOT(this, LocationTableRow::save));
	// the cancel image
	Wt::WImage *wiCancel = new Wt::WImage("img/undo.png");
	wiCancel->setToolTip("abbrechen");
	wiCancel->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiCancel);
	if(isNewEntry_)
        wiCancel->clicked().connect(SLOT(this, LocationTableRow::remove));
    else
        wiCancel->clicked().connect(SLOT(this, LocationTableRow::show));

    // area
    cbArea_ = new Wt::Ext::ComboBox();
    for(const auto& area : flightDb_->FlightAreas)
        cbArea_->addItem(area->name());
    for(int i=0; i<cbArea_->count(); ++i)
        if(location_->area()->name() == cbArea_->itemText(i).narrow())
            cbArea_->setCurrentIndex(i);
	table_->elementAt(rowNr_, colArea)->addWidget(cbArea_);
    // name
    edName_ = new Wt::Ext::LineEdit();
    edName_->setText(location_->name());
	table_->elementAt(rowNr_, colName)->addWidget(edName_);
    // height
    nfHeight_ = new Wt::Ext::NumberField();
    nfHeight_->setValue(location_->height());
	table_->elementAt(rowNr_, colHeight)->addWidget(nfHeight_);
    // position
    pfPosition_ = new Wt::WGeoPosEdit(0, Wt::WGeoPosEdit::WGS84_SEC);
    pfPosition_->setPos(std::make_pair(location_->pos().first, location_->pos().second));
	table_->elementAt(rowNr_, colPosition)->addWidget(pfPosition_);
	// use as takeoff
	cbTakeoff_ = new Wt::Ext::CheckBox();
	cbTakeoff_->setChecked(location_->usage() & Location::UA_TAKEOFF);
	table_->elementAt(rowNr_, colTakeoff)->addWidget(cbTakeoff_);
	// use as landing zone
	cbLanding_ = new Wt::Ext::CheckBox();
	cbLanding_->setChecked(location_->usage() & Location::UA_LANDING);
	table_->elementAt(rowNr_, colLanding)->addWidget(cbLanding_);
	// takeoff
	cbWayPnt_ = new Wt::Ext::CheckBox();
	cbWayPnt_->setChecked(location_->usage() & Location::UA_WAYPNT);
	table_->elementAt(rowNr_, colWaypnt)->addWidget(cbWayPnt_);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTableRow::save()
{
	// save the information in the edit fields back to the internal representation
    try
    {
        // area
        assert(cbArea_);
        boost::shared_ptr<FlightArea> area = flightDb_->getArea(cbArea_->currentText().narrow());
        location_->setArea(area);
        // name
        assert(edName_);
        location_->setName(edName_->text().narrow());
        // height
        assert(nfHeight_);
        location_->setHeight(nfHeight_->value());
        // position
        assert(pfPosition_);
        location_->setPosition(point_ll_deg(pfPosition_->pos().first, pfPosition_->pos().second));
        // usage
        assert(cbTakeoff_ && cbLanding_ && cbWayPnt_);
        int usage = 0;
        if(cbTakeoff_->isChecked())
            usage |= Location::UA_TAKEOFF;
        if(cbLanding_->isChecked())
            usage |= Location::UA_LANDING;
        if(cbWayPnt_->isChecked())
            usage |= Location::UA_WAYPNT;
        location_->setUsage(usage);

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
void LocationTableRow::remove()
{
    try
    {
        flightDb_->deleteLocation(location_);
        clearRow();
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Ok, true);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTableRow::map()
{
    mapDlg_ = new Wt::Ext::Dialog("View the location");
    mapDlg_->resize(500, 400);
    mapDlg_->setSizeGripEnabled(false);
    Wt::WGoogleMap *gmap = new Wt::WGoogleMap();
    mapDlg_->contents()->addWidget(gmap);
	gmap->resize(500, 400);
	gmap->enableScrollWheelZoom();
	gmap->disableDoubleClickZoom();
	gmap->enableDragging();
	gmap->setMapTypeControl(Wt::WGoogleMap::HierarchicalControl);
    const point_ll_deg pdpos = location_->pos();
    if(pdpos.first != 0.0 && pdpos.second != 0.0)
    {
        Wt::WGoogleMap::Coordinate lalo(pdpos.first, pdpos.second);
        gmap->setCenter(lalo, 13);
        gmap->addMarker(lalo);
    }
    Wt::Ext::Button *btnCancel = new Wt::Ext::Button("Cancel");
    mapDlg_->addButton(btnCancel);
    btnCancel->clicked().connect(SLOT(this, LocationTableRow::closeDlg));

    mapDlg_->show();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTableRow::closeDlg()
{
    mapDlg_->accept();
    delete mapDlg_;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
LocationTable::LocationTable(boost::shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : DetailTableBase(flightDb, parent)
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTable::createHeaderRow()
{
    static const std::string captions[7] = {"Gebiet", "Name", "Höhe", "Position", "SP", "LP", "WP"};
    for(size_t i=0; i<sizeof(captions) / sizeof(std::string); ++i)
    {
        Wt::WText *labelText = new Wt::WText(captions[i]);
        labelText->setStyleClass("tableHeader");
        elementAt(0, i + 1)->addWidget(labelText);
    }
    rowAt(0)->setStyleClass("title");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTable::createFooterRow()
{
    Wt::WImage *wiAdd = new Wt::WImage("img/add.png");
    wiAdd->setAlternateText("add new flight");
    wiAdd->setToolTip("Orte hinzufuegen");
	elementAt(insertRowNr_, 0)->addWidget(wiAdd);
	wiAdd->clicked().connect(SLOT(this, LocationTable::addNewLocation));
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
LocationTableRow* LocationTable::addLocation(boost::shared_ptr<Location> loc, size_t row, bool newEntry)
{
	LocationTableRow *locr = new LocationTableRow(loc, this, flightDb_, row, newEntry);
	locr->show();
	rows_.push_back(locr);
	return locr;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTable::addNewLocation()
{
    if(!flightDb_->FlightAreas.size())
    {
        Wt::Ext::MessageBox::show("Error", "Bitte erfassen Sie zuerst ein Fluggebiet.", Wt::Ok, true);
        return;
    }
    boost::shared_ptr<FlightArea> area = *flightDb_->FlightAreas.begin();

    boost::shared_ptr<Location> newLocation(new Location(area, "", 0, point_ll_deg(0.0, 0.0), Location::UA_TAKEOFF));
    flightDb_->addLocation(newLocation);
    LocationTableRow *newRow = addLocation(newLocation, insertRowNr_++, true);
    newRow->edit();
    createFooterRow();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTable::filter(const std::string &area, bool takeoff, bool landing, bool waypnt)
{
    locations_.clear();

    if(area == "alle")
        std::copy(flightDb_->Locations.begin(), flightDb_->Locations.end(), back_inserter(locations_));
    else
    {
        for(auto& loc : flightDb_->Locations)
            if(loc->area()->name() == area)
                locations_.push_back(loc);
    }
    if(!takeoff && !landing && !waypnt)
        return;
    if(takeoff)
        locations_.erase(brng::remove_if(locations_, [](boost::shared_ptr<Location>& loc)
            { return loc->usage() & Location::UA_TAKEOFF; }), locations_.end());
    if(landing)
        locations_.erase(brng::remove_if(locations_, [](boost::shared_ptr<Location>& loc)
            { return loc->usage() & Location::UA_LANDING; }), locations_.end());
    if(waypnt)
        locations_.erase(brng::remove_if(locations_, [](boost::shared_ptr<Location>& loc)
            { return loc->usage() & Location::UA_WAYPNT; }), locations_.end());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTable::loadPage(unsigned int page)
{
    pageNr_ = page;

    clear();
    createHeaderRow();
    const size_t nFirst = (pageNr_ - 1) * entriesPerPage_;
    if(nFirst < locations_.size())
    {
        auto ibeg = locations_.begin();
        std::advance(ibeg, (pageNr_ - 1) * entriesPerPage_);
        auto iend = ibeg;
        const int nLast = std::min<int>(locations_.size(), nFirst + entriesPerPage_);
        std::advance(iend, nLast - nFirst);
        for(size_t i=1; ibeg != iend; ++ibeg, ++i)
            addLocation(*ibeg, i);
        insertRowNr_ = nLast - nFirst + 1;
    }
    else
        insertRowNr_ = locations_.size() + 1;
    createFooterRow();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
LocationPanel::LocationPanel(boost::shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), impl_(new Wt::WContainerWidget())
{
    setImplementation(impl_);
    setStyleClass("flb_detail_panel");
    impl_->setStyleClass("flb_detail_panel");

    cbArea_ = new Wt::Ext::ComboBox(impl_);
    cbTakeoff_ = new Wt::Ext::CheckBox(impl_);
	cbLanding_ = new Wt::Ext::CheckBox(impl_);
	cbWayPnt_  = new Wt::Ext::CheckBox(impl_);
    table_  = new LocationTable(flightDb, impl_);
    pglist_ = new PagesList(table_);
    // signals
    cbArea_->activated().connect(SLOT(this, LocationPanel::filter));
    cbTakeoff_->checked().connect(SLOT(this, LocationPanel::filter));
    cbLanding_->checked().connect(SLOT(this, LocationPanel::filter));
    cbWayPnt_->checked().connect(SLOT(this, LocationPanel::filter));
    cbTakeoff_->unChecked().connect(SLOT(this, LocationPanel::filter));
    cbLanding_->unChecked().connect(SLOT(this, LocationPanel::filter));
    cbWayPnt_->unChecked().connect(SLOT(this, LocationPanel::filter));

    // header
    Wt::WTable *topBar = new Wt::WTable();
    topBar->setStyleClass("FilterBar");
    Wt::WText *wtFilt = new Wt::WText("Filter : ");
    wtFilt->setStyleClass("FilterTitle");
    Wt::WText *wtArea = new Wt::WText("Fluggebiet");
    wtArea->setStyleClass("FilterSubTitle");
    cbTakeoff_->setText("Startplaetze");
    cbLanding_->setText("Landeplaetze");
    cbWayPnt_->setText("Wegpunkte");
    topBar->elementAt(0, 0)->addWidget(wtFilt);
    topBar->elementAt(0, 1)->addWidget(wtArea);
    topBar->elementAt(0, 2)->addWidget(cbArea_);
    topBar->elementAt(0, 3)->addWidget(cbTakeoff_);
    topBar->elementAt(0, 4)->addWidget(cbLanding_);
    topBar->elementAt(0, 5)->addWidget(cbWayPnt_);
    topBar->elementAt(0, 6)->addWidget(pglist_);

    Wt::WContainerWidget *botBar = new Wt::WContainerWidget();

    Wt::WBorderLayout *borderLayout = new Wt::WBorderLayout();
    impl_->setLayout(borderLayout);
    borderLayout->addWidget(topBar, Wt::WBorderLayout::North);
    borderLayout->addWidget(table_, Wt::WBorderLayout::Center);
    borderLayout->addWidget(botBar, Wt::WBorderLayout::South);

    load();
    filter();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationPanel::load()
{
    cbArea_->clear();
    cbArea_->addItem("alle");
    std::vector<std::string> areaNames;
    brng::transform(flightDb_->FlightAreas, back_inserter(areaNames), [](const boost::shared_ptr<FlightArea>& area)
        { return area->name(); });
    brng::sort(areaNames);
    for(const auto& name : areaNames)
        cbArea_->addItem(name);
    cbArea_->setCurrentIndex(0);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationPanel::filter()
{
    const std::string area = cbArea_->currentText().narrow();
    const bool takeoff = cbTakeoff_->isChecked();
    const bool landing = cbLanding_->isChecked();
    const bool waypnt  = cbWayPnt_->isChecked();

    table_->filter(area, takeoff, landing, waypnt);
    table_->loadPage(1);
    pglist_->load();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

