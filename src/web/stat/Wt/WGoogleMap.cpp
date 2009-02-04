// witty
#include <Wt/WGoogleMap>
#include <Wt/WApplication>
// boost
#include <boost/bind.hpp>
// standard library
#include <string>
#include <sstream>
#include <iostream>

using namespace Wt;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::swap;

// example javascript code from :
// http://code.google.com/apis/maps/documentation/

WGoogleMap::WGoogleMap(WContainerWidget *parent) : WContainerWidget(0), rendered_(false)
{
    WApplication *app = WApplication::instance();

    // if there is no google api key configured, use the one for http://localhost:8080/
    static const string localhost_key = "ABQIAAAAWqrN5o4-ISwj0Up_depYvhTwM0brOpm-All5BF6PoaKBxRWWERS-S9gPtCri-B6BZeXV8KpT4F80DQ";
    string googlekey;
    if(!app->readConfigurationProperty("google_api_key", googlekey))
        googlekey = localhost_key;

    // init the google javascript api
    const string gmuri = "http://www.google.com/jsapi?key=" + googlekey;
    app->require(gmuri, "GoogleMapsJavaScriptAPI");

    if(parent)
        parent->addWidget(this);
}

void WGoogleMap::refresh()
{
    rendered_ = false;
}

void WGoogleMap::prepareRerender()
{
    if(!rendered_)
    {
        // initialize the map
        std::ostringstream strm;
        strm << "function initialize() {"
             << "    var map = new google.maps.Map2(" << jsRef() << ");"
             << "    map.setCenter(new google.maps.LatLng(47.01887777, 8.651888), 13);"
             << "    map.enableScrollWheelZoom();"
             << "    map.addControl(new google.maps.HierarchicalMapTypeControl());"
             <<      jsRef() << ".map = map;";
        copy(additions_.begin(), additions_.end(), std::ostream_iterator<string>(strm));
        strm << "}";
        strm << "google.load(\"maps\", \"2\", {other_params:\"sensor=false\", callback: initialize});";

        additions_.clear();

        WApplication::instance()->doJavaScript(strm.str());

        WContainerWidget::prepareRerender();
        rendered_ = true;

        std::cout << std::endl << std::endl << strm.str() << std::endl << std::endl;
    }
}

void WGoogleMap::addMarker(const pair<double, double> &pos)
{
    std::ostringstream strm;
    strm << "var marker = new google.maps.Marker(new google.maps.LatLng(" << pos.first << ", " << pos.second << "));"
         <<  jsRef() << ".map.addOverlay(marker);";

    const string jsstr = strm.str();

    if(rendered_)
        WApplication::instance()->doJavaScript(jsstr);
    else
        additions_.push_back(jsstr);
}

void WGoogleMap::addPolyline(const vector<pair<double, double> > &points, const string &color, int width, double opacity)
{
    opacity = std::max(std::min(opacity, 1.0), 0.0); // opacity has to be between 0.0 and 1.0

    std::ostringstream strm;
    strm << "var waypoints = [];";
    for(size_t i=0; i<points.size(); ++i)
        strm << "waypoints[" << i << "] = new google.maps.LatLng(" << points[i].first << ", " << points[i].second << ");";
    strm << "var poly = new google.maps.Polyline(waypoints, \"" << color << "\", " << width << ", " << opacity << ");";
    strm << jsRef() << ".map.addOverlay(poly);";

    const string jsstr = "if(true){" + strm.str() + "}"; // to keep the variables inside a scope where they don't interfere

    if(rendered_)
        WApplication::instance()->doJavaScript(jsstr);
    else
        additions_.push_back(jsstr);
}

void WGoogleMap::zoomWindow(pair<pair<double, double>, pair<double, double> > bbox)
{
    pair<double, double> center = make_pair((bbox.first.first  + bbox.second.first)  / 2.0,
                                            (bbox.first.second + bbox.second.second) / 2.0);
    if(bbox.first.first > bbox.second.first)
        swap(bbox.first.first, bbox.second.first);
    if(bbox.first.second > bbox.second.second)
        swap(bbox.first.second, bbox.second.second);

    std::ostringstream strm;
    strm << "var bbox = new google.maps.LatLngBounds(new google.maps.LatLng(" << bbox.first.first  << ", " << bbox.first.second  << "), "
         <<                                         "new google.maps.LatLng(" << bbox.second.first << ", " << bbox.second.second << "));"
         << "var zooml = " << jsRef() << ".map.getBoundsZoomLevel(bbox);"
         << jsRef() << ".map.setCenter(new google.maps.LatLng(" << center.first << ", " << center.second << "), zooml);";

    if(rendered_)
        WApplication::instance()->doJavaScript(strm.str());
    else
        additions_.push_back(strm.str());
}



