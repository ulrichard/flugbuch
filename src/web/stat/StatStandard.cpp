// flugbuch
#include "StatStandard.h"
#include "FormatStr.h"
// witty
#include <Wt/WStandardItemModel>
#include <Wt/Chart/WCartesianChart>
#include <Wt/Chart/WPieChart>
#include <Wt/Ext/TableView>
#include <Wt/WTable>
#include <Wt/WText>
// boost
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <boost/array.hpp>
#include <boost/ref.hpp>
// standard library
#include <algorithm>
#include <iterator>
#include <limits>
#include <numeric>
#include <iomanip>

using namespace flbwt;
using Wt::WStandardItemModel;
using Wt::WContainerWidget;
using Wt::Chart::WCartesianChart;
using Wt::Chart::WPieChart;
using Wt::Ext::TableView;
using boost::gregorian::date;
using boost::gregorian::months;
using boost::gregorian::weeks;
using boost::shared_ptr;
using boost::any;
using boost::lexical_cast;
using boost::function;
using boost::array;
using boost::ref;
using namespace boost::lambda;
namespace bll = boost::lambda;
using std::string;
using std::map;
using std::vector;
using std::pair;
using std::make_pair;
using std::auto_ptr;
using std::max;
using std::numeric_limits;
using std::min;
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

    for(flb::FlightDatabase::SeqFlights::const_iterator it = flights.begin(), itn = it; it != flights.end(); ++it, itn = it)
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

    auto_ptr<WStandardItemModel> model(new WStandardItemModel(flightlessWeeks.size(), 3));
    model->setHeaderData(0, Wt::Horizontal, any(string("Jahr")));
    model->setHeaderData(1, Wt::Horizontal, any(string("Fluglose Wochen")));
    model->setHeaderData(2, Wt::Horizontal, any(string("Laengste Pause [Tage]")));

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
void FlightlessTime::draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const
{
    parent->clear();
    Wt::WStandardItemModel *modelrel = model(flights).release();

    WCartesianChart *cartchart = new WCartesianChart(parent);
    cartchart->resize(800, 500);
    cartchart->setTitle("Fluglose Zeit");
    cartchart->setPlotAreaPadding(200, Wt::Right);
    cartchart->setModel(modelrel);
    cartchart->setXSeriesColumn(0);
    Wt::Chart::WDataSeries data1(Wt::Chart::WDataSeries(1, Wt::Chart::LineSeries, Wt::Chart::Y1Axis));
    Wt::Chart::WDataSeries data2(Wt::Chart::WDataSeries(2, Wt::Chart::LineSeries, Wt::Chart::Y2Axis));
    data1.setLegendEnabled(true);
    data2.setLegendEnabled(true);
    cartchart->addSeries(data1);
    cartchart->addSeries(data2);
    cartchart->axis(Wt::Chart::Y2Axis).setVisible(true);
    cartchart->setLegendEnabled(true);

    TableView *tabview = new TableView(parent);
    tabview->resize(800, 300);
    tabview->setModel(modelrel);
    tabview->setColumnSortable(0, true);
    tabview->setColumnSortable(1, true);
    tabview->setColumnSortable(2, true);
//    tabview->setRenderer(2, "function change(val) { return (val / 60.0;) + 'h' }");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
auto_ptr<WStandardItemModel> FlightsPerGlider::model(const flb::FlightDatabase::SeqFlights &flights) const
{
    auto_ptr<WStandardItemModel> model(new WStandardItemModel(flightDb_->Gliders.size(), 3));
    model->setHeaderData(0, Wt::Horizontal, any(string("Gleitschirm")));
    model->setHeaderData(1, Wt::Horizontal, any(string("Fluege")));
    model->setHeaderData(2, Wt::Horizontal, any(string("Flugzeit")));


    int i = 0;
    for(flb::FlightDatabase::Gliders::iterator it = flightDb_->Gliders.begin(); it != flightDb_->Gliders.end(); ++it, ++i)
    {
        string nam = (*it)->identity();
/*
        int    cnt = std::count_if(flightDb_->flights().begin(), flightDb_->flights().end(),
            *it == boost::bind(&flb::Flight::glider, ::_1));
*/
        int cnt = 0;
        boost::posix_time::time_duration dur(0, 0, 0);

        for(flb::FlightDatabase::SeqFlights::const_iterator itf = flights.begin(); itf != flights.end(); ++itf)
            if((*itf)->glider() == *it)
            {
                cnt++;
                dur += (*itf)->duration();
            }

        model->setData(i, 0, any(nam));
        model->setData(i, 1, any(cnt));
        model->setData(i, 2, any(dur.total_seconds() / 3600.0));
    }

    return model;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightsPerGlider::draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const
{
    parent->clear();
    Wt::WStandardItemModel *modelrel = model(flights).release();

    WPieChart *pie1 = new WPieChart(parent);
    pie1->setTitle("anz Fluege");
    pie1->setModel(modelrel);
    pie1->setLabelsColumn(0);
    pie1->setDataColumn(1);
    pie1->setDisplayLabels(Wt::Chart::Outside | Wt::Chart::TextLabel | Wt::Chart::TextPercentage);
    pie1->setPerspectiveEnabled(true, 0.3);

    WPieChart *pie2 = new WPieChart(parent);
    pie2->setTitle("Flugzeit");
    pie2->setModel(modelrel);
    pie2->setLabelsColumn(0);
    pie2->setDataColumn(2);
    pie2->setDisplayLabels(Wt::Chart::Outside | Wt::Chart::TextLabel | Wt::Chart::TextPercentage);
    pie2->setPerspectiveEnabled(true, 0.3);

    TableView *tabview = new TableView(parent);
    tabview->resize(800, 300);
    tabview->setModel(modelrel);
    tabview->setColumnSortable(0, true);
    tabview->setColumnSortable(1, true);
    tabview->setColumnSortable(2, true);
//    tabview->setRenderer(2, "function change(val) { return (val / 60.0;) + 'h' }");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
auto_ptr<WStandardItemModel> FlightsPerPeriod::model(const flb::FlightDatabase::SeqFlights &flights) const
{
    assert(flights.size());
    const date firstDay = (*flights.begin())->date();
    const date lastDay  = (*flights.rbegin())->date();

    map<string, pair<int, boost::posix_time::time_duration> > counts;

    if(interval_ == FLP_YEAR)
    {
        for(int yy = firstDay.year(); yy <= lastDay.year(); ++yy)
            counts[FormatStr() << yy] = make_pair(0, boost::posix_time::time_duration(0, 0, 0));
    }
    else if(interval_ == FLP_MONTH)
    {
        const date lastMonth = date(lastDay.year(), lastDay.month(), 1);
        for(boost::gregorian::date mm = date(firstDay.year(), firstDay.month(), 1); mm < lastMonth; mm += months(1))
            counts[FormatStr() << mm.year() << "." << mm.month()] = make_pair(0, boost::posix_time::time_duration(0, 0, 0));
    }
    else if(interval_ == FLP_WEEK)
    {
        const date lastWeek = date(lastDay.year(), lastDay.month(), 1);
        for(date ww = date(firstDay.year(), firstDay.month(), 1); ww < lastWeek; ww += weeks(1))
            counts[FormatStr() << ww.year() << "." << ww.week_number()] = make_pair(0, boost::posix_time::time_duration(0, 0, 0));
    }
    else
        assert(!"unknown interval");

    BOOST_FOREACH(shared_ptr<flb::Flight> fl, flights)
    {
        if(interval_ == FLP_YEAR)
        {
            const string key(FormatStr() << fl->date().year());
            counts[key].first++;
            counts[key].second += fl->duration();
        }
        else if(interval_ == FLP_MONTH)
        {
            const string key(FormatStr() << fl->date().year() << "." << fl->date().month());
            counts[key].first++;
            counts[key].second += fl->duration();
        }
        else if(interval_ == FLP_WEEK)
        {
            const string key(FormatStr() << fl->date().year() << "." << fl->date().week_number());
            counts[key].first++;
            counts[key].second += fl->duration();
        }
    }

    auto_ptr<WStandardItemModel> model(new WStandardItemModel(counts.size(), 3));
    model->setHeaderData(0, Wt::Horizontal, any(string(std::string(interval_ == FLP_YEAR ? "Jahr" : (interval_ == FLP_MONTH ? "Monat" : "Woche")))));
    model->setHeaderData(1, Wt::Horizontal, any(string("Fluege")));
    model->setHeaderData(2, Wt::Horizontal, any(string("Flugzeit")));

    int i = 0;
    for(map<std::string, pair<int, boost::posix_time::time_duration> >::iterator it=counts.begin(); it!=counts.end(); ++it, ++i)
    {
        model->setData(i, 0, boost::any(it->first));
        model->setData(i, 1, boost::any(it->second.first));
        model->setData(i, 2, boost::any(it->second.second.total_seconds() / 3600.0));
    }

    return model;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightsPerPeriod::draw(WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const
{
    parent->clear();
    Wt::WStandardItemModel *modelrel = model(flights).release();

    WCartesianChart *cartchart = new WCartesianChart(parent);
    cartchart->setModel(modelrel);
    cartchart->setXSeriesColumn(0);
    Wt::Chart::WDataSeries data1(Wt::Chart::WDataSeries(1, Wt::Chart::LineSeries, Wt::Chart::Y1Axis));
    Wt::Chart::WDataSeries data2(Wt::Chart::WDataSeries(2, Wt::Chart::LineSeries, Wt::Chart::Y2Axis));
    data1.setLegendEnabled(true);
    data2.setLegendEnabled(true);
    cartchart->addSeries(data1);
    cartchart->addSeries(data2);
    cartchart->axis(Wt::Chart::Y2Axis).setVisible(true);
    cartchart->setLegendEnabled(true);

    TableView *tabview = new TableView(parent);
    tabview->resize(800, 300);
    tabview->setModel(modelrel);
    tabview->setColumnSortable(0, true);
    tabview->setColumnSortable(1, true);
    tabview->setColumnSortable(2, true);
//    tabview->setRenderer(2, "function change(val) { return (val / 60.0;) + 'h' }");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
auto_ptr<WStandardItemModel> FlightsPerArea::model(const flb::FlightDatabase::SeqFlights &flights) const
{
    typedef pair<int, boost::posix_time::time_duration> PairIntT;
    typedef vector<pair<shared_ptr<flb::FlightArea>, PairIntT> > AreaVectorT;
    AreaVectorT areas;

    for(flb::FlightDatabase::FlightAreas::const_iterator it = flightDb_->FlightAreas.begin(); it != flightDb_->FlightAreas.end(); ++it)
    {
        AreaVectorT::value_type  flarInfo = make_pair(*it, make_pair(0, boost::posix_time::time_duration(0, 0, 0)));

        for(flb::FlightDatabase::SeqFlights::const_iterator itf = flights.begin(); itf != flights.end(); ++itf)
            if((*itf)->takeoff()->area() == *it)
            {
                flarInfo.second.first++;
                flarInfo.second.second += (*itf)->duration();
            }
        areas.push_back(flarInfo);
    }

    // select the area with the most flights
    sort(areas.begin(), areas.end(), bll::bind<int>(&PairIntT::first, bll::bind<PairIntT>(&AreaVectorT::value_type::second, bll::_1)) >
                                     bll::bind<int>(&PairIntT::first, bll::bind<PairIntT>(&AreaVectorT::value_type::second, bll::_2)));
    AreaVectorT::iterator endit = areas.begin();
    std::advance(endit, std::min(areas.size(), numArea_));

    // collect the remaining date in a special entry and then delete them
    AreaVectorT::value_type  flarRest = make_pair(shared_ptr<flb::FlightArea>(new flb::FlightArea("Rest", "", "")),
                                            make_pair(0, boost::posix_time::time_duration(0, 0, 0)));
    for(AreaVectorT::const_iterator it = endit; it != areas.end(); ++it)
    {
        flarRest.second.first  += it->second.first;
        flarRest.second.second += it->second.second;
    }
    areas.erase(endit, areas.end());
    areas.push_back(flarRest);

    // put them into the wt data model
    auto_ptr<WStandardItemModel> model(new WStandardItemModel(areas.size(), 3));
    model->setHeaderData(0, Wt::Horizontal, any(string("Fluggebiet")));
    model->setHeaderData(1, Wt::Horizontal, any(string("Fluege")));
    model->setHeaderData(2, Wt::Horizontal, any(string("Flugzeit")));

    int i=0;
    for(AreaVectorT::const_iterator it = areas.begin(); it != areas.end(); ++it, ++i)
    {
        model->setData(i, 0, any(it->first->name()));
        model->setData(i, 1, any(it->second.first));
        model->setData(i, 2, any(it->second.second.total_seconds() / 3600.0));
    }

    return model;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightsPerArea::draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const
{
    parent->clear();
    Wt::WStandardItemModel *modelrel = model(flights).release();

    WPieChart *pie1 = new WPieChart(parent);
    pie1->setTitle("anz Fluege");
    pie1->setModel(modelrel);
    pie1->setLabelsColumn(0);
    pie1->setDataColumn(1);
    pie1->setDisplayLabels(Wt::Chart::Outside | Wt::Chart::TextLabel | Wt::Chart::TextPercentage);
    pie1->setPerspectiveEnabled(true, 0.3);

    WPieChart *pie2 = new WPieChart(parent);
    pie2->setTitle("Flugzeit");
    pie2->setModel(modelrel);
    pie2->setLabelsColumn(0);
    pie2->setDataColumn(2);
    pie2->setDisplayLabels(Wt::Chart::Outside | Wt::Chart::TextLabel | Wt::Chart::TextPercentage);
    pie2->setPerspectiveEnabled(true, 0.3);

    TableView *tabview = new TableView(parent);
    tabview->resize(800, 300);
    tabview->setModel(modelrel);
    tabview->setColumnSortable(0, true);
    tabview->setColumnSortable(1, true);
    tabview->setColumnSortable(2, true);
 //   tabview->setRenderer(2, "function change(val) { return (val / 60.0;) + 'h'; }");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
template<typename NT>
struct mmas_data
{
    mmas_data(NT min = numeric_limits<NT>::max(), NT max = 0) : cnt_(0), min_(min), max_(max), sum_(0) { }

    double avg() const { return cnt_ ? static_cast<double>(sum_) / cnt_ : 0; }

    typedef NT nt;

    size_t cnt_;
    NT min_, max_, sum_;
};

template<typename NT>
struct mmas_collector
{
    mmas_collector(mmas_data<NT> &mmd, boost::function<NT(const shared_ptr<flb::Flight> &)> getter)
        : mmd_(mmd), getter_(getter) {}

    void operator() (const shared_ptr<flb::Flight> &flt)
    {
        NT currval = getter_(flt);
        ++mmd_.cnt_;
        mmd_.min_  = min(mmd_.min_, currval);
        mmd_.max_  = max(mmd_.max_, currval);
        mmd_.sum_ += currval;
    }

    static void add_line(const size_t row, const string &txt, Wt::WTable *table, bool withSum,
                         const flb::FlightDatabase::SeqFlights &flights,
                         boost::function<NT(const shared_ptr<flb::Flight> &)> getter)
    {
        mmas_data<NT> data;
        std::for_each(flights.begin(), flights.end(), mmas_collector(data, getter));

        // airtime
        array<string, 5> vsText;

        vsText[0] = txt;
        vsText[1] = FormatStr() << std::fixed << std::setprecision(2) << data.min_;
        vsText[2] = FormatStr() << std::fixed << std::setprecision(2) << data.max_;
        vsText[3] = FormatStr() << std::fixed << std::setprecision(2) << data.avg();
        vsText[4] = FormatStr() << std::fixed << std::setprecision(2) << data.sum_;
        for(size_t i=0; i<(withSum ? vsText.size() : vsText.size() - 1); ++i)
        {
            Wt::WText *labelText = new Wt::WText(vsText[i]);
            labelText->setStyleClass("tableContent");
            table->elementAt(row, i)->addWidget(labelText);
        }
    }

    mmas_data<NT> &mmd_;
    boost::function<NT(const shared_ptr<flb::Flight> &)> getter_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatGeneralOverview::draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const
{
    parent->clear();
    Wt::WTable *table = new Wt::WTable(parent);
    table->setStyleClass("tableFlights");

    array<string, 5> captions = {"What", "Min", "Max", "Avg", "Sum"};

    for(size_t i=0; i<captions.size(); ++i)
    {
        Wt::WText *labelText = new Wt::WText(captions[i]);
        labelText->setStyleClass("tableHeader");
        table->elementAt(0, i)->addWidget(labelText);
    }
    table->rowAt(0)->setStyleClass("title");


    Wt::WText *labelText = new Wt::WText("Flights");
    labelText->setStyleClass("tableContent");
    table->elementAt(1, 0)->addWidget(labelText);
    labelText = new Wt::WText(lexical_cast<string>(std::distance(flights.begin(), flights.end())));
    labelText->setStyleClass("tableContent");
    table->elementAt(1, 4)->addWidget(labelText);
    mmas_collector<double>::add_line(2, "AirTime [h]",         table, true,  flights,
                bll::bind(&boost::posix_time::time_duration::hours, bll::bind(&flb::Flight::duration, *bll::_1)));
    mmas_collector<double>::add_line(3, "Distance [km]",       table, true,  flights,
                bll::bind(&flb::Flight::distance, *bll::_1));
    mmas_collector<double>::add_line(4, "TakeoffHeight [MüM]", table, false, flights,
                bll::bind(&flb::Location::height, *bll::bind(&flb::Flight::takeoff, *bll::_1)));



}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A



