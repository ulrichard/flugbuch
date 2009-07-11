// flugbuch
#include "IgcImportForm.h"
#include "FormatStr.h"
#include "FlightTable.h"
#include "TabControl.h"
// witty
#include <Wt/WFileUpload>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WApplication>
#include <Wt/WHBoxLayout>
#include <Wt/WTabWidget>
#include <Wt/WTable>
#include <Wt/Ext/MessageBox>
#include <Wt/Ext/Button>
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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
const shared_ptr<flb::FlightArea> NewLocationField::getArea() const
{
    shared_ptr<FlightArea> area = flightDb_->getArea(cbArea_->currentText().narrow());
    return area;
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
    resize(555, 300);
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

    btnAddFlight_ = new Wt::Ext::Button("Flug hinzufügen");
    btnAddFlight_->clicked().connect(SLOT(this, IgcImportForm::addFlight));
    btnAddFlight_->setEnabled(false);
    addButton(btnAddFlight_);
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
    Wt::Ext::MessageBox::show("Error", msg, Wt::Ok, true);
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
            Wt::Ext::MessageBox::show("Error", "No trackpoints read from the file!", Wt::Ok, true);
            accept();
        }
        else
        {
            contents()->clear();


            Wt::WTable *table = new Wt::WTable(contents());

            // find the takeoff
            table->elementAt(0, 0)->addWidget(new Wt::WText("Startplatz"));
            vector<shared_ptr<Location> > tmploc;
            remove_copy_if(flightDb_->Locations.begin(), flightDb_->Locations.end(), back_inserter(tmploc),
                !ret<bool>(bind(&Location::usage, *bll::_1) & Location::UA_TAKEOFF) ||
                bind(&Location::getDistance, *bll::_1, igcfile_.Trackpoints.begin()->pos_) > distThreshold);
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

                table->elementAt(1, 1)->addWidget(nlfLanding_);
            }

            btnAddFlight_->setEnabled(true);
        }
    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Ok, true);
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
        newFlight->setDuration(boost::posix_time::seconds((igcfile_.Trackpoints.rbegin()->timestamp_ - igcfile_.Trackpoints.begin()->timestamp_).total_seconds()));
        newFlight->clearWaypoints();
        if(lfTakeoff_)
            newFlight->setTakeoff(lfTakeoff_->getLocation());
        else if(nlfTakeoff_)
        {
            shared_ptr<Location> locTakeoff(new Location(nlfTakeoff_->getArea(),
                                                        nlfTakeoff_->getLocationName(),
                                                        igcfile_.Trackpoints.begin()->alt_,
                                                        igcfile_.Trackpoints.begin()->pos_,
                                                        Location::UA_TAKEOFF));
            flightDb_->addLocation(locTakeoff);
            newFlight->setTakeoff(locTakeoff);
        }
        else
            throw std::runtime_error("Wether existing nor new location for takeoff");
        // landing
        if(lfLanding_)
            newFlight->setLanding(lfLanding_->getLocation());
        else if(nlfLanding_)
        {
            shared_ptr<Location> locLanding(new Location(nlfLanding_->getArea(),
                                                        nlfLanding_->getLocationName(),
                                                        igcfile_.Trackpoints.rbegin()->alt_,
                                                        igcfile_.Trackpoints.rbegin()->pos_,
                                                        Location::UA_LANDING));
            flightDb_->addLocation(locLanding);
            newFlight->setLanding(locLanding);
        }
        else
            throw std::runtime_error("Wether existing nor new location for landing");

        // close the dialog
        accept();

        // edit the new flight
        assert(Wt::WApplication::instance()->root()->count() == 2);
        TabControl *tabCtrl = dynamic_cast<TabControl*>(Wt::WApplication::instance()->root()->widget(1));
        if(!tabCtrl)
            throw std::runtime_error("Tab control not found");
        FlightPanel *flightPanel = tabCtrl->flightPanel();
        assert(flightPanel);
        tabCtrl->setCurrentIndex(0);       // switch to the flight table
        FlightTable *flightTable = flightPanel->flightTable();
        const unsigned int pagesCount = 1 + flightTable->entriesCount() / flightTable->entriesPerPage();
        flightTable->loadPage(pagesCount); // switch to the last page
        flightPanel->pagesList()->load();
        flightDb_->addFlight(newFlight);
        FlightTableRow *flrow = flightTable->addFlight(newFlight, flightTable->rowCount() - 1, true);
        flrow->edit();
        flightTable->createFooterRow();

    }
    catch(std::exception &ex)
    {
        Wt::Ext::MessageBox::show("Error", ex.what(), Wt::Ok, true);
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
