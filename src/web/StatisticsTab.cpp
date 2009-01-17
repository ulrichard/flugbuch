// flugbuch
#include "StatisticsTab.h"
#include "FormatStr.h"
#include "FlightlessTime.h"
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
    chart_= NULL;

    // header
    Wt::WContainerWidget *topBar = new Wt::WContainerWidget();
    topBar->setLayout(new Wt::WHBoxLayout());
    Wt::WText *wtStat = new Wt::WText("Auswertung");
    wtStat->setStyleClass("FilterSubTitle");

    cbStatSel_ = new Wt::Ext::ComboBox();
    cbStatSel_->activated.connect(SLOT(this, StatisticsPanel::load));

    cbStatSel_->addItem("Fluege pro Schirm");
    cbStatSel_->addItem("Flugzeit pro Schirm");
    cbStatSel_->addItem("Fluege pro Jahr");
    cbStatSel_->addItem("Fluege pro Monat");
    cbStatSel_->addItem("Fluege pro Woche");
    cbStatSel_->addItem("Fluglose Zeit pro Jahr");
    cbStatSel_->addItem("Fluege pro Fluggebiet");
    cbStatSel_->addItem("Flugzeit pro Fluggebiet");

    topBar->layout()->addWidget(wtStat);
    topBar->layout()->addWidget(cbStatSel_);
    topBar->resize(topBar->width(), 40);
    cbStatSel_->resize(150, cbStatSel_->height());

    blayout_ = new Wt::WBorderLayout();
    impl_->setLayout(blayout_);
    blayout_->addWidget(topBar, Wt::WBorderLayout::North);


}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatisticsPanel::load(int ind)
{
    switch(ind)
    {
    case 0:
        FlightsPerGliderF(false);
        break;
    case 1:
        FlightsPerGliderF(true);
        break;
    case 2:
        FlightsPerTimePeriod(0);
        break;
    case 3:
        FlightsPerTimePeriod(1);
        break;
    case 4:
        FlightsPerTimePeriod(2);
        break;
    case 5:
        FlightlessTimeStat();
        break;
    case 6:
        FlightAreas(false);
        break;
    case 7:
        FlightAreas(true);
        break;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatisticsPanel::FlightsPerGliderF(bool airtime)
{
    blayout_->removeWidget(chart_);
 //   delete chart_;
    Wt::Chart::WPieChart *pie = new Wt::Chart::WPieChart();
    blayout_->addWidget(pie, Wt::WBorderLayout::Center);
    chart_ = pie;

    FlightsPerGlider flg(flightDb_);

    pie->setModel(flg.model(flightDb_->flights()).release());
    pie->setLabelsColumn(0);
    pie->setDataColumn(airtime ? 2 : 1);
    pie->setDisplayLabels(Wt::Chart::Outside | Wt::Chart::TextLabel | Wt::Chart::TextPercentage);
    pie->setPerspectiveEnabled(true, 0.3);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatisticsPanel::FlightsPerTimePeriod(int mode)
{
    blayout_->removeWidget(chart_);
 //   delete chart_;
    Wt::Chart::WCartesianChart *cartchart = new Wt::Chart::WCartesianChart();
    blayout_->addWidget(cartchart, Wt::WBorderLayout::Center);
    chart_ = cartchart;

    ptr_vector<StatBase> flp;
    flp.push_back(new FlightsPerPeriod<FLP_YEAR>(flightDb_));
    flp.push_back(new FlightsPerPeriod<FLP_MONTH>(flightDb_));
    flp.push_back(new FlightsPerPeriod<FLP_WEEK>(flightDb_));

    cartchart->setModel(flp[mode].model(flightDb_->flights()).release());
    cartchart->setXSeriesColumn(0);
    Wt::Chart::WDataSeries data1(Wt::Chart::WDataSeries(1, Wt::Chart::LineSeries, Wt::Chart::Y1Axis));
    Wt::Chart::WDataSeries data2(Wt::Chart::WDataSeries(2, Wt::Chart::LineSeries, Wt::Chart::Y2Axis));
    data1.setLegendEnabled(true);
    data2.setLegendEnabled(true);
    cartchart->addSeries(data1);
    cartchart->addSeries(data2);
    cartchart->axis(Wt::Chart::Y2Axis).setVisible(true);
    cartchart->setLegendEnabled(true);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatisticsPanel::FlightlessTimeStat()
{
    blayout_->removeWidget(chart_);
 //   delete chart_;
    Wt::Chart::WCartesianChart *cartchart = new Wt::Chart::WCartesianChart();
    blayout_->addWidget(cartchart, Wt::WBorderLayout::Center);
    chart_ = cartchart;

    FlightlessTime flt(flightDb_);

    cartchart->setModel(flt.model(flightDb_->flights()).release());
    cartchart->setXSeriesColumn(0);
    Wt::Chart::WDataSeries data1(Wt::Chart::WDataSeries(1, Wt::Chart::LineSeries, Wt::Chart::Y1Axis));
    Wt::Chart::WDataSeries data2(Wt::Chart::WDataSeries(2, Wt::Chart::LineSeries, Wt::Chart::Y2Axis));
    data1.setLegendEnabled(true);
    data2.setLegendEnabled(true);
    cartchart->addSeries(data1);
    cartchart->addSeries(data2);
    cartchart->axis(Wt::Chart::Y2Axis).setVisible(true);
    cartchart->setLegendEnabled(true);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatisticsPanel::FlightAreas(bool airtime)
{
    blayout_->removeWidget(chart_);
//   delete chart_;
    Wt::Chart::WPieChart *pie = new Wt::Chart::WPieChart();
    blayout_->addWidget(pie, Wt::WBorderLayout::Center);
    chart_ = pie;

    Wt::WStandardItemModel *model = new  Wt::WStandardItemModel(flightDb_->flightAreas().size(), 3);


    int i = 0;
    for(flb::FlightDatabase::SeqFlightAreas::iterator it = flightDb_->flightAreas().begin(); it != flightDb_->flightAreas().end(); ++it, ++i)
    {
        string nam = (*it)->name();
        int cnt = 0, dur = 0;
        for(flb::FlightDatabase::SeqFlights::iterator itf = flightDb_->flights().begin(); itf != flightDb_->flights().end(); ++itf)
            if((*itf)->takeoff()->area() == *it)
            {
                cnt++;
                dur += (*itf)->duration();
            }

        model->setData(i, 0, any(nam));
        model->setData(i, 1, any(cnt));
        model->setData(i, 2, any(dur));
    }

    pie->setModel(model);
    pie->setLabelsColumn(0);
    pie->setDataColumn(airtime ? 2 : 1);
    pie->setDisplayLabels(Wt::Chart::Outside | Wt::Chart::TextLabel | Wt::Chart::TextPercentage);
    pie->setPerspectiveEnabled(true, 0.3);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
