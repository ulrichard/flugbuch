#ifndef IGCIMPORT_H_INCLUDED
#define IGCIMPORT_H_INCLUDED

// flugbuch
#include "FlightDatabase.h"
#include "inout_igc.h"
// witty
#include <Wt/WCompositeWidget>
#include <Wt/Ext/Dialog>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// forward declarations
namespace Wt
{
    class WFileUpload;

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
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class IgcImportForm : public Wt::Ext::Dialog
{
public:
    IgcImportForm(boost::shared_ptr<flb::FlightDatabase> flightDb);
    virtual ~IgcImportForm();

private:
    void uploadTooBig(int size);
    void fileReceived();
    void addFlight();


    boost::shared_ptr<flb::FlightDatabase> flightDb_;
    flb::inout_igc  igcfile_;
    Wt::WFileUpload *fileuploader_;
    LocationField   *lfTakeoff_, *lfLanding_;
    std::vector<LocationField*> vlfWaypoints_;
    NewLocationField *nlfTakeoff_, *nlfLanding_;
    Wt::Ext::Button *btnAddFlight_;


};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
}; // namespace flbwt
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#endif
