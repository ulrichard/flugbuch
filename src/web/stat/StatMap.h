#ifndef STATMAP_H_INCLUDED
#define STATMAP_H_INCLUDED


// flugbuch
#include "StatBase.h"
// witty
// boost
// standard library

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class StatMap : public StatBase
{
public:
    StatMap(const shared_ptr<flb::FlightDatabase> flightDb) : StatBase(flightDb) {}
    virtual ~StatMap() {}

    virtual std::string name() const { return "LandKarte"; }
    virtual void draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flbwt
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // STATMAP_H_INCLUDED
