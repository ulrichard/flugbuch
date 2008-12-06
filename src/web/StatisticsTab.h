#ifndef STATISTICSTAB_H
#define STATISTICSTAB_H
// flugbuch
#include "FlightDatabase.h"
// witty
#include <Wt/WCompositeWidget>
// boost
#include <boost/shared_ptr.hpp>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class StatisticsPanel : public Wt::WCompositeWidget
{
public:
    StatisticsPanel(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
    virtual ~StatisticsPanel() { }

    void load(int ind);

    void FlightsPerGlider(bool airtime);
private:
    boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    Wt::WContainerWidget      *impl_;
    Wt::Ext::ComboBox         *cbStatSel_;
    Wt::Chart::WAbstractChart *chart_;
    Wt::WBorderLayout         *blayout_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
} // namespace flbwt

#endif // STATISTICSTAB_H
