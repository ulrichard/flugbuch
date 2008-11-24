// flugbuch
#include "TabControl.h"
#include "FlightTable.h"
#include "GliderTable.h"
#include "AreaTable.h"
#include "LocationTable.h"
// witty
#include <Wt/Ext/Panel>
#include <Wt/WFitLayout>

using namespace flbwt;
using Wt::Ext::Panel;
using std::string;
using boost::shared_ptr;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
TabControl::TabControl(shared_ptr<flb::FlightDatabase> flightDb, Wt::WContainerWidget *parent)
// : Wt::Ext::TabWidget(parent), flightDb_(flightDb)
 : Wt::WTabWidget(parent), flightDb_(flightDb)
{
    // add the flights
    flightPanel_ = new FlightPanel(flightDb_);
    addTab(flightPanel_, "Fluege");
    // add the gliders
    gliderPanel_ = new GliderPanel(flightDb_);
    addTab(gliderPanel_, "Schirme");
    // add the flight areas
    areaPanel_ = new AreaPanel(flightDb_);
    addTab(areaPanel_, "Fluggebiete");
    // add the locations
    locPanel_ = new LocationPanel(flightDb_);
    addTab(locPanel_, "Orte");
    // add a tab with some special functions

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void TabControl::reload(shared_ptr<flb::FlightDatabase> flightDb)
{
    flightPanel_->filter();
    gliderPanel_->filter();
    areaPanel_->filter();
    locPanel_->filter();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
