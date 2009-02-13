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
#include <Wt/Ext/CheckBox>
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
        gmap_  = NULL;
        blayout->addWidget(contw_, Wt::WBorderLayout::Center);

        Wt::WContainerWidget *conth = new Wt::WContainerWidget();
        blayout->addWidget(conth, Wt::WBorderLayout::North);

        Wt::Ext::ComboBox *cbStatSel = new Wt::Ext::ComboBox(conth);
        cbStatSel->activated.connect(SLOT(this, Testapp::drawMap));
        cbStatSel->addItem("Marker");
        cbStatSel->addItem("Polyline");

        cbScrollZoom_ = new Wt::Ext::CheckBox("ScrollWheelZoom", conth);
        cbScrollZoom_->checked.connect(SLOT(this, Testapp::scrollZoom));
        cbScrollZoom_->unChecked.connect(SLOT(this, Testapp::scrollZoom));

        cbDragging_ = new Wt::Ext::CheckBox("Dragging", conth);
        cbDragging_->checked.connect(SLOT(this, Testapp::dragging));
        cbDragging_->unChecked.connect(SLOT(this, Testapp::dragging));

        cbGooBar_ = new Wt::Ext::CheckBox("GoogleBar", conth);
        cbGooBar_->checked.connect(SLOT(this, Testapp::gooBar));
        cbGooBar_->unChecked.connect(SLOT(this, Testapp::gooBar));

        Wt::Ext::ComboBox *cbMapType = new Wt::Ext::ComboBox(conth);
        cbMapType->activated.connect(SLOT(this, Testapp::mapType));
        cbMapType->addItem("Normal");
        cbMapType->addItem("Hierarchical");
        cbMapType->addItem("Menu");

        drawMap(0);
    }

    virtual ~Testapp() { }

    void drawMap(int ind)
    {
        contw_->clear();
        gmap_ = new Wt::WGoogleMap(contw_);
        gmap_->resize(700, 500);

        if(ind)
        {
            vector<Wt::WGoogleMap::LatLng> points;
            points.push_back(Wt::WGoogleMap::LatLng(47.06354722, 8.647369)); // Engelstock
            points.push_back(Wt::WGoogleMap::LatLng(47.01887777, 8.651888)); // Steisteg
            gmap_->addPolyline(points, "#FF0000", 2, 0.9);

            points.clear();
            points.push_back(Wt::WGoogleMap::LatLng(47.063, 8.647)); // Engelstock
            points.push_back(Wt::WGoogleMap::LatLng(47.018, 8.651)); // Steisteg
            gmap_->addPolyline(points, "#FFF000", 2, 0.9);

            gmap_->zoomWindow(make_pair(Wt::WGoogleMap::LatLng(47.063, 8.647), Wt::WGoogleMap::LatLng(47.018, 8.651)));
        }
        else
            gmap_->addMarker(Wt::WGoogleMap::LatLng(47.01887777, 8.651888));
    }

    void mapType(int ind)
    {
        if(gmap_)
        {
            switch(ind)
            {
            case 0:
                gmap_->addMapTypeControl();
                break;
            case 1:
                gmap_->addHierarchicalMapTypeControl();
                break;
            case 2:
                gmap_->addMenuMapTypeControl();
                break;
            }
        }
    }

    void scrollZoom()
    {
        if(gmap_)
        {
            if(cbScrollZoom_->isChecked())
                gmap_->enableScrollWheelZoom();
            else
                gmap_->disableScrollWheelZoom();
        }
    }

    void dragging()
    {
        if(gmap_)
        {
            if(cbDragging_->isChecked())
                gmap_->enableDragging();
            else
                gmap_->disableDragging();
        }
    }

   void gooBar()
    {
        if(gmap_)
        {
            if(cbGooBar_->isChecked())
                gmap_->enableGoogleBar();
            else
                gmap_->disableGoogleBar();
        }
    }

private:
    Wt::WContainerWidget *contw_;
    Wt::WGoogleMap       *gmap_;
    Wt::Ext::CheckBox    *cbScrollZoom_, *cbDragging_, *cbGooBar_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// callback function is called everytime when a user enters the page. Can be used to authenticate.
Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    Testapp *app = new Testapp(env);

	return app;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
int main(int argc, char *argv[])
{
	return Wt::WRun(argc, argv, &createApplication);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

