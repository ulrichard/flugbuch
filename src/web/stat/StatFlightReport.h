#ifndef STATFLIGHTREPORT_H_INCLUDED
#define STATFLIGHTREPORT_H_INCLUDED

// flugbuch
#include "StatBase.h"
// witty
// boost
// standard library

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class StatFlightReport : public StatBase
{
public:
    StatFlightReport(const shared_ptr<flb::FlightDatabase> flightDb) : StatBase(flightDb) {}
    virtual ~StatFlightReport() {}

    virtual std::string name() const { return "Flugdetails"; }
    virtual void draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flbwt
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // STATFLIGHTREPORT_H_INCLUDED
