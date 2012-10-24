#ifndef IGCIMPORT_H_INCLUDED
#define IGCIMPORT_H_INCLUDED

// flugbuch
#include "FlightDatabase.h"
#include "inout_igc.h"
// witty
#include <Wt/WCompositeWidget>
#include <Wt/Ext/Dialog>
//standard library
#include <string>
#include <vector>
#include <map>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// forward declarations
namespace Wt
{
    class WFileUpload;
    class WButtonGroup;
    class WGoogleMap;

    namespace Ext
    {
        class ComboBox;
        class LineEdit;
        class Button;
    };
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flbwt
{
    class LocationField;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class NewLocationField : public Wt::WCompositeWidget
{
public:
    NewLocationField(const boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);

    bool hasLocation() const;
//    const boost::shared_ptr<flb::Location> getLocation() const;
    const boost::shared_ptr<flb::FlightArea> getArea() const;
    std::string getLocationName() const;

    size_t trackpntIdx()        const { return trpidx_; }
    void   setTrackpntIdx(size_t val) { trpidx_ = val; }

public slots:
    void   fillAreas();
    size_t selectArea(const std::string &area);
//    void   fillLocations(int idx);
//    size_t selectLocation(const std::string &loc);
    void   setLocationName(const std::string &loc);

private:
    const boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    Wt::WContainerWidget *impl_;
    Wt::Ext::ComboBox    *cbArea_;
    Wt::Ext::LineEdit    *leLocation_;
    size_t                trpidx_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class IgcImportForm : public Wt::Ext::Dialog
{
    typedef boost::geometry::model::ll::point<> point_ll_deg;
public:
    IgcImportForm(boost::shared_ptr<flb::FlightDatabase> flightDb);
    virtual ~IgcImportForm();

private:
    void uploadTooBig(::int64_t size);
    void fileReceived();
    void addFlight();
    void changeWptStrategy();
    Wt::WWidget * setTurnpointField(Wt::WContainerWidget *parent, const point_ll_deg &pos, const flb::Location::UseAs usage, const size_t trpid);


    boost::shared_ptr<flb::FlightDatabase> flightDb_;
    flb::inout_igc  igcfile_;
    std::string     igcname_;
    std::map<std::string, std::vector<size_t> > wptOpt_;
    Wt::WTable      *table_;
    Wt::WFileUpload *fileuploader_;
    std::vector<Wt::WWidget*> vlfWaypoints_;
    Wt::Ext::Button  *btnAddFlight_;
    Wt::WButtonGroup *btnGrpOptStrategy_;
    Wt::WGoogleMap   *gmap_;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
}; // namespace flbwt
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#endif
