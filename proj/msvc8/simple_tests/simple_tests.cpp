// witty
#include <Wt/WBorderLayout>
#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WBorderLayout>
#include <Wt/WApplication>
#include <Wt/WTabWidget>
#include <Wt/WImage>
#include <Wt/WStackedWidget>
#include <Wt/WPushButton>
// std lib

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class Testapp_IE : public Wt::WApplication
{
public:
	Testapp_IE(const Wt::WEnvironment& env) : Wt::WApplication(env)
	{
	    Wt::WContainerWidget *contall = root();
	    Wt::WBorderLayout *blayout = new Wt::WBorderLayout();
		blayout->setLayoutHint("table-layout", "fixed");
		contall->setLayout(blayout);


        Wt::WTabWidget *tabw = new Wt::WTabWidget();
        blayout->addWidget(tabw, Wt::WBorderLayout::Center);

        Wt::WContainerWidget *tab1 = new Wt::WContainerWidget();
        tabw->addTab(tab1, "Tab1");
        Wt::WImage *img1 = new Wt::WImage("http://www.swissleague.ch/big/sl2486.jpg");
        tab1->addWidget(img1);

        Wt::WContainerWidget *tab2 = new Wt::WContainerWidget();
        tabw->addTab(tab2, "Tab2");
        Wt::WImage *img2 = new Wt::WImage("http://swissleague.ch/raw/sl2486.jpg");
        tab2->addWidget(img2);

	}
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// callback function is called everytime when a user enters the page. Can be used to authenticate.
Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	Testapp_IE *app = new Testapp_IE(env);

	return app;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
int __main(int argc, char *argv[])
{
	return Wt::WRun(argc, argv, &createApplication);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

