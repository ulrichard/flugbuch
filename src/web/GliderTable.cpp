// flugbuch
#include "GliderTable.h"
#include "FormatStr.h"
// witty
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/NumberField>
#include <Wt/Ext/MessageBox>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WHBoxLayout>
#include <Wt/WBorderLayout>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
// std lib
#include <stdexcept>

using namespace flbwt;
using namespace flb;
namespace brng = boost::range;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderTableRow::clearRow()
{
    // set the edit field pointers to null
	edBrand_ = edModel_ = edSize_ = edColor_ = edClassi_ = edDesc_ = NULL;
    edYear_ = NULL;
    img_    = NULL;

	// clear the previous content from the row
	for(int i=0; i<9; ++i)
	{
		Wt::WTableCell *cell = table_->elementAt(rowNr_, i);
		cell->clear();
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderTableRow::show()
{
	clearRow();

	Wt::WImage *wiEdit = new Wt::WImage("img/edit.png");
	wiEdit->setToolTip("Schirm bearbeiten");
	wiEdit->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiEdit);
	wiEdit->clicked().connect(SLOT(this, GliderTableRow::edit));

	Wt::WImage *wiDelete = new Wt::WImage("img/delete.png");
	wiDelete->setToolTip("Schirm löschen");
	wiDelete->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiDelete);
	wiDelete->clicked().connect(SLOT(this, GliderTableRow::remove));

