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
//#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/array.hpp>
// std lib
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <cmath>

using namespace flbwt;
using namespace flb;
using Wt::WText;
using Wt::WImage;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::fabs;
using std::sort;
using std::transform;
//using boost::bind;
using namespace boost::lambda;
using boost::shared_ptr;
using boost::lexical_cast;


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
	WImage *wiEdit = new WImage("img/edit.png");
	wiEdit->setToolTip("Ort bearbeiten");
	wiEdit->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiEdit);
	wiEdit->clicked.connect(SLOT(this, LocationTableRow::edit));
    // the delete image
	WImage *wiDelete = new WImage("img/delete.png");
	wiDelete->setToolTip("Ort löschen");
	wiDelete->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiDelete);
	wiDelete->clicked.connect(SLOT(this, LocationTableRow::remove));
	// the map image
//	WImage *wiMap = new WImage("img/map.png");
//	wiMap->setToolTip("position anschauen");
//    wiMap->setStyleClass("operationImg");
//	table_->elementAt(rowNr_, colOp)->addWidget(wiMap);
//	wiMap->clicked.connect(SLOT(this, LocationTableRow::map));

	// prepare the text
	vector<string> vsText;
	vsText.push_back(location_->area()->name());
	vsText.push_back(location_->name());
	vsText.push_back(lexical_cast<string>(location_->height()));
	vsText.push_back(Wt::WGeoPosEdit::format(make_pair(location_->latitude(), location_->longitude()), Wt::WGeoPosEdit::WGS84_SEC));
	vsText.push_back(location_->usage() & Location::UA_TAKEOFF ? "x" : "_");
	vsText.push_back(location_->usage() & Location::UA_LANDING ? "x" : "_");
	vsText.push_back(location_->usage() & Location::UA_WAYPNT  ? "x" : "_");
	// add the text widgets
	for(size_t i=0; i<vsText.size(); ++i)
	{
		WText *wtxt = new WText(vsText[i]);
		wtxt->setStyleClass("tableContent");
		table_->elementAt(rowNr_, i + 1)->addWidget(wtxt);
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTableRow::edit()
{
	clearRow();

    table_->elementAt(rowNr_, colOp)->setLayout(new Wt::WHBoxLayout());
	// the save image
	WImage *wiSave = new WImage("img/save.png");
	wiSave->setToolTip("speichern");
    wiSave->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->layout()->addWidget(wiSave);
	wiSave->clicked.connect(SLOT(this, LocationTableRow::save));
	// the cancel image
	WImage *wiCancel = new WImage("img/undo.png");
	wiCancel->setToolTip("abbrechen");
	wiCancel->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->layout()->addWidget(wiCancel);
	if(isNewEntry_)
        wiCancel->clicked.connect(SLOT(this, LocationTableRow::remove));
    else
        wiCancel->clicked.connect(SLOT(this, LocationTableRow::show));

    // area
    cbArea_ = new Wt::Ext::ComboBox();
    for_each(flightDb_->FlightAreas.begin(), flightDb_->FlightAreas.end(), bind(&Wt::Ext::ComboBox::addItem, cbArea_, bind(&FlightArea::name, *boost::lambda::_1)));
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
    pfPosition_->setPos(make_pair(location_->latitude(), location_->longitude()));
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
        shared_ptr<FlightArea> area = flightDb_->getArea(cbArea_->currentText().narrow());
        location_->setArea(area);
        // name
        assert(edName_);
        location_->setName(edName_->text().narrow());
        // height
        assert(nfHeight_);
        location_->setHeight(nfHeight_->value());
        // position
        assert(pfPosition_);
        location_->setPosition(pfPosition_->pos());
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
		Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Warning, true);
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
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Warning, true);
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
	gmap->addHierarchicalMapTypeControl();
    const pair<double, double> pdpos = location_->pos();
    if(pdpos.first != 0.0 && pdpos.second != 0.0)
    {
        Wt::WLatLng lalo(pdpos.first, pdpos.second);
        gmap->setCenter(lalo, 13);
        gmap->addMarker(lalo);
    }
    Wt::Ext::Button *btnCancel = new Wt::Ext::Button("Cancel");
    mapDlg_->addButton(btnCancel);
    btnCancel->clicked.connect(SLOT(this, LocationTableRow::closeDlg));

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
    string captions[7] = {"Gebiet", "Name", "Höhe", "Position", "SP", "LP", "WP"};
    for(size_t i=0; i<sizeof(captions) / sizeof(string); ++i)
    {
        WText *labelText = new WText(captions[i]);
        labelText->setStyleClass("tableHeader");
        elementAt(0, i + 1)->addWidget(labelText);
    }
    rowAt(0)->setStyleClass("title");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationTable::createFooterRow()
{
    WImage *wiAdd = new WImage("img/add.png");
    wiAdd->setAlternateText("add new flight");
    wiAdd->setToolTip("Orte hinzufuegen");
	elementAt(insertRowNr_, 0)->addWidget(wiAdd);
	wiAdd->clicked.connect(SLOT(this, LocationTable::addNewLocation));
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
LocationTableRow* LocationTable::addLocation(shared_ptr<Location> loc, size_t row, bool newEntry)
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
        Wt::Ext::MessageBox::show("Error", "Bitte erfassen Sie zuerst ein Fluggebiet.", Wt::Warning, true);
        return;
    }
    shared_ptr<FlightArea> area = *flightDb_->FlightAreas.begin();

    shared_ptr<Location> newLocation(new Location(area, "", 0, 0.0, 0.0, Location::UA_TAKEOFF));
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
//        std::remove_copy_if(flightDb_->locations().begin(), flightDb_->locations().end(), back_inserter(locations_),
//            area != bind(&FlightArea::name, *(bind(&Location::area, *boost::lambda::_1))));

        BOOST_FOREACH(shared_ptr<Location> loc, flightDb_->Locations)
            if(loc->area()->name() == area)
                locations_.push_back(loc);
    }
    if(!takeoff && !landing && !waypnt)
        return;
    if(takeoff)
        locations_.erase(std::remove_if(locations_.begin(), locations_.end(),
            !(bind(&Location::usage, *boost::lambda::_1) & static_cast<int>(Location::UA_TAKEOFF))), locations_.end());
    if(landing)
        locations_.erase(std::remove_if(locations_.begin(), locations_.end(),
            !(bind(&Location::usage, *boost::lambda::_1) & static_cast<int>(Location::UA_LANDING))), locations_.end());
    if(waypnt)
        locations_.erase(std::remove_if(locations_.begin(), locations_.end(),
            !(bind(&Location::usage, *boost::lambda::_1) & static_cast<int>(Location::UA_WAYPNT))), locations_.end());
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
        vector<shared_ptr<Location> >::iterator ibeg = locations_.begin();
        std::advance(ibeg, (pageNr_ - 1) * entriesPerPage_);
        vector<shared_ptr<Location> >::iterator iend = ibeg;
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
LocationPanel::LocationPanel(shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), impl_(new Wt::WContainerWidget())
{
    setImplementation(impl_);

    cbArea_ = new Wt::Ext::ComboBox(impl_);
    cbTakeoff_ = new Wt::Ext::CheckBox(impl_);
	cbLanding_ = new Wt::Ext::CheckBox(impl_);
	cbWayPnt_  = new Wt::Ext::CheckBox(impl_);
    table_  = new LocationTable(flightDb, impl_);
    pglist_ = new PagesList(table_);
    // signals
    cbArea_->activated.connect(SLOT(this, LocationPanel::filter));
    cbTakeoff_->checked.connect(SLOT(this, LocationPanel::filter));
    cbLanding_->checked.connect(SLOT(this, LocationPanel::filter));
    cbWayPnt_->checked.connect(SLOT(this, LocationPanel::filter));
    cbTakeoff_->unChecked.connect(SLOT(this, LocationPanel::filter));
    cbLanding_->unChecked.connect(SLOT(this, LocationPanel::filter));
    cbWayPnt_->unChecked.connect(SLOT(this, LocationPanel::filter));

/*
    cbTakeoff_->checked.connect(SLOT(this, LocationPanel::filter));
    cbTakeoff_->unChecked.connect(SLOT(this, LocationPanel::filter));
    cbLanding_->checked.connect(SLOT(this, LocationPanel::filter));
    cbLanding_->unChecked.connect(SLOT(this, LocationPanel::filter));
    cbWayPnt_->checked.connect(SLOT(this, LocationPanel::filter));
    cbWayPnt_->unChecked.connect(SLOT(this, LocationPanel::filter));
*/
    // header
    Wt::WContainerWidget *topBar = new Wt::WContainerWidget();
    topBar->setLayout(new Wt::WGridLayout());
    WText *wtFilt = new WText("Filter : ");
    wtFilt->setStyleClass("FilterTitle");
    WText *wtArea = new WText("Fluggebiet");
    wtArea->setStyleClass("FilterSubTitle");
//    WText *wtTakeoff = new WText("Startplaetze");
//    wtTakeoff->setStyleClass("tableFilter");
//    WText *wtLanding = new WText("Landeplaetze");
//    wtLanding->setStyleClass("tableFilter");
//    WText *wtWaypoint = new WText("Wegpunkte");
//    wtWaypoint->setStyleClass("tableFilter");
    cbTakeoff_->setText("Startplaetze");
    cbLanding_->setText("Landeplaetze");
    cbWayPnt_->setText("Wegpunkte");
    topBar->layout()->addWidget(wtFilt);
    topBar->layout()->addWidget(wtArea);
    topBar->layout()->addWidget(cbArea_);
//    topBar->layout()->addWidget(wtTakeoff);
    topBar->layout()->addWidget(cbTakeoff_);
//    topBar->layout()->addWidget(wtLanding);
    topBar->layout()->addWidget(cbLanding_);
//    topBar->layout()->addWidget(wtWaypoint);
    topBar->layout()->addWidget(cbWayPnt_);
    topBar->layout()->addWidget(pglist_);
    topBar->resize(topBar->width(), 40);

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
    vector<string> areaNames;
    transform(flightDb_->FlightAreas.begin(), flightDb_->FlightAreas.end(), back_inserter(areaNames), bind(&FlightArea::name, *boost::lambda::_1));
    sort(areaNames.begin(), areaNames.end());
    for_each(areaNames.begin(), areaNames.end(), bind(&Wt::Ext::ComboBox::addItem, cbArea_, boost::lambda::_1));
    cbArea_->setCurrentIndex(0);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void LocationPanel::filter()
{
    const string area = cbArea_->currentText().narrow();
    const bool takeoff = cbTakeoff_->isChecked();
    const bool landing = cbLanding_->isChecked();
    const bool waypnt  = cbWayPnt_->isChecked();

    table_->filter(area, takeoff, landing, waypnt);
    table_->loadPage(1);
    pglist_->load();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

