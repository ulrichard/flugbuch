// flugbuch
#include "main.h"
#include "inout_xml.h"
#include "inout_mdb.h"
#include "TabControl.h"
#include "WelcomeScreen.h"
#include "FormatStr.h"
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
// boost
#include <boost/foreach.hpp>
// std lib
#include <string>


using std::string;
//using namespace Wt;
using namespace flbwt;

// callback function is called everytime when a user enters the page. Can be used to authenticate.
Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
   // You could read information from the environment to decide
   // whether the user has permission to start a new application
    FlightLogApp *flapp = new FlightLogApp(env);

	return flapp;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// main entry point of the application
int main(int argc, char *argv[])
{
	return Wt::WRun(argc, argv, &createApplication);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// We have a FlightLogApp object for each session (user...)
FlightLogApp::FlightLogApp(const Wt::WEnvironment& env)
 : Wt::WApplication(env), flightDb_(shared_ptr<flb::FlightDatabase>())
{
	setTitle("Flugbuch 2");               // application title
	useStyleSheet("flugbuch2.css");

    mainStack_     = new Wt::WStackedWidget(root());
    welcomeScreen_ = new WelcomeScreen(mainStack_);
    doLogin();

/*
#ifdef __DEBUG
	flb::inout_mdb iomdb;
	string fileni = "/home/richi/Flugbuch.mdb";
	boost::shared_ptr<flb::FlightDatabase> fldb(new flb::FlightDatabase(iomdb.read(fileni)));

   	flb::inout_xml ioxml;
	string fileno = "/home/richi/Flugbuch2RichardUlrich.xml";
	ioxml.write(*fldb, fileno);
#else
    shared_ptr<flb::FlightDatabase> fldb = flb::FlightDatabase::makeTestDb();
#endif
*/
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void FlightLogApp::finalize()
{
    // todo : ask the user to save
    const string filename = getPersistFileName(flightDb_->pilotName());

    flb::inout_xml ioxml;
	ioxml.write(*flightDb_, filename);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
string FlightLogApp::getPersistFileName(const string &usr)
{
    string persistenceDir;
    if(!readConfigurationProperty("persistenceDir", persistenceDir))
        persistenceDir = "/home/richi/sourcecode/flugbuch2/flb";
	const string filename = FormatStr() << persistenceDir << "/Flugbuch_" << usr << ".xml";
	return filename;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void FlightLogApp::doLogin()
{
    // display the welcome page with different login options
    mainStack_->setCurrentWidget(welcomeScreen_);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void FlightLogApp::loadFlightDb(const string &usr, const string &pwd)
{
    const string filename = getPersistFileName(usr);

    try
    {
        flb::inout_xml ioxml;
        shared_ptr<flb::FlightDatabase> fldb(new flb::FlightDatabase(ioxml.read(filename)));
        if(!fldb->checkPassword(pwd))
            throw std::invalid_argument("incorrect password");

        loadFlights(flb::FlightDatabase::makeTestDb());
   }
    catch(std::exception &ex)
    {
        const string msg(FormatStr() << "Flugbuch für " << usr << " konnte nicht eingelesen werden : " << ex.what());
        throw std::runtime_error(msg);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void FlightLogApp::loadFlights(shared_ptr<flb::FlightDatabase> fldb)
{
    flightDb_ = fldb;
    flbwt::TabControl *tabCtrl = new flbwt::TabControl(fldb, mainStack_);
    mainStack_->setCurrentWidget(tabCtrl);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void FlightLogApp::loadTestDb()
{
    loadFlights(flb::FlightDatabase::makeTestDb());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void FlightLogApp::importFlightDb(const std::string &file, bool del)
{
    flb::inout_mdb iomdb;
	shared_ptr<flb::FlightDatabase> fldb(new flb::FlightDatabase(iomdb.read(file)));

    loadFlights(fldb);

    if(del)
        unlink(file.c_str());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
