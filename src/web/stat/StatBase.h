#ifndef STATBASE_H_INCLUDED
#define STATBASE_H_INCLUDED

// flugbuch
#include "FlightDatabase.h"
// witty
// boost
// standard library
#include <memory>


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// forward declarations
namespace Wt
{
    class WStandardItemModel;
    class WContainerWidget;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class StatBase
{
protected:
    StatBase(const shared_ptr<flb::FlightDatabase> flightDb) : flightDb_(flightDb) {}
public:
    virtual ~StatBase() {}

    virtual std::string name() const = 0;
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) const = 0;
    virtual void draw(Wt::WContainerWidget *parent, std::auto_ptr<Wt::WStandardItemModel> model) const = 0;

protected:
    const shared_ptr<flb::FlightDatabase> flightDb_;
};


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // STATBASE_H_INCLUDED
