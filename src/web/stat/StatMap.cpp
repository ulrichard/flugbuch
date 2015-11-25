// Flugbuch2
#include "StatMap.h"
#include "FormatStr.h"
// witty
#include <Wt/WGoogleMap>
#include <Wt/WStandardItemModel>
#include <Wt/WContainerWidget>
// boost
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>
// standard library
#include <algorithm>

using namespace flbwt;
using namespace flb;
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
using std::distance;
using std::sort;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
struct FlightSorterPlaces
{
public:
    FlightSorterPlaces(const shared_ptr<FlightDatabase> flightDb) : flightDb_(flightDb) { }

    bool operator()(const shared_ptr<Flight> &lhs, const shared_ptr<Flight> &rhs) const
    {
        if(lhs->takeoff() != rhs->takeoff())
        {
            FlightDatabase::Locations::const_iterator it_to_lhs = find(flightDb_->Locations.begin(), flightDb_->Locations.end(), lhs->takeoff());
            if(it_to_lhs == flightDb_->Locations.end())
                throw std::runtime_error("lhs takeoff location not in db");
            FlightDatabase::Locations::const_iterator it_to_rhs = find(flightDb_->Locations.begin(), flightDb_->Locations.end(), rhs->takeoff());
            if(it_to_rhs == flightDb_->Locations.end())
                throw std::runtime_error("rhs takeoff location not in db");
            return (distance(flightDb_->Locations.begin(), it_to_lhs) < distance(flightDb_->Locations.begin(), it_to_rhs));
        }

        if(lhs->landing() != rhs->landing())
        {
            FlightDatabase::Locations::const_iterator it_la_lhs = find(flightDb_->Locations.begin(), flightDb_->Locations.end(), lhs->landing());
            if(it_la_lhs == flightDb_->Locations.end())
                throw std::runtime_error("lhs landing location not in db");
            FlightDatabase::Locations::const_iterator it_la_rhs = find(flightDb_->Locations.begin(), flightDb_->Locations.end(), rhs->landing());
            if(it_la_rhs == flightDb_->Locations.end())
                throw std::runtime_error("rhs landing location not in db");
            return (distance(flightDb_->Locations.begin(), it_la_lhs) < distance(flightDb_->Locations.begin(), it_la_rhs));
        }

        for(FlightDatabase::Locations::const_iterator it_lhs = lhs->Waypoints.begin(), it_rhs = rhs->Waypoints.begin();
            it_lhs != lhs->Waypoints.end() && it_rhs != rhs->Waypoints.end(); ++it_lhs, ++it_rhs)
        {
            if(*it_lhs != *it_rhs)
            {
                FlightDatabase::Locations::const_iterator it_wp_lhs = find(flightDb_->Locations.begin(), flightDb_->Locations.end(), *it_lhs);
                if(it_wp_lhs == flightDb_->Locations.end())
                    throw std::runtime_error("lhs waypoint location not in db");
                FlightDatabase::Locations::const_iterator it_wp_rhs = find(flightDb_->Locations.begin(), flightDb_->Locations.end(), *it_rhs);
                if(it_wp_rhs == flightDb_->Locations.end())
                    throw std::runtime_error("rhs waypoint location not in db");
                return (distance(flightDb_->Locations.begin(), it_wp_lhs) < distance(flightDb_->Locations.begin(), it_wp_rhs));
            }
        }
        if(lhs->Waypoints.size() != rhs->Waypoints.size())
            if(lhs->Waypoints.size() < rhs->Waypoints.size())
                return true;

        return false;
    }

private:
    const shared_ptr<flb::FlightDatabase> flightDb_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatMap::draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const
{

    Wt::WGoogleMap *gmap = NULL;
    if(parent->count())
        gmap = dynamic_cast<Wt::WGoogleMap*>(parent->widget(0));
    if(!gmap)
    {
        gmap = new Wt::WGoogleMap(parent);
        gmap->resize(800, 500);
        gmap->enableScrollWheelZoom();
        gmap->setMapTypeControl(Wt::WGoogleMap::HierarchicalControl);
    }
    gmap->clearOverlays();

    // if we have multiple flights with the same takeoff/waypoints/landing zone, draw it only once.
    vector<shared_ptr<flb::Flight> > vFlights;
    copy(flights.begin(), flights.end(), back_inserter(vFlights));
    sort(vFlights.begin(), vFlights.end(), FlightSorterPlaces(flightDb_));
    vFlights.erase(unique(vFlights.begin(), vFlights.end(), bll::bind(&flb::Flight::samePlaces, *bll::_1, *bll::_2)), vFlights.end());

    pair<Wt::WGoogleMap::Coordinate, Wt::WGoogleMap::Coordinate> bbox = make_pair(Wt::WGoogleMap::Coordinate(90, 180), Wt::WGoogleMap::Coordinate(-90, -180));
    for(vector<shared_ptr<flb::Flight> >::const_iterator it = vFlights.begin(); it != vFlights.end(); ++it)
    {
        vector<Wt::WGoogleMap::Coordinate> points;
        points.push_back(Wt::WGoogleMap::Coordinate((*it)->takeoff()->pos().first, (*it)->takeoff()->pos().second));
        for(Flight::Waypoints::iterator itw = (*it)->Waypoints.begin(); itw != (*it)->Waypoints.end(); ++itw)
            points.push_back(Wt::WGoogleMap::Coordinate((*itw)->pos().first, (*itw)->pos().second));
        points.push_back(Wt::WGoogleMap::Coordinate((*it)->landing()->pos().first, (*it)->landing()->pos().second));

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

    gmap->setZoom(5);
    gmap->panTo(Wt::WGoogleMap::Coordinate((bbox.first.latitude()  + bbox.second.latitude() / 2.0),
                                           (bbox.first.longitude() + bbox.second.longitude() / 2.0)));
    gmap->zoomWindow(bbox);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
