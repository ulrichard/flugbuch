#ifndef MAINFLBWT_H
#define MAINFLBWT_H

// flugbuch
#include "FlightDatabase.h"
// witty
#include <Wt/WApplication>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
//standard library
#include <string>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// forward declarations
namespace Wt
{
    class WStackedWidget;
    class WContainerWidget;
}
namespace flb
{
    class FlightDatabase;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
//forward declarations
class WelcomeScreen;
class TabControl;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class FlightLogApp : public Wt::WApplication
{
public:
    FlightLogApp(const Wt::WEnvironment& env);

    void doLogin();
    void loadFlightDb(const std::string &usr, const std::string &pwd);
    void createFlightDb(const std::string &usr, const std::string &pwd, bool useStdLoc);
    void loadTestDb();
    void importFlightDb(const boost::filesystem::path &file, bool del, const std::string &usr, const std::string &pwd);
    void loadFlights(boost::shared_ptr<flb::FlightDatabase> fldb);
    void saveDb(void);
    void reload(void);

protected:
    virtual void finalize();
    boost::filesystem::path getPersistFileName(const std::string &usr);
private:
   Wt::WStackedWidget                       *mainStack_;
   WelcomeScreen                            *welcomeScreen_;
   Wt::WContainerWidget                     *mainScreen_;
   flbwt::TabControl                        *tabCtrl_;
   boost::shared_ptr<flb::FlightDatabase>   flightDb_;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flbwt
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // MAINFLBWT_H
