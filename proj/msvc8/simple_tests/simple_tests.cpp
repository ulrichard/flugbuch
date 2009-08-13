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
#include <Wt/WText>
#include <Wt/WTable>
// boost
#include <boost/lexical_cast.hpp>
// std lib

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class Testapp_Scroll : public Wt::WApplication
{
public:
	Testapp_Scroll(const Wt::WEnvironment& env) : Wt::WApplication(env)
	{
	    Wt::WBorderLayout *blayout = new Wt::WBorderLayout();
		root()->setLayout(blayout);

        Wt::WTabWidget *tabw = new Wt::WTabWidget();
        blayout->addWidget(tabw, Wt::WBorderLayout::Center);

        Wt::WContainerWidget *tab1 = new Wt::WContainerWidget();
        tabw->addTab(tab1, "Tab1");
        Wt::WBorderLayout *blayout1 = new Wt::WBorderLayout();
        tab1->setLayout(blayout1);
        Wt::WTable *table1 = new Wt::WTable();
        blayout1->addWidget(table1, Wt::WBorderLayout::Center);
        for(int i=0; i<200; ++i)
        {
            Wt::WText *txt = new Wt::WText(boost::lexical_cast<std::string>(i));
            table1->elementAt(i, 0)->addWidget(txt);
        }
	}
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	Testapp_Scroll *app = new Testapp_Scroll(env);

	return app;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
int main(int argc, char *argv[])
{
	return Wt::WRun(argc, argv, &createApplication);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

