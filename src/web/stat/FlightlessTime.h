#ifndef FLIGHTLESSTIME_H_INCLUDED
#define FLIGHTLESSTIME_H_INCLUDED


// flugbuch
#include "StatBase.h"
// witty
#include <Wt/WStandardItemModel>
// boost
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

    virtual std::string name() const { return "Fluglose Zeit"; }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) const;
    virtual void draw(Wt::WContainerWidget *parent, std::auto_ptr<Wt::WStandardItemModel> model) const;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
enum FlightsPerXInterval { FLP_YEAR, FLP_MONTH, FLP_WEEK };

class FlightsPerPeriod : public StatBase
{
public:
    FlightsPerPeriod(const shared_ptr<flb::FlightDatabase> flightDb, FlightsPerXInterval interval) : StatBase(flightDb), interval_(interval) {}
    virtual ~FlightsPerPeriod() {}

    virtual std::string name() const { return "Fluege pro " + std::string(interval_ == FLP_YEAR ? "Jahr" : (interval_ == FLP_MONTH ? "Monat" : "Woche")); }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) const;
    virtual void draw(Wt::WContainerWidget *parent, std::auto_ptr<Wt::WStandardItemModel> model) const;
private:
    FlightsPerXInterval interval_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class FlightsPerGlider : public StatBase
{
public:
    FlightsPerGlider(const shared_ptr<flb::FlightDatabase> flightDb) : StatBase(flightDb) {}
    virtual ~FlightsPerGlider() {}

    virtual std::string name() const { return "Fluege pro Schirm"; }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) const;
    virtual void draw(Wt::WContainerWidget *parent, std::auto_ptr<Wt::WStandardItemModel> model) const;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class FlightsPerArea : public StatBase
{
public:
    FlightsPerArea(const shared_ptr<flb::FlightDatabase> flightDb) : StatBase(flightDb) {}
    virtual ~FlightsPerArea() {}

    virtual std::string name() const { return "Fluege pro Fluggebiet"; }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) const;
    virtual void draw(Wt::WContainerWidget *parent, std::auto_ptr<Wt::WStandardItemModel> model) const;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // FLIGHTLESSTIME_H_INCLUDED
