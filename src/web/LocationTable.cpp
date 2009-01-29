// flugbuch
#include "LocationTable.h"
#include "FormatStr.h"
// witty
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/CheckBox>
#include <Wt/Ext/NumberField>
#include <Wt/Ext/MessageBox>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WGridLayout>
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
PositionField::PositionField(flb::PositionFormat fmt, Wt::WContainerWidget *parent)
  : Wt::WCompositeWidget(parent), format_(fmt), impl_(new Wt::WContainerWidget())
{
    cbNoSo_ = cbEaWe_ = NULL;
    nfLatDeg_ = nfLatMin_ = nfLatSec_ = nfLonDeg_ = nfLonMin_ = nfLonSec_ = nfGridX_ = nfGridY_ = NULL;
    leZone_ = NULL;

    switch(format_)
    {
        case WGS84_SEC:
            cbNoSo_   = new Wt::Ext::ComboBox(impl_);
            cbNoSo_->addItem("N");
            cbNoSo_->addItem("S");
            nfLatDeg_ = new Wt::Ext::NumberField(impl_);
            nfLatMin_ = new Wt::Ext::NumberField(impl_);
            nfLatSec_ = new Wt::Ext::NumberField(impl_);
            cbEaWe_   = new Wt::Ext::ComboBox(impl_);
            cbEaWe_->addItem("E");
            cbEaWe_->addItem("W");
            nfLonDeg_ = new Wt::Ext::NumberField(impl_);
            nfLonMin_ = new Wt::Ext::NumberField(impl_);
            nfLonSec_ = new Wt::Ext::NumberField(impl_);
            break;
        case WGS84_MIN:
            cbNoSo_   = new Wt::Ext::ComboBox(impl_);
            cbNoSo_->addItem("N");
            cbNoSo_->addItem("S");
            nfLatDeg_ = new Wt::Ext::NumberField(impl_);
            nfLatMin_ = new Wt::Ext::NumberField(impl_);
            cbEaWe_   = new Wt::Ext::ComboBox(impl_);
            cbEaWe_->addItem("E");
            cbEaWe_->addItem("W");
            nfLonDeg_ = new Wt::Ext::NumberField(impl_);
            nfLonMin_ = new Wt::Ext::NumberField(impl_);
            break;
        case WGS84_DEC:
            cbNoSo_   = new Wt::Ext::ComboBox(impl_);
            cbNoSo_->addItem("N");
            cbNoSo_->addItem("S");
            nfLatDeg_ = new Wt::Ext::NumberField(impl_);
            cbEaWe_   = new Wt::Ext::ComboBox(impl_);
            cbEaWe_->addItem("E");
            cbEaWe_->addItem("W");
            nfLonDeg_ = new Wt::Ext::NumberField(impl_);
            break;
        case WGS84_UTM:
            leZone_   = new Wt::Ext::LineEdit(impl_);
            // no break
        case SWISSGRID:
            nfGridX_  = new Wt::Ext::NumberField(impl_);
            nfGridY_  = new Wt::Ext::NumberField(impl_);
            break;
        default:
            assert(!"unknown position format");
    }

    setImplementation(impl_);
    impl_->setLayout(new Wt::WHBoxLayout());
    if(cbNoSo_)
    {
        cbNoSo_->resize(20, cbNoSo_->height());
        impl_->layout()->addWidget(cbNoSo_);
    }
    if(nfLatDeg_)
    {
        nfLatDeg_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLatDeg_);
    }
    if(nfLatMin_)
    {
        nfLatMin_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLatMin_);
    }
    if(nfLatSec_)
    {
        nfLatSec_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLatSec_);
    }
    if(cbEaWe_)
    {
        cbEaWe_->resize(20, cbNoSo_->height());
        impl_->layout()->addWidget(cbEaWe_);
    }
    if(nfLonDeg_)
    {
        nfLonDeg_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLonDeg_);
    }
    if(nfLonMin_)
    {
        nfLonMin_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLonMin_);
    }
    if(nfLonSec_)
    {
        nfLonSec_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLonSec_);
    }
    if(leZone_)
    {
        leZone_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(leZone_);
    }
    if(nfGridX_)
    {
        nfGridX_->resize(80, cbNoSo_->height());
        impl_->layout()->addWidget(nfGridX_);
    }
    if(nfGridY_)
    {
        nfGridY_->resize(80, cbNoSo_->height());
        impl_->layout()->addWidget(nfGridY_);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void PositionField::setPos(std::pair<double, double> pos)
{
    double tmpval;
    switch(format_)
    {
        case WGS84_SEC:
            cbNoSo_->setCurrentIndex(pos.first >= 0.0 ? 0 : 1);
            tmpval = fabs(pos.first);
            nfLatDeg_->setValue(static_cast<int>(tmpval));
            tmpval = (tmpval - static_cast<int>(tmpval)) * 60.0;
            nfLatMin_->setValue(static_cast<int>(tmpval));
            nfLatSec_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            cbEaWe_->setCurrentIndex(pos.second >= 0.0 ? 0 : 1);
            tmpval = fabs(pos.second);
            nfLonDeg_->setValue(static_cast<int>(tmpval));
            tmpval = (tmpval - static_cast<int>(tmpval)) * 60.0;
            nfLonMin_->setValue(static_cast<int>(tmpval));
            nfLonSec_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            break;
        case WGS84_MIN:
            cbNoSo_->setCurrentIndex(pos.first >= 0.0 ? 0 : 1);
            tmpval = fabs(pos.first);
            nfLatDeg_->setValue(static_cast<int>(tmpval));
            nfLatMin_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            cbEaWe_->setCurrentIndex(pos.second >= 0.0 ? 0 : 1);
            tmpval = fabs(pos.second);
            nfLonDeg_->setValue(static_cast<int>(tmpval));
            nfLonMin_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            break;
        case WGS84_DEC:
            cbNoSo_->setCurrentIndex(pos.first >= 0.0 ? 0 : 1);
            nfLatDeg_->setValue(pos.first);
            cbEaWe_->setCurrentIndex(pos.second >= 0.0 ? 0 : 1);
            nfLonDeg_->setValue(pos.second);
            break;
        case WGS84_UTM:
            throw std::logic_error("not implemented");
            break;
        case SWISSGRID:
            throw std::logic_error("not implemented");
            break;
        default:
            assert(!"unknown position format");
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<double, double> PositionField::pos() const
{
    pair<double, double> val = make_pair(0.0, 0.0);

    switch(format_)
    {
        case WGS84_SEC:
            val = make_pair((cbNoSo_->currentIndex() ? -1.0 : 1.0) * (nfLatSec_->value() / 60.0 + nfLatMin_->value()) / 60.0 + nfLatDeg_->value(),
                            (cbEaWe_->currentIndex() ? -1.0 : 1.0) * (nfLonSec_->value() / 60.0 + nfLonMin_->value()) / 60.0 + nfLonDeg_->value());
            break;
        case WGS84_MIN:
            val = make_pair((cbNoSo_->currentIndex() ? -1.0 : 1.0) * nfLatMin_->value() / 60.0 + nfLatDeg_->value(),
                            (cbEaWe_->currentIndex() ? -1.0 : 1.0) * nfLonMin_->value() / 60.0 + nfLonDeg_->value());
            break;
        case WGS84_DEC:
            val = make_pair((cbNoSo_->currentIndex() ? -1.0 : 1.0) * nfLatDeg_->value(),
                            (cbEaWe_->currentIndex() ? -1.0 : 1.0) * nfLonDeg_->value());
            break;
        case WGS84_UTM:
            throw std::logic_error("not implemented");
            break;
        case SWISSGRID:
            throw std::logic_error("not implemented");
            break;
        default:
            assert(!"unknown position format");
    }
    return val;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
string PositionField::format(pair<double, double> pos, flb::PositionFormat fmt)
{
    std::stringstream sstr;
    double tmpval;
    switch(fmt)
    {
        case WGS84_SEC:
            sstr << (pos.first >= 0.0 ? "N " : "S ");
            tmpval = fabs(pos.first);
            sstr << static_cast<int>(tmpval) << "° ";
            tmpval = (tmpval - static_cast<int>(tmpval)) * 60.0;
            sstr << static_cast<int>(tmpval) << "' ";
            sstr << ((tmpval - static_cast<int>(tmpval)) * 60.0) << "\"   ";
            sstr << (pos.second >= 0.0 ? "O " : "W ");
            tmpval = pos.second;
            sstr << static_cast<int>(tmpval) << "° ";
            tmpval = (tmpval - static_cast<int>(tmpval)) * 60.0;
            sstr << static_cast<int>(tmpval) << "' ";
            sstr << ((tmpval - static_cast<int>(tmpval)) * 60.0) << "\"";
            break;
        case WGS84_MIN:
            sstr << (pos.first >= 0.0 ? "N " : "S ");
            tmpval = fabs(pos.first);
            sstr << static_cast<int>(tmpval) << "° ";
            sstr << ((tmpval - static_cast<int>(tmpval)) * 60.0) << "'   ";
            sstr << (pos.second >= 0.0 ? "O " : "W ");
            tmpval = pos.second;
            sstr << static_cast<int>(tmpval) << "° ";
            sstr << ((tmpval - static_cast<int>(tmpval)) * 60.0) << "' ";
            break;
        case WGS84_DEC:
            sstr << (pos.first >= 0.0 ? "N " : "S ");
            sstr << pos.first << "° ";
            sstr << (pos.second >= 0.0 ? "O " : "W ");
            sstr << pos.second << "° ";
            break;
        case WGS84_UTM:
            throw std::logic_error("not implemented");
            break;
        case SWISSGRID:
            throw std::logic_error("not implemented");
            break;
        default:
            assert(!"unknown position format");
    }
    return sstr.str();
}
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

	WImage *wiEdit = new WImage("img/edit.png");
	wiEdit->setToolTip("Ort bearbeiten");
	wiEdit->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiEdit);
	wiEdit->clicked.connect(SLOT(this, LocationTableRow::edit));

	WImage *wiDelete = new WImage("img/delete.png");
	wiDelete->setToolTip("Ort löschen");
	wiDelete->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiDelete);
	wiDelete->clicked.connect(SLOT(this, LocationTableRow::remove));

	// prepare the text
	vector<string> vsText;
	vsText.push_back(location_->area()->name());
	vsText.push_back(location_->name());
	vsText.push_back(lexical_cast<string>(location_->height()));
	vsText.push_back(PositionField::format(make_pair(location_->latitude(), location_->longitude()), flb::WGS84_SEC));
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
    pfPosition_ = new PositionField(flb::WGS84_SEC);
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
        std::copy(flightDb_->locations().begin(), flightDb_->locations().end(), back_inserter(locations_));
    else
    {
//        std::remove_copy_if(flightDb_->locations().begin(), flightDb_->locations().end(), back_inserter(locations_),
//            area != bind(&FlightArea::name, *(bind(&Location::area, *boost::lambda::_1))));

        BOOST_FOREACH(shared_ptr<Location> loc, flightDb_->locations())
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
    const int nFirst = (pageNr_ - 1) * entriesPerPage_;
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
    cbTakeoff_->dataChanged.connect(SLOT(this, LocationPanel::filter));
    cbLanding_->dataChanged.connect(SLOT(this, LocationPanel::filter));
    cbWayPnt_->dataChanged.connect(SLOT(this, LocationPanel::filter));

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

