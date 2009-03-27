// flugbuch
#include "IgcImportForm.h"
#include "FormatStr.h"
// witty
#include <Wt/WFileUpload>
#include <Wt/WContainerWidget>
#include <Wt/WApplication>
#include <Wt/Ext/MessageBox>
#include <Wt/Ext/Button>

using namespace flbwt;
using boost::shared_ptr;

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
//    fileuploader_->stealSpooledFile();

    igcfile_.read(fileuploader_->spoolFileName());



    shared_ptr<flb::Flight> flight = igcfile_.flight();

    flightDb_->addFlight(flight);
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
