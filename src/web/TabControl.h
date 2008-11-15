#ifndef TABCONTROL_H
#define TABCONTROL_H

// flugbuch
#include "FlightDatabase.h"
// witty
#include <Wt/Ext/TabWidget>
#include <Wt/WTabWidget>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
//class TabControl : public Wt::Ext::TabWidget
class TabControl : public Wt::WTabWidget
{
public:
    TabControl(boost::shared_ptr<flb::FlightDatabase> flightDb, Wt::WContainerWidget *parent = 0);

private:
    boost::shared_ptr<flb::FlightDatabase>  flightDb_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
} // namespace flbwt
#endif // TABCONTROL_H
