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
using std::max;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatMap::draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const
{
    Wt::WGoogleMap *gmap = new Wt::WGoogleMap();
    gmap->resize(700, 500);
    parent->addWidget(gmap);

    for(flb::FlightDatabase::SeqFlights::const_iterator it = flights.begin(); it != flights.end(); ++it)
    {
        vector<pair<double, double> > points;
        points.push_back((*it)->takeoff()->pos());
        transform((*it)->Waypoints.begin(), (*it)->Waypoints.end(), back_inserter(points), bind(&flb::Location::pos, *bll::_1));
        points.push_back((*it)->landing()->pos());

        gmap->addPolyline(points, "#FF0000", 2, 0.9);
    }

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
