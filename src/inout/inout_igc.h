#ifndef INOUT_IGC_H_INCLUDED
#define INOUT_IGC_H_INCLUDED

// flugbuch
#include "FlightDatabase.h"
// boost
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
// standard library
#include <string>
#include <utility>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class inout_igc
{
private:
    struct track_point
    {
        track_point(const boost::posix_time::ptime &timestamp, double lat, double lon, unsigned short alt)
            : timestamp_(timestamp), pos_(std::make_pair(lat, lon)), alt_(alt) { }
        track_point(const track_point &tpnt)
            : timestamp_(tpnt.timestamp_), pos_(tpnt.pos_), alt_(tpnt.alt_) {}

        boost::posix_time::ptime timestamp_;
        std::pair<double, double> pos_;
        unsigned short  alt_;
    };
public:
    inout_igc(boost::shared_ptr<flb::FlightDatabase> flightDb);

    void read(const boost::filesystem::path &source);

    std::pair<double, double> getFirstLastPos(bool first) const;
//    void setTakeoff(boost::shared_ptr<Location> to) { takeoff_ = to; };
//    void setLanding(boost::shared_ptr<Location> la) { landing_ = la; };

    boost::shared_ptr<flb::Flight> flight(void);

private:
    boost::shared_ptr<flb::FlightDatabase> flightDb_;
    std::string                 pilot_, glider_, callsign_, takeoff_, comment_;
    boost::gregorian::date    	date_;
    std::vector<track_point>    track_points_;
//    boost::shared_ptr<Location>                 takeoff_, landing_;
//    std::vector<boost::shared_ptr<Location> >   waypoints_;


};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}; // namespace flb
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif //INOUT_IGC_H_INCLUDED
