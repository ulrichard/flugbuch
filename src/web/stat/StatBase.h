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
/*
    class WContainerWidget;
    class WBorderLayout;
    namespace Ext
    {
        class ComboBox;
    }
    namespace Chart
    {
        class WAbstractChart;
    }
*/
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

    virtual std::string name() = 0;
    virtual std::auto_ptr<Wt::WStandardItemModel> model(const flb::FlightDatabase::SeqFlights &flights) = 0;
    virtual bool pie() = 0; // or cartesian

protected:
    const shared_ptr<flb::FlightDatabase> flightDb_;
};


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // STATBASE_H_INCLUDED
