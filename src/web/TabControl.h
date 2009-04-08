#ifndef TABCONTROL_H
#define TABCONTROL_H

// flugbuch
#include "FlightDatabase.h"
// witty
#include <Wt/Ext/TabWidget>
#include <Wt/WTabWidget>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
    // forward declarations
    class FlightPanel;
    class GliderPanel;
    class AreaPanel;
    class LocationPanel;
    class StatisticsPanel;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
//class TabControl : public Wt::Ext::TabWidget
class TabControl : public Wt::WTabWidget
{
public:
    TabControl(boost::shared_ptr<flb::FlightDatabase> flightDb, Wt::WContainerWidget *parent = 0);

    void reload(boost::shared_ptr<flb::FlightDatabase> flightDb);

    FlightPanel     * flightPanel() { return flightPanel_; }
    GliderPanel     * gliderPanel() { return gliderPanel_; }
    AreaPanel       * areaPanel()   { return areaPanel_; }
    LocationPanel   * locPanel()    { return locPanel_; }
    StatisticsPanel * statPanel()   {return statPanel_; }

private:
    boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    FlightPanel     *flightPanel_;
    GliderPanel     *gliderPanel_;
    AreaPanel       *areaPanel_;
    LocationPanel   *locPanel_;
    StatisticsPanel *statPanel_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flbwt
#endif // TABCONTROL_H
