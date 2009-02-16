// witty
#include <Wt/WGoogleMap>
#include <Wt/WApplication>
// boost
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
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

WGoogleMap::WGoogleMap(WContainerWidget *parent) : WContainerWidget(0), click(this), click_js(this, "click"), rendered_(false)
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

    click_js.connect(SLOT(this, WGoogleMap::click_relay));
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
             <<      jsRef() << ".map = map;";
        // eventhandling
        strm << "    google.maps.Event.addListener(map, \"click\", function(overlay, latlng) "
             << "    {"
             << "        if(latlng)"
             << "        {"
             << "            Wt.emit(\"" << id() << "\", \"" << click_js.name() << "\", latlng.lat(), latlng.lng());"
             << "        }"
             << "    }";

        // additional things
        copy(additions_.begin(), additions_.end(), std::ostream_iterator<string>(strm));
        strm << "}";
        strm << "google.load(\"maps\", \"2\", {other_params:\"sensor=false\", callback: initialize});";

        additions_.clear();

        WApplication::instance()->doJavaScript(strm.str());

        WContainerWidget::prepareRerender();
        rendered_ = true;

//        std::cout << std::endl << std::endl << strm.str() << std::endl << std::endl;
    }
}

void WGoogleMap::doGmJavaScript(std::string jscode, bool sepScope)
{
    if(sepScope)
        jscode = "if(true){" + jscode + "}"; // to keep the variables inside a scope where they don't interfere

    if(rendered_)
        WApplication::instance()->doJavaScript(jscode);
    else
        additions_.push_back(jscode);
}

void WGoogleMap::addMarker(const WGoogleMap::LatLng &pos)
{
    std::ostringstream strm;
    strm << "var marker = new google.maps.Marker(new google.maps.LatLng(" << pos.lat_ << ", " << pos.lon_ << "));"
         <<  jsRef() << ".map.addOverlay(marker);";

    doGmJavaScript(strm.str(), false);
}

void WGoogleMap::addPolyline(const vector<WGoogleMap::LatLng> &points, const string &color, int width, double opacity)
{
    opacity = std::max(std::min(opacity, 1.0), 0.0); // opacity has to be between 0.0 and 1.0

    std::ostringstream strm;
    strm << "var waypoints = [];";
    for(size_t i=0; i<points.size(); ++i)
        strm << "waypoints[" << i << "] = new google.maps.LatLng(" << points[i].lat_ << ", " << points[i].lon_ << ");";
    strm << "var poly = new google.maps.Polyline(waypoints, \"" << color << "\", " << width << ", " << opacity << ");";
    strm << jsRef() << ".map.addOverlay(poly);";

    doGmJavaScript(strm.str(), true);
}

void WGoogleMap::setCenter(const WGoogleMap::LatLng &center, int zoom)
{
    std::ostringstream strm;
    strm << jsRef() << ".map.setCenter(new google.maps.LatLng(" << center.lat_ << ", " << center.lon_ << "), " << zoom << ");";

    doGmJavaScript(strm.str(), false);
}

void WGoogleMap::panTo(const WGoogleMap::LatLng &center)
{
    std::ostringstream strm;
    strm << jsRef() << ".map.panTo(new google.maps.LatLng(" << center.lat_ << ", " << center.lon_ << "));";

    doGmJavaScript(strm.str(), false);
}

void WGoogleMap::setZoom(int level)
{
    doGmJavaScript(jsRef() + ".map.setZoom(" + boost::lexical_cast<std::string>(level) + ");", false);
}


void WGoogleMap::zoomWindow(pair<WGoogleMap::LatLng, WGoogleMap::LatLng > bbox)
{
    LatLng center((bbox.first.lat_ + bbox.second.lat_) / 2.0,
                  (bbox.first.lon_ + bbox.second.lon_) / 2.0);
    if(bbox.first.lat_ > bbox.second.lat_)
        swap(bbox.first.lat_, bbox.second.lat_);
    if(bbox.first.lon_ > bbox.second.lon_)
        swap(bbox.first.lon_, bbox.second.lon_);

    std::ostringstream strm;
    strm << "var bbox = new google.maps.LatLngBounds(new google.maps.LatLng(" << bbox.first.lat_  << ", " << bbox.first.lon_  << "), "
         <<                                         "new google.maps.LatLng(" << bbox.second.lat_ << ", " << bbox.second.lon_ << "));"
         << "var zooml = " << jsRef() << ".map.getBoundsZoomLevel(bbox);"
         << jsRef() << ".map.setCenter(new google.maps.LatLng(" << center.lat_ << ", " << center.lon_ << "), zooml);";

    doGmJavaScript(strm.str(), true);
}

void WGoogleMap::addMapTypeControl()
{
    std::ostringstream strm;
    strm << "var mtc = new google.maps.MapTypeControl();"
         << jsRef() << ".map.removeControl(" << jsRef() << ".mtc);"
         << jsRef() << ".mtc = mtc;"
         << jsRef() << ".map.addControl(mtc);";
    doGmJavaScript(strm.str(), false);
}

void WGoogleMap::addHierarchicalMapTypeControl()
{
    std::ostringstream strm;
    strm << "var mtc = new google.maps.HierarchicalMapTypeControl();"
         << jsRef() << ".map.removeControl(" << jsRef() << ".mtc);"
         << jsRef() << ".mtc = mtc;"
         << jsRef() << ".map.addControl(mtc);";
    doGmJavaScript(strm.str(), false);
}

void WGoogleMap::addMenuMapTypeControl()
{
    std::ostringstream strm;
    strm << "var mtc = new google.maps.MenuMapTypeControl();"
         << jsRef() << ".map.removeControl(" << jsRef() << ".mtc);"
         << jsRef() << ".mtc = mtc;"
         << jsRef() << ".map.addControl(mtc);";
    doGmJavaScript(strm.str(), false);
}

void WGoogleMap::click_relay(double lat, double lon)
{
    click.emit(LatLng(lat, lon));
}

