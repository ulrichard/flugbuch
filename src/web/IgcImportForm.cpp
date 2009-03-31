// flugbuch
#include "IgcImportForm.h"
#include "FormatStr.h"
// witty
#include <Wt/WFileUpload>
#include <Wt/WContainerWidget>
#include <Wt/WApplication>
#include <Wt/Ext/MessageBox>
#include <Wt/Ext/Button>
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace flbwt;
using namespace flb;
using boost::shared_ptr;
using std::vector;
using std::remove_copy_if;
using namespace boost::lambda;
namespace bll = boost::lambda;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
IgcImportForm::IgcImportForm(shared_ptr<flb::FlightDatabase> flightDb)
  : Wt::Ext::Dialog("Igc file import"), flightDb_(flightDb), igcfile_(flightDb_)
{
    resize(508, 440);
    setSizeGripEnabled(false);

    fileuploader_ = new Wt::WFileUpload();
    fileuploader_->uploaded().connect(SLOT(this, IgcImportForm::fileReceived));
    fileuploader_->fileTooLarge().connect(SLOT(this, IgcImportForm::uploadTooBig));
    contents()->addWidget(fileuploader_);



    Wt::Ext::Button *btnCancel = new Wt::Ext::Button("Cancel");
    btnCancel->clicked().connect(SLOT(this, IgcImportForm::accept));
    addButton(btnCancel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
IgcImportForm::~IgcImportForm()
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void IgcImportForm::uploadTooBig(int size)
{
    int maxreqsize = Wt::WApplication::instance()->maximumRequestSize();
    std::string msg(FormatStr() << "Die Datei ist zu gross fuer den upload :\n"
                                << "Upload  : " << size / 1024 << " kB\n"
                                << "Maximum : " << maxreqsize / 1024 << " kB");
//    im_txt_->setText(msg);
    Wt::Ext::MessageBox::show("Error", msg, Wt::WFlags<Wt::StandardButton>(), true);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void IgcImportForm::fileReceived()
{
    static const double distThreshold = 0.3; // km
    try
    {
//      fileuploader_->stealSpooledFile();

        igcfile_.read(fileuploader_->spoolFileName());
        // find the takeoff
        vector<shared_ptr<Location> > tmploc;
//        remove_copy_if(flightDb_->Locations.begin(), flightDb_->Locations.end(), back_inserter(tmploc),
//            bind(&Location::usage, *bll::_1) & Location::UA_TAKEOFF &&
//            bind(&Location::getDistance, *bll::_1, igcfile_.Trackpoints.begin()->pos_) < distThreshold);





        shared_ptr<flb::Flight> flight = igcfile_.flight();

        flightDb_->addFlight(flight);
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::WFlags<Wt::StandardButton>(), true);
        accept();
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
