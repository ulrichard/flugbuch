// flugbuch
#include "IgcImportForm.h"
#include "FormatStr.h"
#include "FlightTable.h"
// witty
#include <Wt/WFileUpload>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WApplication>
#include <Wt/WHBoxLayout>
#include <Wt/Ext/MessageBox>
#include <Wt/Ext/Button>
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace flbwt;
using namespace flb;
using boost::shared_ptr;
using std::string;
using std::vector;
using std::remove_copy_if;
using namespace boost::lambda;
namespace bll = boost::lambda;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
NewLocationField::NewLocationField(const boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), impl_(new Wt::WContainerWidget()),
   cbArea_(new Wt::Ext::ComboBox(impl_)), leLocation_(new Wt::Ext::LineEdit(impl_))
{
    setImplementation(impl_);
    impl_->setLayout(new Wt::WHBoxLayout());
    impl_->layout()->addWidget(cbArea_);
    impl_->layout()->addWidget(leLocation_);

	fillAreas();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void NewLocationField::fillAreas()
{
    while(cbArea_->count())
        cbArea_->removeItem(0);

    for_each(flightDb_->FlightAreas.begin(), flightDb_->FlightAreas.end(), bind(&Wt::Ext::ComboBox::addItem, cbArea_, bind(&FlightArea::name, *bll::_1)));
    cbArea_->setCurrentIndex(0);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
size_t NewLocationField::selectArea(const string &area)
{
    for(int i=0; i<cbArea_->count(); ++i)
        if(area == cbArea_->itemText(i).narrow())
        {
            cbArea_->setCurrentIndex(i);
            return i;
        }
    return 0;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bool NewLocationField::hasLocation() const
{
    return (cbArea_->currentText().narrow().length() && leLocation_->text().narrow().length());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void NewLocationField::setLocationName(const std::string &loc)
{
    leLocation_->setText(loc);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
string NewLocationField::getLocationName() const
{
    return leLocation_->text().narrow();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
IgcImportForm::IgcImportForm(shared_ptr<flb::FlightDatabase> flightDb)
  : Wt::Ext::Dialog("Igc file import"), flightDb_(flightDb), igcfile_(flightDb_),
    lfTakeoff_(NULL), lfLanding_(NULL), nlfTakeoff_(NULL), nlfLanding_(NULL)
{
    resize(555, 150);
    setSizeGripEnabled(false);

    fileuploader_ = new Wt::WFileUpload();
    fileuploader_->uploaded().connect(SLOT(this, IgcImportForm::fileReceived));
    fileuploader_->fileTooLarge().connect(SLOT(this, IgcImportForm::uploadTooBig));
    fileuploader_->changed().connect(SLOT(fileuploader_, Wt::WFileUpload::upload));
    fileuploader_->setFileTextSize(50);
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
    accept();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void IgcImportForm::fileReceived()
{
    static const double distThreshold = 0.3; // km
    try
    {
//        fileuploader_->stealSpooledFile();
        igcfile_.read(fileuploader_->spoolFileName());

        if(!igcfile_.Trackpoints.size())
        {
            Wt::Ext::MessageBox::show("Error", "No trackpoints read from the file!", Wt::WFlags<Wt::StandardButton>(), true);
            accept();
        }
        else
        {
            resize(550, 300);
            contents()->clear();

            Wt::Ext::Button *btnAddFlight = new Wt::Ext::Button("Flug hinzufügen");
            btnAddFlight->clicked().connect(SLOT(this, IgcImportForm::addFlight));
            addButton(btnAddFlight);

            Wt::WTable *table = new Wt::WTable(contents());

            // find the takeoff
            table->elementAt(0, 0)->addWidget(new Wt::WText("Startplatz"));
            vector<shared_ptr<Location> > tmploc;
            remove_copy_if(flightDb_->Locations.begin(), flightDb_->Locations.end(), back_inserter(tmploc),
                !ret<bool>(bind(&Location::usage, *bll::_1) & Location::UA_TAKEOFF) ||
                bind(&Location::getDistance, *bll::_1, igcfile_.Trackpoints.begin()->pos_) > distThreshold);
//            tmploc.erase(remove_if(tmploc.begin(), tmploc.end(),
//                bind(&Location::getDistance, *bll::_1, igcfile_.Trackpoints.begin()->pos_) > distThreshold), tmploc.end());
            if(tmploc.size())
            {
                sort(tmploc.begin(), tmploc.end(), bind(&Location::getDistance, *bll::_1, igcfile_.Trackpoints.begin()->pos_)
                                                 < bind(&Location::getDistance, *bll::_2, igcfile_.Trackpoints.begin()->pos_));
                lfTakeoff_ = new LocationField(flightDb_, flb::Location::UA_TAKEOFF);
                lfTakeoff_->setLocation(**tmploc.begin());

                table->elementAt(0, 1)->addWidget(lfTakeoff_);
            }
            else
            {
                nlfTakeoff_ = new NewLocationField(flightDb_);
                nlfTakeoff_->fillAreas();

                table->elementAt(0, 1)->addWidget(nlfTakeoff_);
            }

            // find the landing zone
            table->elementAt(1, 0)->addWidget(new Wt::WText("Landeplatz"));
            tmploc.clear();
            remove_copy_if(flightDb_->Locations.begin(), flightDb_->Locations.end(), back_inserter(tmploc),
                !ret<bool>(bind(&Location::usage, *bll::_1) & Location::UA_LANDING) ||
                bind(&Location::getDistance, *bll::_1, igcfile_.Trackpoints.rbegin()->pos_) > distThreshold);
            if(tmploc.size())
            {
                sort(tmploc.begin(), tmploc.end(), bind(&Location::getDistance, *bll::_1, igcfile_.Trackpoints.rbegin()->pos_)
                                                 < bind(&Location::getDistance, *bll::_2, igcfile_.Trackpoints.rbegin()->pos_));
                lfLanding_ = new LocationField(flightDb_, flb::Location::UA_LANDING);
                lfLanding_->setLocation(**tmploc.begin());

                table->elementAt(1, 1)->addWidget(lfLanding_);
            }
            else
            {
                nlfLanding_ = new NewLocationField(flightDb_);
                nlfLanding_->fillAreas();

                table->elementAt(0, 1)->addWidget(nlfLanding_);
            }



        }

    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::WFlags<Wt::StandardButton>(), true);
        accept();
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void IgcImportForm::addFlight()
{
    try
    {
        // create a new flight by copying the last flight and modify some fields
        shared_ptr<Flight> newFlight(new Flight(**flightDb_->flights().rbegin()));
        newFlight->setNumber(newFlight->number() + 1);
        newFlight->setDate(igcfile_.Trackpoints.begin()->timestamp_.date());
        newFlight->setDuration((igcfile_.Trackpoints.rbegin()->timestamp_ - igcfile_.Trackpoints.begin()->timestamp_).total_seconds() / 60);
        newFlight->clearWaypoints();
        if(lfTakeoff_)
            newFlight->setTakeoff(lfTakeoff_->getLocation());
        if(lfLanding_)
            newFlight->setLanding(lfLanding_->getLocation());

        flightDb_->addFlight(newFlight);

        accept();
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::WFlags<Wt::StandardButton>(), true);
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
