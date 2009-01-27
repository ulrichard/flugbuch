// flugbuch
#include "StatisticsTab.h"
#include "FormatStr.h"
#include "StatStandard.h"
#include "StatMap.h"
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
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
// standard library
#include <algorithm>
#include <map>
#include <utility>

using std::string;
using std::map;
using std::pair;
using std::make_pair;
using std::for_each;
using std::max;
using std::auto_ptr;
using boost::shared_ptr;
using boost::any;
using boost::gregorian::date;
using boost::ptr_vector;
using namespace boost::lambda;
using namespace flbwt;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
StatisticsPanel::StatisticsPanel(const boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), impl_(new Wt::WContainerWidget())
{
    setImplementation(impl_);

    // add new statistics classes here
    addStatistic(auto_ptr<StatBase>(new FlightsPerGlider(flightDb_)));
    addStatistic(auto_ptr<StatBase>(new FlightsPerArea(flightDb_)));
    addStatistic(auto_ptr<StatBase>(new FlightlessTime(flightDb_)));
    addStatistic(auto_ptr<StatBase>(new FlightsPerPeriod(flightDb_, FLP_YEAR)));
    addStatistic(auto_ptr<StatBase>(new FlightsPerPeriod(flightDb_, FLP_MONTH)));
    addStatistic(auto_ptr<StatBase>(new FlightsPerPeriod(flightDb_, FLP_WEEK)));
    addStatistic(auto_ptr<StatBase>(new StatMap(flightDb_)));

    // header
    Wt::WContainerWidget *topBar = new Wt::WContainerWidget();
    topBar->setLayout(new Wt::WHBoxLayout());
    Wt::WText *wtStat = new Wt::WText("Auswertung");
    wtStat->setStyleClass("FilterSubTitle");

    cbStatSel_ = new Wt::Ext::ComboBox();
    cbStatSel_->activated.connect(SLOT(this, StatisticsPanel::load));

    for(boost::ptr_map<std::string, StatBase>::iterator it = stats_.begin(); it != stats_.end(); ++it)
        cbStatSel_->addItem(it->first);
    //for_each(stats_.begin(), stats_.end(), bind(&Wt::Ext::ComboBox::addItem, cbStatSel_, bind(pair<string, StatBase*>::first)));

    topBar->layout()->addWidget(wtStat);
    topBar->layout()->addWidget(cbStatSel_);
    topBar->resize(topBar->width(), 40);
    cbStatSel_->resize(150, cbStatSel_->height());

    // body
    report_ = new Wt::WContainerWidget();

    Wt::WBorderLayout *blayout = new Wt::WBorderLayout();
    impl_->setLayout(blayout);
    blayout->addWidget(topBar, Wt::WBorderLayout::North);
    blayout->addWidget(report_, Wt::WBorderLayout::Center);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatisticsPanel::load(int ind)
{
    report_->clear();

    const string statname = cbStatSel_->text().narrow();
    boost::ptr_map<std::string, StatBase>::const_iterator fit = stats_.find(statname);
    if(fit != stats_.end())
        fit->second->draw(report_, flightDb_->flights());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
