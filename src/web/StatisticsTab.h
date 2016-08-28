#ifndef STATISTICSTAB_H
#define STATISTICSTAB_H
// flugbuch
#include "FlightDatabase.h"
#include "StatBase.h"
// witty
#include <Wt/WCompositeWidget>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_map.hpp>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// forward declarations
namespace Wt
{
    class WContainerWidget;
    class WSelectionBox;
    namespace Ext
    {
        class ComboBox;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class StatisticsPanel : public Wt::WCompositeWidget
{
public:
    StatisticsPanel(const boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
    virtual ~StatisticsPanel() { }

    void addStatistic(std::unique_ptr<StatBase> stat) { std::string nam = stat->name(); stats_.insert(nam, stat.release()); }

    void load();

private:
    void initFilter();
    bool filter(const flb::Flight &fl) const;

    const boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    Wt::WContainerWidget      *impl_;
    Wt::Ext::ComboBox         *cbStatSel_;
    Wt::WSelectionBox         *sbCountry_;
    Wt::WSelectionBox         *sbYear_;
    Wt::WSelectionBox         *sbClassi_;
    boost::ptr_map<std::string, StatBase> stats_;

    std::set<std::string> filtCountries_;
    std::set<int>         filtYears_;
    std::set<std::string> filtClassi_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flbwt

#endif // STATISTICSTAB_H
