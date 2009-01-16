// flugbuch
#include "FlightlessTime.h"
#include "FormatStr.h"
// witty
#include <Wt/WStandardItemModel>
// boost
#include <boost/foreach.hpp>
// standard library
#include <algorithm>

using namespace flbwt;
using std::string;
using std::map;
using std::vector;
using std::auto_ptr;
using std::max;
using boost::gregorian::date;
using boost::shared_ptr;
using boost::any;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
auto_ptr<Wt::WStandardItemModel> FlightlessTime::model(const flb::FlightDatabase::SeqFlights &flights)
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

    auto_ptr<Wt::WStandardItemModel> model(new  Wt::WStandardItemModel(flightlessWeeks.size(), 3));

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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
