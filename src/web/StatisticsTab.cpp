// flugbuch
#include "StatisticsTab.h"
#include "FormatStr.h"
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
// standard library
#include <algorithm>
#include <map>
#include <utility>

using std::string;
using std::map;
using std::pair;
using std::make_pair;
using std::for_each;
using boost::shared_ptr;
using boost::any;
using boost::gregorian::date;
using namespace boost::lambda;
using namespace flbwt;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
StatisticsPanel::StatisticsPanel(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void StatisticsPanel::load(int ind)
{
    switch(ind)
    {
    case 0:
        FlightsPerGlider(false);
        break;
    case 1:
        FlightsPerGlider(true);
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
        FlightAreas(false);
        break;
    case 6:
        FlightAreas(true);
        break;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void StatisticsPanel::FlightsPerGlider(bool airtime)
{
    blayout_->removeWidget(chart_);
 //   delete chart_;
    Wt::Chart::WPieChart *pie = new Wt::Chart::WPieChart();
    blayout_->addWidget(pie, Wt::WBorderLayout::Center);
    chart_ = pie;

    Wt::WStandardItemModel *model = new  Wt::WStandardItemModel(flightDb_->gliders().size(), 3);


    int i = 0;
    for(flb::FlightDatabase::SeqGliders::iterator it = flightDb_->gliders().begin(); it != flightDb_->gliders().end(); ++it, ++i)
    {
        string nam = (*it)->identity();
/*
        int    cnt = std::count_if(flightDb_->flights().begin(), flightDb_->flights().end(),
            *it == boost::bind(&flb::Flight::glider, ::_1));
*/
        int cnt = 0, dur = 0;
        for(flb::FlightDatabase::SeqFlights::iterator itf = flightDb_->flights().begin(); itf != flightDb_->flights().end(); ++itf)
            if((*itf)->glider() == *it)
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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void StatisticsPanel::FlightsPerTimePeriod(int mode)
{
    blayout_->removeWidget(chart_);
 //   delete chart_;
    Wt::Chart::WCartesianChart *cartchart = new Wt::Chart::WCartesianChart();
    blayout_->addWidget(cartchart, Wt::WBorderLayout::Center);
    chart_ = cartchart;

    assert(flightDb_->flights().size());
    const date firstDay = (*flightDb_->flights().begin())->date();
    const date lastDay  = (*flightDb_->flights().rbegin())->date();

    map<string, pair<int, int> > counts;

    if(mode == 0)
    {
        for(int yy = firstDay.year(); yy <= lastDay.year(); ++yy)
            counts[FormatStr() << yy] = make_pair(0, 0);
    }
    else if(mode == 1)
    {
        const date lastMonth = date(lastDay.year(), lastDay.month(), 1);
        for(date mm = date(firstDay.year(), firstDay.month(), 1); mm < lastMonth; mm += boost::gregorian::months(1))
            counts[FormatStr() << mm.year() << "." << mm.month()] = make_pair(0, 0);
    }
    else if(mode == 2)
    {
        const date lastWeek = date(lastDay.year(), lastDay.month(), 1);
        for(date ww = date(firstDay.year(), firstDay.month(), 1); ww < lastWeek; ww += boost::gregorian::weeks(1))
            counts[FormatStr() << ww.year() << "." << ww.week_number()] = make_pair(0, 0);
    }

    BOOST_FOREACH(shared_ptr<flb::Flight> fl, flightDb_->flights())
    {
        if(mode == 0)
        {
            const string key(FormatStr() << fl->date().year());
            counts[key].first++;
            counts[key].second += fl->duration();
        }
        else if(mode == 1)
        {
            const string key(FormatStr() << fl->date().year() << "." << fl->date().month());
            counts[key].first++;
            counts[key].second += fl->duration();
        }
        else if(mode == 2)
        {
            const string key(FormatStr() << fl->date().year() << "." << fl->date().week_number());
            counts[key].first++;
            counts[key].second += fl->duration();
        }
    }

    Wt::WStandardItemModel *model = new  Wt::WStandardItemModel(counts.size(), 3);

    int i = 0;
    for(map<string, pair<int, int> >::iterator it=counts.begin(); it!=counts.end(); ++it, ++i)
    {
        model->setData(i, 0, any(it->first));
        model->setData(i, 1, any(it->second.first));
        model->setData(i, 2, any(it->second.second));
    }


    cartchart->setModel(model);
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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
