#ifndef STATISTICSTAB_H
#define STATISTICSTAB_H
// flugbuch
#include "FlightDatabase.h"
// witty
#include <Wt/WCompositeWidget>
// boost
#include <boost/shared_ptr.hpp>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// forward declarations
namespace Wt
{
    class WContainerWidget;
    class WBorderLayout;
    namespace Ext
    {
        class ComboBox;
    }
    namespace Chart
    {
        class WAbstractChart;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class StatisticsPanel : public Wt::WCompositeWidget
{
public:
    StatisticsPanel(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
    virtual ~StatisticsPanel() { }

    void load(int ind);

    void FlightsPerGlider(bool airtime);
    void FlightsPerTimePeriod(int mode);
    void FlightAreas(bool airtime);
private:
    boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    Wt::WContainerWidget      *impl_;
    Wt::Ext::ComboBox         *cbStatSel_;
    Wt::Chart::WAbstractChart *chart_;
    Wt::WBorderLayout         *blayout_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flbwt

#endif // STATISTICSTAB_H