	// prepare the text
	std::vector<std::string> vsText;
	vsText.push_back(glider_->brand());
	vsText.push_back(glider_->model());
	vsText.push_back(glider_->size());
	vsText.push_back(glider_->color());
	vsText.push_back(boost::lexical_cast<std::string>(glider_->year()));
	vsText.push_back(glider_->classification());
	vsText.push_back(glider_->description());
	// add the text widgets
	for(size_t i=0; i<vsText.size(); ++i)
	{
		Wt::WText *wtxt = new Wt::WText(vsText[i]);
		wtxt->setStyleClass("tableContent");
		table_->elementAt(rowNr_, i + 1)->addWidget(wtxt);
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderTableRow::edit()
{
	clearRow();

	// the save image
	Wt::WImage *wiSave = new Wt::WImage("img/save.png");
	wiSave->setToolTip("speichern");
	wiSave->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiSave);
	wiSave->clicked().connect(SLOT(this, GliderTableRow::save));
    // the cancel image
	Wt::WImage *wiCancel = new Wt::WImage("img/undo.png");
	wiCancel->setToolTip("abbrechen");
	wiCancel->setStyleClass("operationImg");
	table_->elementAt(rowNr_, colOp)->addWidget(wiCancel);
	if(isNewEntry_)
        wiCancel->clicked().connect(SLOT(this, GliderTableRow::remove));
    else
        wiCancel->clicked().connect(SLOT(this, GliderTableRow::show));
	// brand
	edBrand_ = new Wt::Ext::LineEdit();
	edBrand_->setText(glider_->brand());
	edBrand_->resize(120, edBrand_->height());
	table_->elementAt(rowNr_, colBrand)->addWidget(edBrand_);
	// model
	edModel_ = new Wt::Ext::LineEdit();
	edModel_->setText(glider_->model());
	edModel_->resize(120, edModel_->height());
	table_->elementAt(rowNr_, colModel)->addWidget(edModel_);
	// size
	edSize_ = new Wt::Ext::LineEdit();
	edSize_->setText(glider_->size());
	edSize_->resize(50, edSize_->height());
	table_->elementAt(rowNr_, colSize)->addWidget(edSize_);
    // color
   	edColor_ = new Wt::Ext::LineEdit();
	edColor_->setText(glider_->color());
	edColor_->resize(100, edColor_->height());
	table_->elementAt(rowNr_, colColor)->addWidget(edColor_);
    // year
   	edYear_ = new Wt::Ext::NumberField();
	edYear_->setValue(glider_->year());
	edYear_->resize(50, edYear_->height());
	table_->elementAt(rowNr_, colYear)->addWidget(edYear_);
    // classification
   	edClassi_ = new Wt::Ext::LineEdit();
	edClassi_->setText(glider_->classification());
	edClassi_->resize(70, edClassi_->height());
	table_->elementAt(rowNr_, colClassi)->addWidget(edClassi_);
    // description
   	edDesc_ = new Wt::Ext::LineEdit();
	edDesc_->setText(glider_->description());
	table_->elementAt(rowNr_, colDesc)->addWidget(edDesc_);
	// the glider image
	if(glider_->image().length())
	{
        img_ = new Wt::WImage(glider_->image());
        table_->elementAt(rowNr_, colImg)->addWidget(wiSave);
	}
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderTableRow::save()
{
	// save the information in the edit fields back to the internal representation
    try
    {
        // first make sure there's no other glider in the db with the same specs
        //...
        // brand
        assert(edBrand_);
        glider_->setBrand(edBrand_->text().narrow());
        // model
        assert(edModel_);
        glider_->setModel(edModel_->text().narrow());
        // size
        assert(edSize_);
        glider_->setSize(edSize_->text().narrow());
        // color
        assert(edColor_);
        glider_->setColor(edColor_->text().narrow());
        // year
        assert(edYear_);
        glider_->setYear(edYear_->value());
        // classi
        assert(edClassi_);
        glider_->setClassification(edClassi_->text().narrow());
        // desc
        assert(edDesc_);
        glider_->setDescription(edDesc_->text().narrow());

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
void GliderTableRow::remove()
{
    try
    {
        flightDb_->deleteGlider(glider_);
        clearRow();
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Ok, true);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
GliderTable::GliderTable(boost::shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : DetailTableBase(flightDb, parent)
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderTable::createHeaderRow()
{
    static const std::string captions[8] = {"Marke",   "Typ",    "Groesse",      "Farbe",
                                            "Baujahr", "Klasse", "Beschreibung", "Bild"};
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
void GliderTable::createFooterRow()
{
    Wt::WImage *wiAdd = new Wt::WImage("img/add.png");
    wiAdd->setAlternateText("add new glider");
    wiAdd->setToolTip("Schirm hinzufuegen");
	elementAt(insertRowNr_, 0)->addWidget(wiAdd);
	wiAdd->clicked().connect(SLOT(this, GliderTable::addNewGlider));
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
GliderTableRow * GliderTable::addGlider(boost::shared_ptr<Glider> gld, size_t row, bool newEntry)
{
	GliderTableRow *gldrw = new GliderTableRow(gld, this, flightDb_, row, newEntry);
	gldrw->show();
	rows_.push_back(gldrw);
	return gldrw;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderTable::addNewGlider()
{
    // create the new glider
    boost::shared_ptr<Glider> newGlider(new Glider("",                                              // brand
                                            "",                                              // model
                                            "",                                              // size
                                            "",                                              // color
                                            boost::gregorian::day_clock::local_day().year(), // year
                                            "CEN A"));                                       // classification
    flightDb_->addGlider(newGlider);
    GliderTableRow *newRow = addGlider(newGlider, insertRowNr_++, true);
    newRow->edit();
    createFooterRow();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderTable::filter(const std::string &brand, const std::string &classification)
{
    gliders_.clear();

    if(brand == "alle")
        brng::copy(flightDb_->Gliders, back_inserter(gliders_));
    else
    {
        for(const auto& glider : flightDb_->Gliders)
            if(glider->brand() == brand)
                gliders_.push_back(glider);
    }
    if(classification == "alle")
        return;
    auto classificationFilter = [&classification](boost::shared_ptr<Glider>& glider)
        { return glider->classification() != classification; };
    gliders_.erase(brng::remove_if(gliders_, classificationFilter), gliders_.end());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderTable::loadPage(unsigned int page)
{
    pageNr_ = page;

    clear();
    createHeaderRow();
    const size_t nFirst = (pageNr_ - 1) * entriesPerPage_;
    if(nFirst < gliders_.size())
    {
        auto ibeg = gliders_.begin();
        std::advance(ibeg, (pageNr_ - 1) * entriesPerPage_);
        auto iend = ibeg;
        const int nLast = std::min<int>(gliders_.size(), nFirst + entriesPerPage_);
        std::advance(iend, nLast - nFirst);
        for(size_t i=1; ibeg != iend; ++ibeg, ++i)
            addGlider(*ibeg, i);
        insertRowNr_ = nLast - nFirst + 1;
    }
    else
        insertRowNr_ = gliders_.size() + 1;
    createFooterRow();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
GliderPanel::GliderPanel(boost::shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), impl_(new Wt::WContainerWidget())
{
    setImplementation(impl_);
    setStyleClass("flb_detail_panel");
    impl_->setStyleClass("flb_detail_panel");

    cbBrand_  = new Wt::Ext::ComboBox(impl_);
    cbClassi_ = new Wt::Ext::ComboBox(impl_);
    table_  = new GliderTable(flightDb, impl_);
    pglist_ = new PagesList(table_);
    // signals
    cbBrand_->activated().connect(SLOT(this, GliderPanel::filter));
    cbClassi_->activated().connect(SLOT(this, GliderPanel::filter));
    // header
    Wt::WTable *topBar = new Wt::WTable();
    topBar->setStyleClass("FilterBar");
    Wt::WText *wtFilt = new Wt::WText("Filter : ");
    wtFilt->setStyleClass("FilterTitle");
    Wt::WText *wtBrand = new Wt::WText("Hersteller");
    wtBrand->setStyleClass("FilterSubTitle");
    Wt::WText *wtClassi = new Wt::WText("Klassierung");
    wtClassi->setStyleClass("FilterSubTitle");
    topBar->elementAt(0, 0)->addWidget(wtFilt);
    topBar->elementAt(0, 1)->addWidget(wtBrand);
    topBar->elementAt(0, 2)->addWidget(cbBrand_);
    topBar->elementAt(0, 3)->addWidget(wtClassi);
    topBar->elementAt(0, 4)->addWidget(cbClassi_);
    topBar->elementAt(0, 5)->addWidget(pglist_);

    Wt::WBorderLayout *borderLayout = new Wt::WBorderLayout();
    impl_->setLayout(borderLayout);
    borderLayout->addWidget(topBar, Wt::WBorderLayout::North);
    borderLayout->addWidget(table_, Wt::WBorderLayout::Center);

    load();
    filter();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderPanel::load()
{
    cbBrand_->clear();
    cbBrand_->addItem("alle");
    for(const auto& glider : flightDb_->Gliders)
        cbBrand_->addItem(glider->brand());
    cbBrand_->setCurrentIndex(0);

    cbClassi_->clear();
    cbClassi_->addItem("alle");
    for(const auto& glider : flightDb_->Gliders)
        cbClassi_->addItem(glider->classification());
    cbClassi_->setCurrentIndex(0);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void GliderPanel::filter()
{
    const std::string brand  = cbBrand_->currentText().narrow();
    const std::string classi = cbClassi_->currentText().narrow();

    table_->filter(brand, classi);
    table_->loadPage(1);
    pglist_->load();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

