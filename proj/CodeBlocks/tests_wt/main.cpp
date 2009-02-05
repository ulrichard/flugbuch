// flugbuch
// witty
#include <Wt/WBorderLayout>
#include <Wt/Ext/ComboBox>
#include <Wt/WComboBox>
#include <Wt/WEnvironment>
#include <Wt/WGoogleMap>
#include <Wt/WContainerWidget>
#include <Wt/WBorderLayout>
#include <Wt/WApplication>
// std lib
#include <string>
#include <vector>
#include <utility>

using std::string;
using std::vector;
using std::pair;
using std::make_pair;


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class Testapp : public Wt::WApplication
{
public:
    Testapp(const Wt::WEnvironment& env) : Wt::WApplication(env)
    {
        // only for testing
        Wt::WBorderLayout *blayout = new Wt::WBorderLayout();
        root()->setLayout(blayout);
        contw_ = new Wt::WContainerWidget();
        blayout->addWidget(contw_, Wt::WBorderLayout::Center);

        Wt::WComboBox *cbStatSel = new Wt::WComboBox();
//        Wt::Ext::ComboBox *cbStatSel = new Wt::Ext::ComboBox();
        cbStatSel->activated.connect(SLOT(this, Testapp::drawMap));
        cbStatSel->addItem("Marker");
        cbStatSel->addItem("Polyline");
        blayout->addWidget(cbStatSel, Wt::WBorderLayout::North);

        drawMap(0);
    }

    virtual ~Testapp() { }

    void drawMap(int ind)
    {
        contw_->clear();
        Wt::WGoogleMap *gmap = new Wt::WGoogleMap(contw_);
        gmap->resize(700, 500);

        if(ind)
        {
            vector<pair<double, double> > points;
            points.push_back(make_pair(47.06354722, 8.647369)); // Engelstock
            points.push_back(make_pair(47.01887777, 8.651888)); // Steisteg
            gmap->addPolyline(points, "#FF0000", 2, 0.9);

            points.clear();
            points.push_back(make_pair(47.063, 8.647)); // Engelstock
            points.push_back(make_pair(47.018, 8.651)); // Steisteg
            gmap->addPolyline(points, "#FFF000", 2, 0.9);

            gmap->zoomWindow(make_pair(make_pair(47.063, 8.647), make_pair(47.018, 8.651)));
        }
        else
            gmap->addMarker(std::make_pair(47.01887777, 8.651888));
    }

private:
    Wt::WContainerWidget *contw_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// callback function is called everytime when a user enters the page. Can be used to authenticate.
Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
   // You could read information from the environment to decide
   // whether the user has permission to start a new application
    Testapp *app = new Testapp(env);

	return app;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// main entry point of the application
int main(int argc, char *argv[])
{
	return Wt::WRun(argc, argv, &createApplication);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

