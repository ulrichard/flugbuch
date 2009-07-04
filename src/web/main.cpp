// flugbuch
#include "main.h"
#include "inout_xml.h"
#include "TabControl.h"
#include "WelcomeScreen.h"
#include "FormatStr.h"
#include "MainMenu.h"
#include "SystemInformation.h"
#include "IgcImportForm.h"
#ifdef WIN32
  #include "inout_mdb_win.h"
#else
  #include "inout_mdb.h"
#endif
// witty
#include <Wt/WBorderLayout>
#include <Wt/Ext/Dialog>
#include <Wt/Ext/Button>
#include <Wt/WDialog>
#include <Wt/WPushButton>
#include <Wt/WBreak>
#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/WEnvironment>
#include <Wt/Ext/MessageBox>
#include <Wt/WLength>
// boost
#include <boost/foreach.hpp>
// std lib
#include <string>


using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using namespace flbwt;
namespace bfs = boost::filesystem;

// callback function is called everytime when a user enters the page. Can be used to authenticate.
Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
   // You could read information from the environment to decide
   // whether the user has permission to start a new application
    FlightLogApp *flapp = new FlightLogApp(env);

	return flapp;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// main entry point of the application
int main(int argc, char *argv[])
{
	return Wt::WRun(argc, argv, &createApplication);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// We have a FlightLogApp object for each session (user...)
FlightLogApp::FlightLogApp(const Wt::WEnvironment& env)
 : Wt::WApplication(env), flightDb_(shared_ptr<flb::FlightDatabase>())
{
	setTitle("Flugbuch 2");               // application title
	useStyleSheet("flugbuch2.css");

    doLogin();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::finalize()
{
    // todo : ask the user to save
    saveDb();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bfs::path FlightLogApp::getPersistFileName(const string &usr)
{
    string filename = FormatStr()  << "Flugbuch_" << usr << ".xml";
    string persistenceDir;
    if(readConfigurationProperty("persistenceDir", persistenceDir))
        return bfs::path(persistenceDir) / filename;
    else
        return flb::SystemInformation::homeDir() / "flugbuch2" / filename;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::doLogin()
{
    // display the welcome page with different login options
    root()->clear();
    WelcomeScreen *welcomeScreen = new WelcomeScreen();
    root()->addWidget(welcomeScreen);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::loadFlightDb(const string &usr, const string &pwd)
{
    try
    {
        const bfs::path filename = getPersistFileName(usr);

        flb::inout_xml ioxml;
        shared_ptr<flb::FlightDatabase> fldb(new flb::FlightDatabase(ioxml.read(filename)));
        if(!fldb->checkPassword(pwd))
            throw std::invalid_argument("incorrect password");

        loadFlights(fldb);
    }
    catch(std::exception &ex)
    {
        const string msg(FormatStr() << "Flugbuch für " << usr << " konnte nicht eingelesen werden : " << ex.what());
        throw std::runtime_error(msg);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::createFlightDb(const string &usr, const string &pwd, bool useStdLoc)
{
    shared_ptr<flb::FlightDatabase> fldb(new flb::FlightDatabase(usr, pwd));

    if(useStdLoc)
    {
        shared_ptr<flb::FlightDatabase> testdb = flb::FlightDatabase::makeTestDb();
        // todo: copy the locations
    }

    loadFlights(fldb);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::loadFlights(shared_ptr<flb::FlightDatabase> fldb)
{
    flightDb_ = fldb;

    root()->clear();
    root()->setStyleClass("flb_root");
	Wt::WBorderLayout *borderLayout = new Wt::WBorderLayout();
	borderLayout->setLayoutHint("table-layout", "fixed");
	root()->setLayout(borderLayout);

    flbwt::MainMenu  *mainMenu = new flbwt::MainMenu();
	borderLayout->addWidget(mainMenu, Wt::WBorderLayout::North);

    flbwt::TabControl *tabCtrl = new flbwt::TabControl(fldb);
    tabCtrl->setStyleClass("flb_tab_control");
	borderLayout->addWidget(tabCtrl,  Wt::WBorderLayout::Center);

    string footerText;
    if(!readConfigurationProperty("footerText", footerText))
        footerText = "flugbuch2 ist opensource software im alpha stadium.";
	Wt::WText *txtFooter = new Wt::WText(footerText);
	borderLayout->addWidget(txtFooter, Wt::WBorderLayout::South);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::loadTestDb()
{
    try
    {
        loadFlights(flb::FlightDatabase::makeTestDb());
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error loading test db", ex.what(), Wt::WFlags<Wt::StandardButton>(), true);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::importFlightDb(const bfs::path &file, bool del, const string &usr, const string &pwd)
{
    try
    {
        flb::inout_mdb iomdb;
        shared_ptr<flb::FlightDatabase> fldb(new flb::FlightDatabase(iomdb.read(file)));
        fldb->setPilotNameAndPwd(usr, pwd);

        loadFlights(fldb);

#ifndef WIN32
        if(del)
            bfs::remove(file);
#endif
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error importing flight db", ex.what(), Wt::WFlags<Wt::StandardButton>(), true);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::saveDb()
{
    try
    {
        const bfs::path filename = getPersistFileName(flightDb_->pilotName());

        flb::inout_xml ioxml;
        ioxml.write(*flightDb_, filename);
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error saving flight db", ex.what(), Wt::WFlags<Wt::StandardButton>(), true);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::reload()
{
    try
    {
        if(flightDb_->pilotName() == "*testdb*")
            loadTestDb();
        else
        {
            const bfs::path filename = getPersistFileName(flightDb_->pilotName());
            flb::inout_xml ioxml;
            shared_ptr<flb::FlightDatabase> fldb(new flb::FlightDatabase(ioxml.read(filename)));
            // no password checking this time
            loadFlights(fldb);
        }
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error reloading flight db", ex.what(), Wt::WFlags<Wt::StandardButton>(), true);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightLogApp::importIGC()
{
    IgcImportForm *impDlg = new IgcImportForm(flightDb_);
    impDlg->show();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A



