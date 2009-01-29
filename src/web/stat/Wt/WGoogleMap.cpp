// witty
#include <Wt/WGoogleMap>
#include <Wt/WText>
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
    // if there is no google api key configured, use the one for http://localhost:8080/
    const string localhost_key = "ABQIAAAAWqrN5o4-ISwj0Up_depYvhTwM0brOpm-All5BF6PoaKBxRWWERS-S9gPtCri-B6BZeXV8KpT4F80DQ";
    string googlekey;
    if(!WApplication::instance()->readConfigurationProperty("google_api_key", googlekey))
        googlekey = localhost_key;

    // init the google javascript api
    const string gmuri = "http://www.google.com/jsapi?key=" + googlekey;
    bool bret = WApplication::instance()->require(gmuri, "GoogleMapsJavaScriptAPI");

    // init the google map window
    std::ostringstream strm;
    strm << "google.load(\"maps\", \"2\",{\"other_params\":\"sensor=false\"});";
    strm << "function initialize()"
         << "{"
//         << "    var map = new google.maps.Map2(document.getElementById(\"map\"));"
         << "    var map = new google.maps.Map2(document.getElementById(\"" << id() << "\"));"
         << "    map.setCenter(new google.maps.LatLng(47.01887777, 8.651888), 13);"
         << "    map.enableScrollWheelZoom();"
         << "    map.addControl(new google.maps.HierarchicalMapTypeControl());"
         << "}"
         << "google.setOnLoadCallback(initialize);"
            ;
//    WApplication::instance()->doJavaScript(strm.str(), true);

    // then display the map
//    const string mapstr = "<div id=\"map\" style=\"width:500px;height:300px\"></div>";
//    htmltext_ = new Wt::WText(mapstr, this);
//    htmltext_->setFormatting(Wt::WText::XHTMLUnsafeFormatting);

    htmltext_ = new Wt::WText("<script type=\"text/javascript\">" + strm.str() + "</script>", this);
    htmltext_->setFormatting(Wt::WText::XHTMLUnsafeFormatting);
}
