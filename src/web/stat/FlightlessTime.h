#ifndef FLIGHTLESSTIME_H_INCLUDED
#define FLIGHTLESSTIME_H_INCLUDED


// flugbuch
#include "StatBase.h"
#include "FormatStr.h"
// witty
#include <Wt/WStandardItemModel>
// boost
#include <boost/foreach.hpp>
// standard library


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class FlightlessTime : public StatBase
{
public:
    FlightlessTime(const shared_ptr<flb::FlightDatabase> flightDb) : StatBase(flightDb) {}
    virtual ~FlightlessTime() {}

    virtual std::string name() { return "Fluglose Zeit"; }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights);
    virtual bool pie() { return false; }

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
enum FlightsPerXInterval { FLP_YEAR, FLP_MONTH, FLP_WEEK };
template<FlightsPerXInterval interval>
class FlightsPerPeriod : public StatBase
{
public:
    FlightsPerPeriod(const shared_ptr<flb::FlightDatabase> flightDb) : StatBase(flightDb) {}
    virtual ~FlightsPerPeriod() {}

    virtual std::string name() { return "Fluege pro " + std::string(interval == FLP_YEAR ? "Jahr" : (interval == FLP_MONTH ? "Monat" : "Woche")); }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights)
    {
        assert(flights.size());
        const boost::gregorian::date firstDay = (*flights.begin())->date();
        const boost::gregorian::date lastDay  = (*flights.rbegin())->date();

        std::map<std::string, std::pair<int, int> > counts;

        if(interval == FLP_YEAR)
        {
            for(int yy = firstDay.year(); yy <= lastDay.year(); ++yy)
                counts[FormatStr() << yy] = std::make_pair(0, 0);
        }
        else if(interval == FLP_MONTH)
        {
            const boost::gregorian::date lastMonth = boost::gregorian::date(lastDay.year(), lastDay.month(), 1);
            for(boost::gregorian::date mm = boost::gregorian::date(firstDay.year(), firstDay.month(), 1); mm < lastMonth; mm += boost::gregorian::months(1))
                counts[FormatStr() << mm.year() << "." << mm.month()] = std::make_pair(0, 0);
        }
        else if(interval == FLP_WEEK)
        {
            const boost::gregorian::date lastWeek = boost::gregorian::date(lastDay.year(), lastDay.month(), 1);
            for(boost::gregorian::date ww = boost::gregorian::date(firstDay.year(), firstDay.month(), 1); ww < lastWeek; ww += boost::gregorian::weeks(1))
                counts[FormatStr() << ww.year() << "." << ww.week_number()] = std::make_pair(0, 0);
        }

        BOOST_FOREACH(boost::shared_ptr<flb::Flight> fl, flights)
        {
            if(interval == FLP_YEAR)
            {
                const std::string key(FormatStr() << fl->date().year());
                counts[key].first++;
                counts[key].second += fl->duration();
            }
            else if(interval == FLP_MONTH)
            {
                const std::string key(FormatStr() << fl->date().year() << "." << fl->date().month());
                counts[key].first++;
                counts[key].second += fl->duration();
            }
            else if(interval == FLP_WEEK)
            {
                const std::string key(FormatStr() << fl->date().year() << "." << fl->date().week_number());
                counts[key].first++;
                counts[key].second += fl->duration();
            }
        }

        std::auto_ptr<Wt::WStandardItemModel> model(new Wt::WStandardItemModel(counts.size(), 3));

        int i = 0;
        for(std::map<std::string, std::pair<int, int> >::iterator it=counts.begin(); it!=counts.end(); ++it, ++i)
        {
            model->setData(i, 0, boost::any(it->first));
            model->setData(i, 1, boost::any(it->second.first));
            model->setData(i, 2, boost::any(it->second.second));
        }

        return model;
    }

    virtual bool pie() { return false; }

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class FlightsPerGlider : public StatBase
{
public:
    FlightsPerGlider(const shared_ptr<flb::FlightDatabase> flightDb) : StatBase(flightDb) {}
    virtual ~FlightsPerGlider() {}

    virtual std::string name() { return "Fluege pro Schirm"; }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights);
    virtual bool pie() { return true; }

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // FLIGHTLESSTIME_H_INCLUDED
