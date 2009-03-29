// flugbuch
#include "inout_igc.h"
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
// standard library
#include <stdexcept>

using namespace flb;
using boost::shared_ptr;
using boost::lexical_cast;
using namespace boost::lambda;
namespace bfs   = boost::filesystem;
namespace bgreg = boost::gregorian;
namespace bpt   = boost::posix_time;
using std::string;
using std::pair;
using std::make_pair;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
inout_igc::inout_igc(shared_ptr<flb::FlightDatabase> flightDb) : flightDb_(flightDb)
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_igc::read(const boost::filesystem::path &source)
{
    bfs::ifstream ifs(source);
    static const string hdrDate     = "HFDTE";
    static const string hdrPilot    = "HPPLTPILOT:";
    static const string hdrGlider   = "HPGTYGLIDERTYPE:";
    static const string hdrCallsign = "HFGIDGLIDERID:";
    static const string hdrGpsDatum = "HODTM100GPSDATUM:";
    static const string hdrTakeoff  = "HOSITSite:";
    static const string hdrComment  = "LPLT";

    char linec[1024];
    while(ifs.getline(linec, sizeof(linec)))
    {
        const string line(linec);
        if(line.substr(0, hdrDate.length()) == hdrDate)
        {
            unsigned short day  = lexical_cast<unsigned short>(line.substr(hdrDate.length(),     2));
            unsigned short mon  = lexical_cast<unsigned short>(line.substr(hdrDate.length() + 2, 2));
            unsigned short year = lexical_cast<unsigned short>(line.substr(hdrDate.length() + 4, 2));
            date_ = bgreg::date(year, mon, day);
        }
        else if(line.substr(0, hdrPilot.length()) == hdrPilot)
            pilot_    = line.substr(hdrPilot.length(), line.length());
        else if(line.substr(0, hdrGlider.length()) == hdrGlider)
            glider_   = line.substr(hdrGlider.length(), line.length());
        else if(line.substr(0, hdrCallsign.length()) == hdrCallsign)
            callsign_ = line.substr(hdrCallsign.length(), line.length());
         else if(line.substr(0, hdrTakeoff.length()) == hdrTakeoff)
            takeoff_ = line.substr(hdrTakeoff.length(), line.length());
        else if(line.substr(0, hdrComment.length()) == hdrComment)
            comment_ = line.substr(hdrComment.length(), line.length());
        else if(line.substr(0, hdrGpsDatum.length()) == hdrGpsDatum)
        {
            if(line.substr(hdrGpsDatum.length(), line.length()) != "WGS-84")
                throw std::runtime_error("unsupported map datum");
        }
       else if(line[0] == 'B')
        {
            double lat, lon;
            int hh, mm, ss;
            int latdeg, latminute, latmindec;
            int londeg, lonminute, lonmindec;
            int altPress, altGPS;
            char northsouth;
            char eastwest;
            char valid;

            if(sscanf(linec,
                    "%*c%2d%2d%2d%2d%2d%3d%c%3d%2d%3d%c%c%5d%5d",
                    &hh, &mm, &ss, &latdeg, &latminute, &latmindec, &northsouth, &londeg,
                    &lonminute, &lonmindec, &eastwest, &valid, &altPress, &altGPS) == 14)
            {
                lat = ((double) latdeg + ((double) latminute / 60.0) + ((double) latmindec / 60000.0));
                lon = ((double) londeg + ((double) lonminute / 60.0) + ((double) lonmindec / 60000.0));

                track_point tpt(bpt::ptime(date_, bpt::time_duration(hh, mm, ss)),
                                northsouth == 'N' ? lat : -lat,
                                eastwest   == 'E' ? lon : -lon,
                                altGPS ? altGPS : altPress);
                track_points_.push_back(tpt);
            }
        }
    } // while getline
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<double, double> inout_igc::getFirstLastPos(bool first) const
{
    if(!track_points_.size())
        throw std::runtime_error("no trackpoints");
    if(first)
        return track_points_.front().pos_;
    else
        return track_points_.back().pos_;
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
