// witty
#include <Wt/WGoogleMap>
#include <Wt/WApplication>
// standard library
#include <string>
#include <sstream>

using namespace Wt;
using std::string;

// example javascript code from :
// http://code.google.com/apis/maps/documentation/

WGoogleMap::WGoogleMap(WContainerWidget *parent) : WContainerWidget(parent)
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
}

void WGoogleMap::load()
{
    refresh();
}

void WGoogleMap::refresh()
{
    // initialize the map
    std::ostringstream strm;
    strm << "function initialize() {"
         << "    var map = new google.maps.Map2(" << jsRef() << ");"
         << "    map.setCenter(new google.maps.LatLng(47.01887777, 8.651888), 13);"
         << "    map.enableScrollWheelZoom();"
         << "    map.addControl(new google.maps.HierarchicalMapTypeControl());"
         << "}";
    strm << "google.load(\"maps\", \"2\", {other_params:\"sensor=false\", callback: initialize});";

    WApplication::instance()->doJavaScript(strm.str());
}
