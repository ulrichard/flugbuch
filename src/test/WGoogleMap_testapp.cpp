// flugbuch
// witty
#include <Wt/WBorderLayout>
#include <Wt/Ext/ComboBox>
#include <Wt/WComboBox>
#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WBorderLayout>
#include <Wt/WApplication>
#include <Wt/Ext/CheckBox>
#include <Wt/WHBoxLayout>
#include <Wt/Ext/MessageBox>
#include <Wt/WBreak>

#include <Wt/WGoogleMap>
#include <Wt/WGeoPosEdit>
// boost
#include <boost/lexical_cast.hpp>
// std lib
#include <string>
#include <vector>
#include <utility>

using boost::lexical_cast;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class Testapp_GM : public Wt::WApplication
{
public:
    Testapp_GM(const Wt::WEnvironment& env) : Wt::WApplication(env)
    {
        Wt::WContainerWidget *contall = new Wt::WContainerWidget(root());
        Wt::WBorderLayout *blayout = new Wt::WBorderLayout();
        contall->setLayout(blayout);
        contw_ = new Wt::WContainerWidget();
        gmap_  = NULL;
        blayout->addWidget(contw_, Wt::WBorderLayout::Center);

        Wt::WContainerWidget *conth = new Wt::WContainerWidget();
        conth->setLayout(new Wt::WHBoxLayout());
        blayout->addWidget(conth, Wt::WBorderLayout::North);

        Wt::Ext::ComboBox *cbStatSel = new Wt::Ext::ComboBox();
        cbStatSel->activated.connect(SLOT(this, Testapp_GM::drawMap));
        cbStatSel->addItem("Marker");
        cbStatSel->addItem("Polyline");
        conth->layout()->addWidget(cbStatSel);

		cbScrollZoom_ = new Wt::Ext::CheckBox("ScrollWheelZoom");
		cbScrollZoom_->checked.connect(SLOT(this, Testapp_GM::scrollZoom));
		cbScrollZoom_->unChecked.connect(SLOT(this, Testapp_GM::scrollZoom));
		conth->layout()->addWidget(cbScrollZoom_);

		cbDblClickZoom_ = new Wt::Ext::CheckBox("DoubleClickZoom");
		cbDblClickZoom_->setChecked(true);
		cbDblClickZoom_->checked.connect(SLOT(this, Testapp_GM::dblclickZoom));
		cbDblClickZoom_->unChecked.connect(SLOT(this, Testapp_GM::dblclickZoom));
		conth->layout()->addWidget(cbDblClickZoom_);

        cbDragging_ = new Wt::Ext::CheckBox("Dragging");
        cbDragging_->setChecked(true);
        cbDragging_->checked.connect(SLOT(this, Testapp_GM::dragging));
        cbDragging_->unChecked.connect(SLOT(this, Testapp_GM::dragging));
        conth->layout()->addWidget(cbDragging_);

        cbGooBar_ = new Wt::Ext::CheckBox("GoogleBar");
        cbGooBar_->checked.connect(SLOT(this, Testapp_GM::gooBar));
        cbGooBar_->unChecked.connect(SLOT(this, Testapp_GM::gooBar));
        conth->layout()->addWidget(cbGooBar_);

        Wt::Ext::ComboBox *cbMapType = new Wt::Ext::ComboBox();
        cbMapType->activated.connect(SLOT(this, Testapp_GM::mapType));
        cbMapType->addItem("Normal");
        cbMapType->addItem("Hierarchical");
        cbMapType->addItem("Menu");
        conth->layout()->addWidget(cbMapType);

        Wt::WGeoPosEdit *geoEditS = new Wt::WGeoPosEdit(NULL, Wt::WGeoPosEdit::WGS84_SEC);
        blayout->addWidget(geoEditS, Wt::WBorderLayout::South);

        drawMap(0);
    }

    virtual ~Testapp_GM() { }

    void drawMap(int ind)
    {
        contw_->clear();
        gmap_ = new Wt::WGoogleMap(contw_);
        gmap_->setCenter(Wt::WLatLng(47.01887777, 8.651888), 13);
        gmap_->resize(700, 500);

//        gmap_->clicked.connect(SLOT(this, Testapp_GM::positionPopup));
        gmap_->dblclicked.connect(SLOT(this, Testapp_GM::positionPopup));

        if(ind)
        {
            vector<Wt::WLatLng> points;
            points.push_back(Wt::WLatLng(47.06354722, 8.647369)); // Engelstock
            points.push_back(Wt::WLatLng(47.01887777, 8.651888)); // Steisteg
            gmap_->addPolyline(points, "#FF0000", 2, 0.9);

            points.clear();
            points.push_back(Wt::WLatLng(47.063, 8.647)); // Engelstock
            points.push_back(Wt::WLatLng(47.018, 8.651)); // Steisteg
            gmap_->addPolyline(points, "#FFF000", 2, 0.9);

            gmap_->zoomWindow(make_pair(Wt::WLatLng(47.063, 8.647), Wt::WLatLng(47.018, 8.651)));
        }
        else
            gmap_->addMarker(Wt::WLatLng(47.01887777, 8.651888));

		scrollZoom();
		dblclickZoom();
		dragging();
		gooBar();
    }

    void positionPopup(Wt::WLatLng lalo)
    {
        string latlonstr = lexical_cast<string>(lalo.lat()) + ", " + lexical_cast<string>(lalo.lon());
        Wt::Ext::MessageBox::show("Clicked at position", latlonstr, Wt::Warning, true);
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

	void dblclickZoom()
	{
		if(gmap_)
		{
			if(cbDblClickZoom_->isChecked())
				gmap_->enableDoubleClickZoom();
			else
				gmap_->disableDoubleClickZoom();
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
    Wt::Ext::CheckBox    *cbScrollZoom_, *cbDblClickZoom_, *cbDragging_, *cbGooBar_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// callback function is called everytime when a user enters the page. Can be used to authenticate.
Wt::WApplication *createApplication_GM(const Wt::WEnvironment& env)
{
    Testapp_GM *app = new Testapp_GM(env);

	return app;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
int __main(int argc, char *argv[])
{
	return Wt::WRun(argc, argv, &createApplication_GM);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

