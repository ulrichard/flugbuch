// flugbuch
#include "IgcImportForm.h"
#include "FormatStr.h"
#include "FlightTable.h"
#include "TabControl.h"
#include "WaypointOptimizer.h"
// ggl (boost sandbox)
#include <geometry/algorithms/distance.hpp>
#include <geometry/strategies/geographic/geo_distance.hpp>
// witty
#include <Wt/WFileUpload>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WApplication>
#include <Wt/WHBoxLayout>
#include <Wt/WTabWidget>
#include <Wt/WTable>
#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/Ext/MessageBox>
#include <Wt/Ext/Button>
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/lexical_cast.hpp>
// standard library
#include <algorithm>
#include <numeric>

using namespace flbwt;
using namespace flb;
using boost::shared_ptr;
using std::string;
using std::vector;
using std::remove_copy_if;
using namespace boost::lambda;
namespace bll = boost::lambda;
using boost::lexical_cast;

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

            // optimize the waypoints
            WaypointOptimizer::addOptStrategy(new WaypointOptimizerOpenDistance());
            WaypointOptimizer::addOptStrategy(new WaypointOptimizerRichi());

            typedef inout_igc::Trackpoints::iterator::value_type track_point_t;
            WaypointOptimizer wptopt;
            wptopt.initialize(boost::make_transform_iterator(igcfile_.Trackpoints.begin(), boost::bind(&track_point_t::pos_, ::_1)),
                              boost::make_transform_iterator(igcfile_.Trackpoints.end(),   boost::bind(&track_point_t::pos_, ::_1)));
            WaypointOptimizer::OptMap optres = wptopt.optimize();

            // use a button group to logically group the 3 options
            Wt::WButtonGroup *group = new Wt::WButtonGroup(0);
            for(WaypointOptimizer::OptMap::const_iterator it = optres.begin(); it != optres.end(); ++it)
            {
                // calculate the exact distance and score
                vector<double> leglengths;
                WaypointOptimizerStrategyBase::OptRes::const_iterator it0 = it->second.begin();
                WaypointOptimizerStrategyBase::OptRes::const_iterator it1 = it0;
                ++it1;
                for(; it1 != it->second.end(); ++it0, ++it1)
                {
                    inout_igc::Trackpoints::iterator pit0 = igcfile_.Trackpoints.begin();
                    inout_igc::Trackpoints::iterator pit1 = igcfile_.Trackpoints.begin();
                    std::advance(pit0, *it0);
                    std::advance(pit1, *it1);
                    leglengths.push_back(geometry::distance(pit0->pos_, pit1->pos_,
                                                            geometry::strategy::distance::vincenty<geometry::point_ll_deg>()));
                }
                const double dist = std::accumulate(leglengths.begin(), leglengths.end(), 0.0);

                Wt::WRadioButton *button = new Wt::WRadioButton(it->first + " (" + lexical_cast<string>(dist) + "km)");
                group->addButton(button, std::distance<WaypointOptimizer::OptMap::const_iterator>(optres.begin(), it));
                table->elementAt(2, 1)->addWidget(button);
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
