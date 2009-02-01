// Flugbuch2
#include "StatMap.h"
#include "FormatStr.h"
#include "Wt/WGoogleMap"
// witty
#include <Wt/WStandardItemModel>
// boost
#include <boost/foreach.hpp>
// standard library
#include <algorithm>

using namespace flbwt;
using Wt::WContainerWidget;
using boost::gregorian::date;
using boost::shared_ptr;
using boost::any;
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

    Wt::WGoogleMap *gmap = new Wt::WGoogleMap(parent);
    gmap->resize(700, 500);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
