#define _USE_MATH_DEFINES
// flugbuch
#include "CoreStructures.h"
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
// standard library
#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace flb;
using namespace boost::lambda;
using std::vector;
using boost::shared_ptr;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void Location::setHeight(unsigned short height)
{
    if(height_ > 10000)
        throw std::out_of_range("takeoff in the stratosphere");
    height_   = height;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void Location::setPosition(double lat, double lon)
{
    if(lat < -90.0 || lat > 90.0 || lon < -90.0 || lon > 90.0)
        throw std::out_of_range("position not valid");
    latitude_ = lat;
    longitude_ = lon;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
double Location::getDistance(const std::pair<double, double> &latlon) const
{
	const double lat1 = latitude_    * M_PI / 180.0;
	const double lat2 = latlon.first * M_PI / 180.0;
	const double deltaLong = (latlon.second - longitude_) * M_PI / 180.0;
	const double angle = sinl(lat1) * sinl(lat2) + cosl(lat1) * cosl(lat2) * cosl(deltaLong);
    const double earthRadius = 6371.0; // km
	const double dist = earthRadius * acosl(angle);

	return dist;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bool Location::isEquivalent(const Location &rhs) const
{
    if(abs(height_ - rhs.height()) > 50.0)
        return false;
    if(name_ != rhs.name())
        return false;
    if(getDistance(rhs.pos()) > 0.1)
        return false;
    return true;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
double Flight::calcDistance() const
{
    // get all the relevant locations into a vector
    vector<shared_ptr<Location> > locations;
    locations.push_back(takeoff_);
    copy(waypoints_.begin(), waypoints_.end(), back_inserter(locations));
    locations.push_back(landing_);
    // calculate the lengths of the legs
    vector<double> legLengths;
    vector<shared_ptr<Location> >::const_iterator it1 = locations.begin(), it2 = locations.begin();
    ++it2;
    for( ; it2 != locations.end(); ++it1, ++it2)
        legLengths.push_back((*it1)->getDistance((*it2)->pos()));
    // summm the legs
    return std::accumulate(legLengths.begin(), legLengths.end(), 0.0);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bool Flight::samePlaces(const Flight &other) const
{
    if(takeoff_ != other.takeoff())
        return false;
    if(landing_ != other.landing())
        return false;
    if(waypoints_.size() != other.Waypoints.size())
        return false;
    if(!equal(waypoints_.begin(), waypoints_.end(), other.Waypoints.begin()))
        return false;
    return true;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

