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
#include <boost/lambda/construct.hpp>
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

    vector<shared_ptr<flb::Flight> > vFlights;
    copy(flights.begin(), flights.end(), back_inserter(vFlights));
    vFlights.erase(unique(vFlights.begin(), vFlights.end(), bll::bind(&flb::Flight::samePlaces, *bll::_1, *bll::_2)), vFlights.end());

    pair<Wt::WGoogleMap::Coordinate, Wt::WGoogleMap::Coordinate> bbox = make_pair(Wt::WGoogleMap::Coordinate(90, 180), Wt::WGoogleMap::Coordinate(-90, -180));
    for(vector<shared_ptr<flb::Flight> >::const_iterator it = vFlights.begin(); it != vFlights.end(); ++it)
    {
        vector<Wt::WGoogleMap::Coordinate> points;
        points.push_back(Wt::WGoogleMap::Coordinate((*it)->takeoff()->pos()));
        transform((*it)->Waypoints.begin(), (*it)->Waypoints.end(), back_inserter(points),
            bind(bll::constructor<Wt::WGoogleMap::Coordinate>(), bind(&flb::Location::pos, *bll::_1)));
        points.push_back(Wt::WGoogleMap::Coordinate((*it)->landing()->pos()));

        // bbox
        for(vector<Wt::WGoogleMap::Coordinate>::const_iterator itb = points.begin(); itb != points.end(); ++itb)
        {
            bbox.first.setLatitude(  min(bbox.first.latitude(),   itb->latitude()));
            bbox.first.setLongitude( min(bbox.first.longitude(),  itb->longitude()));
            bbox.second.setLatitude( max(bbox.second.latitude(),  itb->latitude()));
            bbox.second.setLongitude(max(bbox.second.longitude(), itb->longitude()));
        }

        gmap->addPolyline(points, Wt::WColor("#FF0000"), 2, 0.7);
    }

    gmap->zoomWindow(bbox);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
