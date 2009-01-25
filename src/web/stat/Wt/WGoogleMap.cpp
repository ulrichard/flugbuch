// witty
#include <Wt/WGoogleMap>
#include <Wt/WText>
// standard library
#include <sstream>

using namespace Wt;

// example javascript code from :
// http://code.google.com/apis/maps/documentation/

WGoogleMap::WGoogleMap(Wt::WContainerWidget *parent, const std::string &key) : WPanel(parent)
{
    std::ostringstream strm;

    strm << "<script type=\"text/javascript\" src=\"http://www.google.com/jsapi?key=" << key << "\"></script>";
    strm << "<script type=\"text/javascript\">google.load(\"maps\", \"2\",{\"other_params\":\"sensor=true\"});"
         << "function initialize()"
         << "{"
         << "    var map = new google.maps.Map2(document.getElementById(\"map\"));"
         << "    map.setCenter(new google.maps.LatLng(37.4419, -122.1419), 13);"
         << "}"
         << "google.setOnLoadCallback(initialize);"
         << "</script>";


    htmltext_ = new Wt::WText(strm.str(), parent);
    htmltext_->setFormatting(Wt::WText::XHTMLUnsafeFormatting);

}
