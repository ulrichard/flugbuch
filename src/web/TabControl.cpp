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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
TabControl::TabControl(boost::shared_ptr<flb::FlightDatabase> flightDb, Wt::WContainerWidget *parent)
// : Wt::Ext::TabWidget(parent), flightDb_(flightDb)
 : Wt::WTabWidget(parent), flightDb_(flightDb)
{
    // add the flights
//    FlightTable *flightTable = new FlightTable(flightDb_);
//    addTab(flightTable, "Fluege");
    FlightPanel *flightPanel = new FlightPanel(flightDb_);
    addTab(flightPanel, "Fluege");
    // add the gliders
//    GliderTable *gliderTable = new GliderTable(flightDb_);
//    addTab(gliderTable, "Schirme");
    GliderPanel *gliderPanel = new GliderPanel(flightDb_);
    addTab(gliderPanel, "Schirme");
    // add the flight areas
//    AreaTable *areaTable = new AreaTable(flightDb_);
//    addTab(areaTable, "Fluggebiete");
    AreaPanel *areaPanel = new AreaPanel(flightDb_);
    addTab(areaPanel, "Fluggebiete");
    // add the flight areas
//    LocationTable *locTable = new LocationTable(flightDb_);
//    addTab(locTable, "Orte");
    LocationPanel *locPanel = new LocationPanel(flightDb_);
    addTab(locPanel, "Orte");
    // add a tab with some special functions

/*

    // add areas panel
    Panel *panAreas = new Panel();
    panAreas->setTitle("Gebiete");
    addTab(panAreas);

    // add locations panel
    Panel *panLocations = new Panel();
    panLocations->setTitle("Plaetze");
    addTab(panLocations);
*/
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
