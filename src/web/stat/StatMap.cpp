// Flugbuch2
#include "StatMap.h"
#include "FormatStr.h"
#include "Wt/WGoogleMap"
// witty
#include <Wt/WStandardItemModel>
// boost
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
// standard library
#include <algorithm>

using namespace flbwt;
using Wt::WContainerWidget;
using boost::gregorian::date;
using boost::shared_ptr;
using boost::any;
using namespace boost::lambda;
namespace bll = boost::lambda;
using std::string;
using std::map;
using std::vector;
using std::pair;
using std::make_pair;
using std::auto_ptr;
using std::min;
using std::max;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatMap::draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const
{
    Wt::WGoogleMap *gmap = new Wt::WGoogleMap(parent);
    gmap->resize(700, 500);
    gmap->enableScrollWheelZoom();
    gmap->addHierarchicalMapTypeControl();

    pair<Wt::WGoogleMap::LatLng, Wt::WGoogleMap::LatLng> bbox = make_pair(Wt::WGoogleMap::LatLng(90, 180), Wt::WGoogleMap::LatLng(-90, -180));
    for(flb::FlightDatabase::SeqFlights::const_iterator it = flights.begin(); it != flights.end(); ++it)
    {
        vector<Wt::WGoogleMap::LatLng> points;
        points.push_back(Wt::WGoogleMap::LatLng((*it)->takeoff()->pos()));
        transform((*it)->Waypoints.begin(), (*it)->Waypoints.end(), back_inserter(points), bind(&flb::Location::pos, *bll::_1));
        points.push_back(Wt::WGoogleMap::LatLng((*it)->landing()->pos()));

        // bbox
        for(vector<Wt::WGoogleMap::LatLng>::const_iterator itb = points.begin(); itb != points.end(); ++itb)
        {
            bbox.first.lat_  = min(bbox.first.lat_,  itb->lat_);
            bbox.first.lon_  = min(bbox.first.lon_,  itb->lon_);
            bbox.second.lat_ = max(bbox.second.lat_, itb->lat_);
            bbox.second.lon_ = max(bbox.second.lon_, itb->lon_);
        }

        gmap->addPolyline(points, "#FF0000", 2, 0.7);
    }

    gmap->zoomWindow(bbox);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
