// flugbuch
#include "AreaTable.h"
#include "FormatStr.h"
// witty
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/MessageBox>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WHBoxLayout>
#include <Wt/WBorderLayout>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/checked_delete.hpp>
//#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
// std lib
#include <stdexcept>
#include <set>

using namespace flbwt;
using namespace flb;
using Wt::WText;
using Wt::WImage;
using std::string;
using std::vector;
//using boost::bind;
using namespace boost::lambda;
using boost::shared_ptr;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTableRow::clearRow()
{
    // set the edit field pointers to null
    edName_ = edCountry_ = edDesc_ = NULL;

	// clear the previous content from the row
	for(int i=0; i<4; ++i)
	{
		Wt::WTableCell *cell = table_->elementAt(rowNr_, i);
		cell->clear();
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTableRow::show()
{
	clearRow();

	WImage *wiEdit = new WImage("img/edit.png");
	wiEdit->setToolTip("Fluggebiet bearbeiten");
	wiEdit->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiEdit);
	wiEdit->clicked.connect(SLOT(this, AreaTableRow::edit));

	WImage *wiDelete = new WImage("img/delete.png");
	wiDelete->setToolTip("Fluggebiet löschen");
	wiDelete->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiDelete);
	wiDelete->clicked.connect(SLOT(this, AreaTableRow::remove));

	// prepare the text
	vector<string> vsText;
	vsText.push_back(area_->name());
	vsText.push_back(area_->country());
	vsText.push_back(area_->description());
	// add the text widgets
	for(size_t i=0; i<vsText.size(); ++i)
	{
		WText *wtxt = new WText(vsText[i]);
		wtxt->setStyleClass("tableContent");
		table_->elementAt(rowNr_, i + 1)->addWidget(wtxt);
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTableRow::edit()
{
	clearRow();

    table_->elementAt(rowNr_, colOp)->setLayout(new Wt::WHBoxLayout());
	// the save image
	WImage *wiSave = new WImage("img/save.png");
	wiSave->setToolTip("speichern");
	wiSave->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->layout()->addWidget(wiSave);
	wiSave->clicked.connect(SLOT(this, AreaTableRow::save));
	// the cancel image
	WImage *wiCancel = new WImage("img/undo.png");
	wiCancel->setToolTip("abbrechen");
	wiCancel->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->layout()->addWidget(wiCancel);
	if(isNewEntry_)
        wiCancel->clicked.connect(SLOT(this, AreaTableRow::remove));
    else
        wiCancel->clicked.connect(SLOT(this, AreaTableRow::show));
    // name
    edName_ = new Wt::Ext::LineEdit();
    edName_->setText(area_->name());
	table_->elementAt(rowNr_, colName)->addWidget(edName_);
    // country
    edCountry_ = new Wt::Ext::LineEdit();
    edCountry_->setText(area_->country());
	table_->elementAt(rowNr_, colCountry)->addWidget(edCountry_);
    // description
    edDesc_ = new Wt::Ext::LineEdit();
    edDesc_->setText(area_->description());
	table_->elementAt(rowNr_, colDesc)->addWidget(edDesc_);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTableRow::save()
{
	// save the information in the edit fields back to the internal representation
    try
    {
        // name
        assert(edName_);
        area_->setName(edName_->text().narrow());
        // country
        assert(edCountry_);
        area_->setCountry(edCountry_->text().narrow());
        // description
        assert(edDesc_);
        area_->setDescription(edDesc_->text().narrow());

    	// display the information as text without the edit fields
        show();
    }
    catch(std::exception &ex)
    {
		Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Warning, true);

    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTableRow::remove()
{
    try
    {
        flightDb_->deleteFlightArea(area_);
        clearRow();
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Warning, true);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
AreaTable::AreaTable(boost::shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : DetailTableBase(flightDb, parent)
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTable::createHeaderRow()
{
    string captions[3] = {"Name", "Land", "Beschreibung"};
    for(size_t i=0; i<sizeof(captions) / sizeof(string); ++i)
    {
        WText *labelText = new WText(captions[i]);
        labelText->setStyleClass("tableHeader");
        elementAt(0, i + 1)->addWidget(labelText);
    }
    rowAt(0)->setStyleClass("title");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTable::createFooterRow()
{
    WImage *wiAdd = new WImage("img/add.png");
    wiAdd->setAlternateText("add new flight");
    wiAdd->setToolTip("Flug hinzufuegen");
	elementAt(insertRowNr_, 0)->addWidget(wiAdd);
	wiAdd->clicked.connect(SLOT(this, AreaTable::addNewArea));
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
AreaTableRow* AreaTable::addArea(shared_ptr<FlightArea> flar, size_t row, bool newEntry)
{
	AreaTableRow *flr = new AreaTableRow(flar, this, flightDb_, row, newEntry);
	flr->show();
	rows_.push_back(flr);
	return flr;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTable::addNewArea()
{
    shared_ptr<FlightArea> newArea(new FlightArea("", "", ""));
    flightDb_->addFlightArea(newArea);
    AreaTableRow *newRow = addArea(newArea, insertRowNr_++, true);
    newRow->edit();
    createFooterRow();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTable::filter(const std::string &country)
{
    areas_.clear();

    if(country == "alle")
        std::copy(flightDb_->flightAreas().begin(), flightDb_->flightAreas().end(), back_inserter(areas_));
    else
    {
        std::remove_copy_if(flightDb_->flightAreas().begin(), flightDb_->flightAreas().end(), back_inserter(areas_),
            country != bind(&FlightArea::country, *boost::lambda::_1));

//        BOOST_FOREACH(shared_ptr<FlightArea> flar, flightDb_->flightAreas())
//            if(flar->country() == country)
//                areas_.push_back(loc);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaTable::loadPage(unsigned int page)
{
    pageNr_ = page;

    clear();
    createHeaderRow();
    const int nFirst = (pageNr_ - 1) * entriesPerPage_;
    if(nFirst < areas_.size())
    {
        vector<shared_ptr<FlightArea> >::iterator ibeg = areas_.begin();
        std::advance(ibeg, (pageNr_ - 1) * entriesPerPage_);
        vector<shared_ptr<FlightArea> >::iterator iend = ibeg;
        const int nLast = std::min<int>(areas_.size(), nFirst + entriesPerPage_);
        std::advance(iend, nLast - nFirst);
        for(size_t i=1; ibeg != iend; ++ibeg, ++i)
            addArea(*ibeg, i);
        insertRowNr_ = nLast - nFirst + 1;
    }
    else
        insertRowNr_ = areas_.size() + 1;
    createFooterRow();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
AreaPanel::AreaPanel(shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), impl_(new Wt::WContainerWidget())
{
    setImplementation(impl_);

    cbCountry_ = new Wt::Ext::ComboBox(impl_);
    table_  = new AreaTable(flightDb, impl_);
    pglist_ = new PagesList(table_);
    // signals
    cbCountry_->activated.connect(SLOT(this, AreaPanel::filter));
    // header
    Wt::WContainerWidget *topBar = new Wt::WContainerWidget();
    topBar->setLayout(new Wt::WHBoxLayout());
    WText *wtFilt = new WText("Filter : ");
    wtFilt->setStyleClass("FilterTitle");
    WText *wtCountry = new WText("Land");
    wtCountry->setStyleClass("FilterSubTitle");

    topBar->layout()->addWidget(wtFilt);
    topBar->layout()->addWidget(wtCountry);
    topBar->layout()->addWidget(cbCountry_);
    topBar->layout()->addWidget(pglist_);
    topBar->resize(topBar->width(), 40);

    Wt::WBorderLayout *borderLayout = new Wt::WBorderLayout();
    impl_->setLayout(borderLayout);
    borderLayout->addWidget(topBar, Wt::WBorderLayout::North);
    borderLayout->addWidget(table_, Wt::WBorderLayout::Center);

    load();
    filter();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaPanel::load()
{
    cbCountry_->clear();
    cbCountry_->addItem("alle");
    std::set<string> countries;
    transform(flightDb_->flightAreas().begin(), flightDb_->flightAreas().end(), inserter(countries, countries.begin()),
        bind(&FlightArea::country, *boost::lambda::_1));
//    for_each(countries.begin(), countries.end(), bind(&Wt::Ext::ComboBox::addItem, cbCountry_, *boost::lambda::_1));
    BOOST_FOREACH(string str, countries)
        cbCountry_->addItem(str);
    cbCountry_->setCurrentIndex(0);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void AreaPanel::filter()
{
    const string country = cbCountry_->currentText().narrow();

    table_->filter(country);
    table_->loadPage(1);
    pglist_->load();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8


