#ifndef INOUT_IGC_H_INCLUDED
#define INOUT_IGC_H_INCLUDED

// flugbuch
#include "FlightDatabase.h"
#include "inout.h"
// ggl (boost sandbox)
#include <boost/geometry/extensions/gis/latlong/latlong.hpp>
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
class inout_igc : public inout_flight
{
    typedef boost::geometry::model::ll::point<> point_ll_deg;
private:
    struct track_point
    {
        track_point(const boost::posix_time::ptime &timestamp, const point_ll_deg pos, unsigned short alt)
            : timestamp_(timestamp), pos_(pos), alt_(alt) { }
        track_point(const track_point &tpnt)
            : timestamp_(tpnt.timestamp_), pos_(tpnt.pos_), alt_(tpnt.alt_) {}

        boost::posix_time::ptime timestamp_;
        point_ll_deg pos_;
        unsigned short  alt_;
    };
public:
    inout_igc(boost::shared_ptr<flb::FlightDatabase> flightDb);

    virtual void read(const boost::filesystem::path &source);
    virtual void write(const Flight &flt, const boost::filesystem::path &destination);

    std::string             pilotName()   const { return pilot_; }
    std::string             gliderName()  const { return glider_; }
    std::string             callsign()    const { return callsign_; }
    std::string             takeoffName() const { return takeoff_; }
    std::string             comment()     const { return comment_; }
    boost::gregorian::date  date()        const { return date_; }

    boost::shared_ptr<flb::Flight> flight(void);

private:
    boost::shared_ptr<flb::FlightDatabase> flightDb_;
    std::string                 pilot_, glider_, callsign_, takeoff_, comment_;
    boost::gregorian::date    	date_;
    std::vector<track_point>    track_points_;

public:
    // memberspace for the trackpoints
    struct Trackpoints
	{
	    typedef std::vector<track_point>::const_iterator         iterator;
	    typedef std::vector<track_point>::const_iterator         const_iterator;
	    typedef std::vector<track_point>::const_reverse_iterator reverse_iterator;
	    typedef std::vector<track_point>::const_reverse_iterator const_reverse_iterator;

        iterator       begin()         { return ioigc_.track_points_.begin(); }
        const_iterator begin()  const  { return ioigc_.track_points_.begin(); }
        iterator       end()           { return ioigc_.track_points_.end(); }
        const_iterator end()    const  { return ioigc_.track_points_.end(); }
        reverse_iterator       rbegin()        { return ioigc_.track_points_.rbegin(); }
        const_reverse_iterator rbegin() const  { return ioigc_.track_points_.rbegin(); }
        reverse_iterator       rend()          { return ioigc_.track_points_.rend(); }
        const_reverse_iterator rend()   const  { return ioigc_.track_points_.rend(); }

        size_t size() const { return ioigc_.track_points_.size(); }
    private:
        friend class inout_igc;
        Trackpoints(const inout_igc &ioigc) : ioigc_(ioigc) { }
        const inout_igc &ioigc_;
    }Trackpoints;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}; // namespace flb
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif //INOUT_IGC_H_INCLUDED
