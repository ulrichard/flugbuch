// flugbuch
#include "FlightlessTime.h"
#include "FormatStr.h"
// witty
#include <Wt/WStandardItemModel>
#include <Wt/Chart/WCartesianChart>
#include <Wt/Chart/WPieChart>
// boost
#include <boost/foreach.hpp>
// standard library
#include <algorithm>

using namespace flbwt;
using Wt::WStandardItemModel;
using Wt::Chart::WCartesianChart;
using Wt::Chart::WPieChart;
using boost::gregorian::date;
using boost::shared_ptr;
using boost::any;
using std::string;
using std::map;
using std::vector;
using std::pair;
using std::make_pair;
using std::auto_ptr;
using std::max;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
auto_ptr<WStandardItemModel> FlightlessTime::model(const flb::FlightDatabase::SeqFlights &flights) const
{
    assert(flights.size());
    const date firstDay = (*flights.begin())->date();
    const date lastDay  = (*flights.rbegin())->date();

    map<string, int> counts;       // flights per week
    map<int, int> flightlessWeeks; // flightless weeks per year
    map<int, int> longestGap;      // in days - per year

    const date lastWeek = date(lastDay.year(), lastDay.month(), 1);
    for(date ww = date(firstDay.year(), firstDay.month(), 1); ww < lastWeek; ww += boost::gregorian::weeks(1))
        counts[FormatStr() << ww.year() << "." << ww.week_number()] = 0;

    for(flb::FlightDatabase::SeqFlights::iterator it = flights.begin(), itn = it; it != flights.end(); ++it, itn = it)
    {
        const int year = (*it)->date().year();
        const string key(FormatStr() << year << "." << (*it)->date().week_number());
        counts[key]++;
        if(++itn != flights.end())
            longestGap[year] = max<int>(longestGap[year], ((*itn)->date() - (*it)->date()).days());
    }

    for(int yy = firstDay.year(); yy <= lastDay.year(); ++yy)
    {
        const string syy = FormatStr() << yy;
//        flightlessWeeks[yy] = count_if(counts.begin(), counts.end(),
//            (bind(&string::substr, bind(&pair<string, int>::first, boost::lambda::_1), 0, 4) == syy) &&
//            (bind(&pair<string, int>::second, boost::lambda::_1) == constant(0)));

        int flcount = 0;
        for(map<string, int>::iterator it = counts.begin(); it != counts.end(); ++it)
            if(it->first.substr(0, 4) == syy && it->second == 0)
                ++flcount;
        flightlessWeeks[yy] = flcount;
    }

    auto_ptr<WStandardItemModel> model(new  Wt::WStandardItemModel(flightlessWeeks.size(), 3));

    int i = 0;
    for(map<int, int>::iterator it=flightlessWeeks.begin(); it!=flightlessWeeks.end(); ++it, ++i)
    {
        model->setData(i, 0, any(it->first));
        model->setData(i, 1, any(it->second));
        model->setData(i, 2, any(longestGap[it->first]));
    }

    return model;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightlessTime::draw(Wt::WContainerWidget *parent, std::auto_ptr<Wt::WStandardItemModel> model) const
{
    WCartesianChart *cartchart = new WCartesianChart(parent);

    cartchart->setModel(model.release());
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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
auto_ptr<WStandardItemModel> FlightsPerGlider::model(const flb::FlightDatabase::SeqFlights &flights) const
{
    auto_ptr<WStandardItemModel> model(new WStandardItemModel(flightDb_->gliders().size(), 3));

    int i = 0;
    for(flb::FlightDatabase::SeqGliders::iterator it = flightDb_->gliders().begin(); it != flightDb_->gliders().end(); ++it, ++i)
    {
        string nam = (*it)->identity();
/*
        int    cnt = std::count_if(flightDb_->flights().begin(), flightDb_->flights().end(),
            *it == boost::bind(&flb::Flight::glider, ::_1));
*/
        int cnt = 0, dur = 0;
        for(flb::FlightDatabase::SeqFlights::iterator itf = flights.begin(); itf != flights.end(); ++itf)
            if((*itf)->glider() == *it)
            {
                cnt++;
                dur += (*itf)->duration();
            }

        model->setData(i, 0, any(nam));
        model->setData(i, 1, any(cnt));
        model->setData(i, 2, any(dur));
    }

    return model;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightsPerGlider::draw(Wt::WContainerWidget *parent, std::auto_ptr<Wt::WStandardItemModel> model) const
{
    WPieChart *pie = new WPieChart(parent);
    pie->setModel(model.release());
    pie->setLabelsColumn(0);
    pie->setDataColumn(/*airtime ? 2 :*/ 1);
    pie->setDisplayLabels(Wt::Chart::Outside | Wt::Chart::TextLabel | Wt::Chart::TextPercentage);
    pie->setPerspectiveEnabled(true, 0.3);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
