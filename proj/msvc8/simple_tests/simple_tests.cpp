// witty
#include <Wt/WBorderLayout>
#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WBorderLayout>
#include <Wt/WApplication>
#include <Wt/WTabWidget>
#include <Wt/WImage>
// std lib

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class Testapp_IE : public Wt::WApplication
{
public:
	Testapp_IE(const Wt::WEnvironment& env) : Wt::WApplication(env)
	{
		// only for testing
		contall_ = new Wt::WContainerWidget(root());
		Wt::WBorderLayout *blayout = new Wt::WBorderLayout();
		contall_->setLayout(blayout);
		contall_->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));

		contw_ = new Wt::WContainerWidget();
		blayout->addWidget(contw_, Wt::WBorderLayout::Center);
		contw_->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));

		tabw_ = new Wt::WTabWidget(contw_);
		tabw_->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));
		tabw_->currentChanged.connect(SLOT(this, Testapp_IE::resizeAgain));

		Wt::WContainerWidget *tab1 = new Wt::WContainerWidget();
		tabw_->addTab(tab1, "Tab1");
		Wt::WImage *img1 = new Wt::WImage("http://www.swissleague.ch/big/sl2486.jpg");
		tab1->addWidget(img1);
		tab1->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));

		Wt::WContainerWidget *tab2 = new Wt::WContainerWidget();
		tabw_->addTab(tab2, "Tab2");
		Wt::WImage *img2 = new Wt::WImage("http://swissleague.ch/raw/sl2486.jpg");
		tab2->addWidget(img2);
		tab2->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));

	}

	virtual ~Testapp_IE() { }

	void resizeAgain(int ind)
	{
	    root()->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));
	    contall_->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));
	    contw_->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));
	    tabw_->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));
        tabw_->currentWidget()->resize(Wt::WLength(100.0, Wt::WLength::Percentage), Wt::WLength(100.0, Wt::WLength::Percentage));
	}

private:
	Wt::WTabWidget *tabw_;
	Wt::WContainerWidget *contall_, *contw_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// callback function is called everytime when a user enters the page. Can be used to authenticate.
Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	Testapp_IE *app = new Testapp_IE(env);

	return app;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
int main(int argc, char *argv[])
{
	return Wt::WRun(argc, argv, &createApplication);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

