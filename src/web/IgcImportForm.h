#ifndef IGCIMPORT_H_INCLUDED
#define IGCIMPORT_H_INCLUDED

// flugbuch
#include "FlightDatabase.h"
#include "inout_igc.h"
// witty
#include <Wt/Ext/Dialog>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// forward declarations
namespace Wt
{
    class WFileUpload;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class IgcImportForm : public Wt::Ext::Dialog
{
public:
    IgcImportForm(boost::shared_ptr<flb::FlightDatabase> flightDb);
    virtual ~IgcImportForm();

private:
    void uploadTooBig(int size);
    void fileReceived();


    boost::shared_ptr<flb::FlightDatabase> flightDb_;
    flb::inout_igc  igcfile_;
    Wt::WFileUpload *fileuploader_;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
}; // namespace flbwt
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#endif
