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
    // for now use the key for http://localhost:8080/
    // todo : read the key from the configuration file
    const string localhost_key = "ABQIAAAAWqrN5o4-ISwj0Up_depYvhTwM0brOpm-All5BF6PoaKBxRWWERS-S9gPtCri-B6BZeXV8KpT4F80DQ";
    // init the google javascript api
    const string gmuri = "http://www.google.com/jsapi?key=" + localhost_key;
    bool bret = WApplication::instance()->require(gmuri, "GoogleMapsJavaScriptAPI");

    std::ostringstream strm;
    strm << "google.load(\"maps\", \"2\",{\"other_params\":\"sensor=false\"});"
//         << "function initialize()"
//         << "{"
         << "    var map = new google.maps.Map2(document.getElementById(\"map\"));"
         << "    map.setCenter(new google.maps.LatLng(47.01887777, 8.651888), 13);"
//         << "}"
//         << "google.setOnLoadCallback(initialize);"
            ;
    WApplication::instance()->doJavaScript(strm.str());


    // then display the map
    const string mapstr = "<div id=\"map\" style=\"width:500px;height:300px\"></div>";
    htmltext_ = new Wt::WText(mapstr, this);
    htmltext_->setFormatting(Wt::WText::XHTMLUnsafeFormatting);

}
