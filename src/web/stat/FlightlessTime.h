#ifndef FLIGHTLESSTIME_H_INCLUDED
#define FLIGHTLESSTIME_H_INCLUDED


// flugbuch
#include "StatBase.h"
// witty
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

    virtual std::string name() { return "Fluglose Zeit"; }
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights);
    virtual bool pie() { return false; }

};


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // FLIGHTLESSTIME_H_INCLUDED
