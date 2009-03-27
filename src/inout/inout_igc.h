#ifndef INOUT_IGC_H_INCLUDED
#define INOUT_IGC_H_INCLUDED

// flugbuch
#include "FlightDatabase.h"
// boost
#include <boost/filesystem.hpp>


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class inout_igc
{
public:
    inout_igc(boost::shared_ptr<flb::FlightDatabase> flightDb);

    void read(const boost::filesystem::path &source);

    boost::shared_ptr<flb::Flight> flight(void);

private:
    boost::shared_ptr<flb::FlightDatabase> flightDb_;


};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}; // namespace flb
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif //INOUT_IGC_H_INCLUDED
