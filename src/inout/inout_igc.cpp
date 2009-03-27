// flugbuch
#include "inout_igc.h"
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace flb;
using boost::shared_ptr;
using namespace boost::lambda;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
inout_igc::inout_igc(shared_ptr<flb::FlightDatabase> flightDb) : flightDb_(flightDb)
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_igc::read(const boost::filesystem::path &source)
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
shared_ptr<flb::Flight> inout_igc::flight(void) // throws std::exception
{
    // first make sure, we have gliders and locations defined
    if(!flightDb_->Gliders.size())
        throw std::runtime_error("no gliders found");

    // find the first takeoff and the first landing place
    FlightDatabase::Locations::const_iterator itTo = find_if(flightDb_->Locations.begin(), flightDb_->Locations.end(),
                        bind(&Location::usage, *_1) & static_cast<int>(Location::UA_TAKEOFF));
    FlightDatabase::Locations::const_iterator itLa = find_if(flightDb_->Locations.begin(), flightDb_->Locations.end(),
                        bind(&Location::usage, *_1) & static_cast<int>(Location::UA_LANDING));
    if(itTo == flightDb_->Locations.end())
        throw std::runtime_error("no takeoff found");
    if(itLa == flightDb_->Locations.end())
        throw std::runtime_error("no landing zone found");

    // create the new flight
    shared_ptr<Flight> flight(new Flight(1,                              // number
                                            boost::gregorian::day_clock::local_day(), // date
                                            0,                              // airtime
                                            *flightDb_->Gliders.begin(),    // glider
                                            *itTo,                          // takeoff
                                            *itLa));                        // landing



    return flight;
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
