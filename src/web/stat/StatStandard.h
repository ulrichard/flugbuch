#ifndef STATSTANDARD_H_INCLUDED
#define STATSTANDARD_H_INCLUDED

// flugbuch
#include "StatBase.h"
// witty
// boost
// standard library

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class FlightlessTime : public StatChartBase
{
public:
    FlightlessTime(const shared_ptr<flb::FlightDatabase> flightDb) : StatChartBase(flightDb) {}
    virtual ~FlightlessTime() {}

    virtual std::string name() const { return "Fluglose Zeit"; }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) const;
    virtual void draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
enum FlightsPerXInterval { FLP_YEAR, FLP_MONTH, FLP_WEEK };

class FlightsPerPeriod : public StatChartBase
{
public:
    FlightsPerPeriod(const shared_ptr<flb::FlightDatabase> flightDb, FlightsPerXInterval interval) : StatChartBase(flightDb), interval_(interval) {}
    virtual ~FlightsPerPeriod() {}

    virtual std::string name() const { return "Fluege pro " + std::string(interval_ == FLP_YEAR ? "Jahr" : (interval_ == FLP_MONTH ? "Monat" : "Woche")); }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) const;
    virtual void draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const;
private:
    FlightsPerXInterval interval_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class FlightsPerGlider : public StatChartBase
{
public:
    FlightsPerGlider(const shared_ptr<flb::FlightDatabase> flightDb) : StatChartBase(flightDb) {}
    virtual ~FlightsPerGlider() {}

    virtual std::string name() const { return "Fluege pro Schirm"; }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) const;
    virtual void draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class FlightsPerArea : public StatChartBase
{
public:
    FlightsPerArea(const shared_ptr<flb::FlightDatabase> flightDb, size_t numArea = 20) : StatChartBase(flightDb), numArea_(numArea) {}
    virtual ~FlightsPerArea() {}

    virtual std::string name() const { return "Fluege pro Fluggebiet"; }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) const;
    virtual void draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const;

    void setNumArea(size_t val) { numArea_ = std::min<size_t>(val, 50); }
    size_t numArea() const      { return numArea_;}
private:
    size_t numArea_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class StatGeneralOverview : public StatBase
{
public:
    StatGeneralOverview (const shared_ptr<flb::FlightDatabase> flightDb) : StatBase(flightDb) {}
    virtual ~StatGeneralOverview() {}

    virtual std::string name() const { return "General Overview"; }
    virtual void draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const;


};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // STATSTANDARD_H_INCLUDED
