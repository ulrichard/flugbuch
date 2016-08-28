// flugbuch
#include "StatisticsTab.h"
#include "FormatStr.h"
#include "StatStandard.h"
#include "StatMap.h"
#include "StatFlightReport.h"
// witty
#include <Wt/Ext/ComboBox>
#include <Wt/WContainerWidget>
#include <Wt/Chart/WPieChart>
#include <Wt/Chart/WCartesianChart>
#include <Wt/Chart/WDataSeries>
#include <Wt/WHBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WText>
#include <Wt/WStandardItemModel>
#include <Wt/WSelectionBox>
#include <Wt/WTable>
// boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
// standard library
#include <map>
#include <utility>

using namespace flbwt;
namespace brng = boost::range;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
StatisticsPanel::StatisticsPanel(const boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), impl_(new Wt::WContainerWidget())
{
    setImplementation(impl_);
    impl_->setStyleClass("flb_detail_panel");

    // add new statistics classes here
    addStatistic(std::unique_ptr<StatBase>(new FlightsPerGlider(flightDb_)));
    addStatistic(std::unique_ptr<StatBase>(new FlightsPerArea(flightDb_)));
    addStatistic(std::unique_ptr<StatBase>(new FlightlessTime(flightDb_)));
    addStatistic(std::unique_ptr<StatBase>(new FlightsPerPeriod(flightDb_, FLP_YEAR)));
    addStatistic(std::unique_ptr<StatBase>(new FlightsPerPeriod(flightDb_, FLP_MONTH)));
    addStatistic(std::unique_ptr<StatBase>(new FlightsPerPeriod(flightDb_, FLP_WEEK)));
    addStatistic(std::unique_ptr<StatBase>(new StatMap(flightDb_)));
    addStatistic(std::unique_ptr<StatBase>(new StatFlightReport(flightDb_)));
    addStatistic(std::unique_ptr<StatBase>(new StatGeneralOverview(flightDb_)));

    // header
    Wt::WTable *topBar = new Wt::WTable();
    topBar->setStyleClass("FilterBar");
    // statistic type
    Wt::WText *wtStat = new Wt::WText("Auswertung");
    wtStat->setStyleClass("FilterSubTitle");
    cbStatSel_ = new Wt::Ext::ComboBox();
    cbStatSel_->resize(150, cbStatSel_->height());
    cbStatSel_->activated().connect(SLOT(this, StatisticsPanel::load));
    for(auto it = stats_.begin(); it != stats_.end(); ++it)
        cbStatSel_->addItem(it->first);
//    for_each(stats_.begin(), stats_.end(), bind(&Wt::Ext::ComboBox::addItem, cbStatSel_, bind(&pair<string, StatBase*>::first, ::_1)));
    topBar->elementAt(0, 0)->addWidget(wtStat);
    topBar->elementAt(0, 0)->addWidget(cbStatSel_);
    // countries
    Wt::WText *wtCountry = new Wt::WText("Land");
    wtCountry->setStyleClass("FilterSubTitle");
    sbCountry_ = new Wt::WSelectionBox();
    sbCountry_->setSelectionMode(Wt::ExtendedSelection);
    std::set<std::string> countries;
    brng::transform(flightDb_->FlightAreas, inserter(countries, countries.end()),
        [](const boost::shared_ptr<flb::FlightArea>& area){ return area->country(); });
    for(std::string str : countries)
        sbCountry_->addItem(str);
    std::set<int> selind;
    for(size_t i=0; i<countries.size(); ++i)
        selind.insert(i);
    sbCountry_->setSelectedIndexes(selind);
    sbCountry_->setVerticalSize(5);
    sbCountry_->clicked().connect(SLOT(this, StatisticsPanel::load));
    topBar->elementAt(0, 1)->addWidget(wtCountry);
    topBar->elementAt(0, 2)->addWidget(sbCountry_);
    // years
    Wt::WText *wtYear = new Wt::WText("Jahr");
    wtYear->setStyleClass("FilterSubTitle");
    sbYear_ = new Wt::WSelectionBox();
    sbYear_->setSelectionMode(Wt::ExtendedSelection);
    std::set<int> years;
    for(auto& flt : flightDb_->flights())
        years.insert(flt->date().year());
    for(int yyy : years)
        sbYear_->addItem(boost::lexical_cast<std::string>(yyy));
    selind.clear();
    for(size_t i=0; i<years.size(); ++i)
        selind.insert(i);
    sbYear_->setSelectedIndexes(selind);
    sbYear_->setVerticalSize(5);
    sbYear_->clicked().connect(SLOT(this, StatisticsPanel::load));
    topBar->elementAt(0, 3)->addWidget(wtYear);
    topBar->elementAt(0, 4)->addWidget(sbYear_);
    // glider class
    Wt::WText *wtClassi = new Wt::WText("Schirmklasse");
    wtClassi->setStyleClass("FilterSubTitle");
    sbClassi_ = new Wt::WSelectionBox();
    sbClassi_->setSelectionMode(Wt::ExtendedSelection);
    std::set<std::string> classes;
    brng::transform(flightDb_->Gliders, inserter(classes, classes.end()),
        [](const boost::shared_ptr<flb::Glider>& glider){ return glider->classification(); });
    for(const auto& str : classes)
        sbClassi_->addItem(str);
    selind.clear();
    for(size_t i=0; i<classes.size(); ++i)
        selind.insert(i);
    sbClassi_->setSelectedIndexes(selind);
    sbClassi_->setVerticalSize(5);
    sbClassi_->clicked().connect(SLOT(this, StatisticsPanel::load));
    topBar->elementAt(0, 5)->addWidget(wtClassi);
    topBar->elementAt(0, 6)->addWidget(sbClassi_);

    Wt::WBorderLayout *blayout = new Wt::WBorderLayout();
    impl_->setLayout(blayout);
    blayout->addWidget(topBar, Wt::WBorderLayout::North);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatisticsPanel::load()
{
    Wt::WBorderLayout *blayout = dynamic_cast<Wt::WBorderLayout*>(impl_->layout());
    assert(blayout);
    const std::string statname = cbStatSel_->text().narrow();
    static std::string laststatname = "";
    Wt::WContainerWidget *cont = dynamic_cast<Wt::WContainerWidget*>(blayout->widgetAt(Wt::WBorderLayout::Center));
    if(statname != laststatname && cont)
    {
        blayout->removeWidget(cont);
        cont = NULL;
    }
    laststatname = statname;
    if(!cont)
    {
        cont = new Wt::WContainerWidget();
        blayout->addWidget(cont, Wt::WBorderLayout::Center);
    }

    initFilter();
    std::set<boost::shared_ptr<flb::Flight> > flights;
    std::remove_copy_if(flightDb_->flights().begin(), flightDb_->flights().end(), inserter(flights, flights.end()),
        [this](const boost::shared_ptr<flb::Flight>& flight){ return filter(*flight); });

    if(statname.length())
    {
        boost::ptr_map<std::string, StatBase>::const_iterator fit = stats_.find(statname);
        if(fit != stats_.end())
            fit->second->draw(cont, flights);
        else
            throw std::runtime_error("StatisticsPanel::load()  statistic not found : " + statname);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatisticsPanel::initFilter()
{
    filtCountries_.clear();
    const std::set<int> &countrysel = sbCountry_->selectedIndexes();
//    transform(countrysel.begin(), countrysel.end(), inserter(filtCountries_, filtCountries_.end()),
//        bind(&Wt::WString::narrow, bind(&Wt::WSelectionBox::itemText, sbCountry_, ::_1)));
    for(int cn : countrysel)
        filtCountries_.insert(sbCountry_->itemText(cn).narrow());

    filtYears_.clear();
    for(int idx : sbYear_->selectedIndexes())
        filtYears_.insert(boost::lexical_cast<int>(sbYear_->itemText(idx).narrow()));

    filtClassi_.clear();
    const std::set<int> &classisel = sbClassi_->selectedIndexes();
//    transform(classisel.begin(), classisel.end(), inserter(filtClassi_, filtClassi_.end()),
//        bind(&Wt::WString::narrow, bind(&Wt::WSelectionBox::itemText, sbClassi_, ::_1)));
    for(int csn : classisel)
        filtClassi_.insert(sbClassi_->itemText(csn).narrow());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bool StatisticsPanel::filter(const flb::Flight &fl) const
{
    if(filtCountries_.find(fl.takeoff()->area()->country()) == filtCountries_.end())
        return false;

    if(filtYears_.find(fl.date().year()) == filtYears_.end())
        return false;

    if(filtClassi_.find(fl.glider()->classification()) == filtClassi_.end())
        return false;

    return true;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

