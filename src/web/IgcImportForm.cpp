// flugbuch
#include "IgcImportForm.h"
#include "FlightTable.h"
#include "TabControl.h"
#include "WaypointOptimizer.h"
#include "igc_storage.h"
#include "FormatStr.h"
#include "SystemInformation.h"
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
#include <Wt/WGoogleMap>
#include <Wt/WBreak>
#include <Wt/WBorderLayout>
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
#include <iomanip>
#include <utility>

using namespace flbwt;
using namespace flb;
using namespace boost::lambda;
namespace bll = boost::lambda;
namespace bfs = boost::filesystem;
using boost::lexical_cast;
using boost::shared_ptr;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::remove_copy_if;

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
  : Wt::Ext::Dialog("Igc file import"), flightDb_(flightDb), igcfile_(flightDb_), gmap_(NULL)
{
    resize(650, 320);
    setSizeGripEnabled(false);

    fileuploader_ = new Wt::WFileUpload();
    fileuploader_->uploaded().connect(SLOT(this, IgcImportForm::fileReceived));
    fileuploader_->fileTooLarge().connect(SLOT(this, IgcImportForm::uploadTooBig));
    fileuploader_->changed().connect(SLOT(fileuploader_, Wt::WFileUpload::upload));
    fileuploader_->setFileTextSize(58);
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
    try
    {
//        fileuploader_->stealSpooledFile();

        // read the file
        igcfile_.read(fileuploader_->spoolFileName());

        if(!igcfile_.Trackpoints.size())
        {
            Wt::Ext::MessageBox::show("Error", "No trackpoints read from the file!", Wt::Ok, true);
            accept();
        }
        else
        {
            // copy the igc file
            igcname_ = fileuploader_->clientFileName().narrow();
            string igcBaseDir;
            if(!Wt::WApplication::readConfigurationProperty("igcBaseDir", igcBaseDir))
                igcBaseDir = (flb::SystemInformation::homeDir() / "gipsy").string();
            flb::igc_storage igcstore(bfs::path(igcBaseDir) / flightDb_->pilotName(), true);
            bfs::path igcfile = igcstore.make_igc_filename(igcname_, igcfile_.Trackpoints.begin()->timestamp_.date().year());
            bfs::create_directories(igcfile.parent_path());
            bfs::copy_file(bfs::path(fileuploader_->spoolFileName()), igcfile);
            if(!bfs::exists(igcfile))
            {
                Wt::Ext::MessageBox::show("Error", "Failed to copy the igc file to the igc files directory!", Wt::Ok, true);
                accept();
            }

            contents()->clear();
            Wt::WTable *tblbrd = new Wt::WTable(contents());
            table_ = new Wt::WTable(tblbrd->elementAt(0, 0));
            table_->elementAt(0, 0)->addWidget(new Wt::WText("Startplatz"));
            table_->elementAt(2, 0)->addWidget(new Wt::WText("Wegpunkte"));
            table_->elementAt(3, 0)->addWidget(new Wt::WText("Landeplatz"));

            // optimize the waypoints
            typedef inout_igc::Trackpoints::iterator::value_type track_point_t;
            WaypointOptimizer wptopt;
            wptopt.initialize(boost::make_transform_iterator(igcfile_.Trackpoints.begin(), boost::bind(&track_point_t::pos_, ::_1)),
                              boost::make_transform_iterator(igcfile_.Trackpoints.end(),   boost::bind(&track_point_t::pos_, ::_1)));
            wptOpt_ = wptopt.optimize();

            // use a button group to logically group the options
            btnGrpOptStrategy_ = new Wt::WButtonGroup(0);
            for(WaypointOptimizer::OptMap::const_iterator it = wptOpt_.begin(); it != wptOpt_.end(); ++it)
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
                const double dist = std::accumulate(leglengths.begin(), leglengths.end(), 0.0) / 1000.0;

                if(it != wptOpt_.begin())
                    table_->elementAt(1, 1)->addWidget(new Wt::WBreak());
                const string btntxt = FormatStr() << it->first << " (" << std::fixed << std::setprecision(2) << dist << " km)";
                Wt::WRadioButton *button = new Wt::WRadioButton(btntxt);
                button->checked().connect(SLOT(this, IgcImportForm::changeWptStrategy));
                btnGrpOptStrategy_->addButton(button, std::distance<WaypointOptimizer::OptMap::const_iterator>(wptOpt_.begin(), it));
                table_->elementAt(1, 1)->addWidget(button);
            }
            btnGrpOptStrategy_->button(0)->setChecked(true);

            // the google map control
            gmap_ = new Wt::WGoogleMap(tblbrd->elementAt(0, 1));
            gmap_->resize(400, 250);
            gmap_->enableScrollWheelZoom();

            changeWptStrategy();

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
void IgcImportForm::changeWptStrategy()
{
    table_->elementAt(0, 1)->clear(); // takeoff
    table_->elementAt(2, 1)->clear(); // waypoints
    table_->elementAt(3, 1)->clear(); // landing
    vlfWaypoints_.clear();

    const size_t optid = btnGrpOptStrategy_->checkedId();

    map<string, vector<size_t> >::iterator mit = wptOpt_.begin();
    std::advance(mit, optid);
    const vector<size_t> &ids = mit->second;
    for(vector<size_t>::const_iterator it = ids.begin(); it != ids.end(); ++it)
    {
        assert(*it < igcfile_.Trackpoints.size());
        inout_igc::Trackpoints::iterator tpit = igcfile_.Trackpoints.begin();
        std::advance(tpit, *it);

        if(it == ids.begin())
            vlfWaypoints_.push_back(setTurnpointField(table_->elementAt(0, 1), tpit->pos_, Location::UA_TAKEOFF, *it));
        else if(1 == std::distance<vector<size_t>::const_iterator>(it, ids.end()))
            vlfWaypoints_.push_back(setTurnpointField(table_->elementAt(3, 1), tpit->pos_, Location::UA_LANDING, *it));
        else
            vlfWaypoints_.push_back(setTurnpointField(table_->elementAt(2, 1), tpit->pos_, Location::UA_WAYPNT, *it));
    }

    gmap_->clearOverlays();
    // draw the flight
    vector<Wt::WGoogleMap::Coordinate> points;
    for(inout_igc::Trackpoints::const_iterator itp = igcfile_.Trackpoints.begin(); itp != igcfile_.Trackpoints.end(); ++itp)
        points.push_back(Wt::WGoogleMap::Coordinate(itp->pos_.lat(), itp->pos_.lon()));
    gmap_->addPolyline(points, Wt::WColor("#EE4444"), 3, 0.4);
    // bounding box
    pair<Wt::WGoogleMap::Coordinate, Wt::WGoogleMap::Coordinate> bbox = std::make_pair(Wt::WGoogleMap::Coordinate(90, 180), Wt::WGoogleMap::Coordinate(-90, -180));
    for(vector<Wt::WGoogleMap::Coordinate>::const_iterator itb = points.begin(); itb != points.end(); ++itb)
    {
        bbox.first.setLatitude(  std::min(bbox.first.latitude(),   itb->latitude()));
        bbox.first.setLongitude( std::min(bbox.first.longitude(),  itb->longitude()));
        bbox.second.setLatitude( std::max(bbox.second.latitude(),  itb->latitude()));
        bbox.second.setLongitude(std::max(bbox.second.longitude(), itb->longitude()));
    }
    gmap_->zoomWindow(bbox);
    // draw the legs
    points.clear();
    for(vector<size_t>::const_iterator it = ids.begin(); it != ids.end(); ++it)
    {
        assert(*it < igcfile_.Trackpoints.size());
        inout_igc::Trackpoints::iterator tpit = igcfile_.Trackpoints.begin();
        std::advance(tpit, *it);
        points.push_back(Wt::WGoogleMap::Coordinate(tpit->pos_.lat(), tpit->pos_.lon()));
    }
    gmap_->addPolyline(points, Wt::WColor("#FF0000"), 2, 0.9);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
Wt::WWidget * IgcImportForm::setTurnpointField(Wt::WContainerWidget *parent, const geometry::point_ll_deg &pos, const Location::UseAs usage, const size_t trpid)
{
    static const double distThreshold = 0.3; // km
    // find the waypoint
    vector<shared_ptr<Location> > tmploc;
    remove_copy_if(flightDb_->Locations.begin(), flightDb_->Locations.end(), back_inserter(tmploc),
        !ret<bool>(bind(&Location::usage, *bll::_1) & usage) ||
        bind(&Location::getDistance, *bll::_1, pos) > distThreshold);

    if(tmploc.size())
    {
        sort(tmploc.begin(), tmploc.end(), bind(&Location::getDistance, *bll::_1, pos)
                                         < bind(&Location::getDistance, *bll::_2, pos));
        LocationField *lfWpt = new LocationField(flightDb_, usage);
        lfWpt->setLocation(**tmploc.begin());

        parent->addWidget(lfWpt);

        return lfWpt;
    }
    else
    {
        NewLocationField *nlfWpt = new NewLocationField(flightDb_);
        nlfWpt->fillAreas();
        nlfWpt->setTrackpntIdx(trpid);

        parent->addWidget(nlfWpt);

        return nlfWpt;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void IgcImportForm::addFlight()
{
    try
    {
        assert(vlfWaypoints_.size() >= 2);
        // create a new flight by copying the last flight and modify some fields
        shared_ptr<Flight> newFlight(new Flight(**flightDb_->flights().rbegin()));
        newFlight->setNumber(newFlight->number() + 1);
        newFlight->setDate(igcfile_.Trackpoints.begin()->timestamp_.date());
        newFlight->setDuration(boost::posix_time::seconds((igcfile_.Trackpoints.rbegin()->timestamp_ - igcfile_.Trackpoints.begin()->timestamp_).total_seconds()));
        newFlight->clearWaypoints();
        if(LocationField *locfld = dynamic_cast<LocationField*>(vlfWaypoints_[0]))
            newFlight->setTakeoff(locfld->getLocation());
        else if(NewLocationField *locfld = dynamic_cast<NewLocationField*>(vlfWaypoints_[0]))
        {
            if(!locfld->getLocationName().length())
                throw std::runtime_error("Need a name for the takeoff");
            inout_igc::Trackpoints::iterator tpit = igcfile_.Trackpoints.begin();
            std::advance(tpit, locfld->trackpntIdx());
            shared_ptr<Location> locTakeoff(new Location(locfld->getArea(),
                                                        locfld->getLocationName(),
                                                        tpit->alt_,
                                                        tpit->pos_,
                                                        Location::UA_TAKEOFF));
            flightDb_->addLocation(locTakeoff);
            newFlight->setTakeoff(locTakeoff);
        }
        else
            throw std::runtime_error("Wether existing nor new location for takeoff");

        // landing
        if(LocationField *locfld = dynamic_cast<LocationField*>(vlfWaypoints_[vlfWaypoints_.size()-1]))
            newFlight->setLanding(locfld->getLocation());
        else if(NewLocationField *locfld = dynamic_cast<NewLocationField*>(vlfWaypoints_[vlfWaypoints_.size()-1]))
        {
            if(!locfld->getLocationName().length())
                throw std::runtime_error("Need a name for the landing zone");
            inout_igc::Trackpoints::iterator tpit = igcfile_.Trackpoints.begin();
            std::advance(tpit, locfld->trackpntIdx());
            shared_ptr<Location> locLanding(new Location(locfld->getArea(),
                                                        locfld->getLocationName(),
                                                        tpit->alt_,
                                                        tpit->pos_,
                                                        Location::UA_LANDING));
            flightDb_->addLocation(locLanding);
            newFlight->setLanding(locLanding);
        }
        else
            throw std::runtime_error("Wether existing nor new location for landing");

        // waypoints
        for(size_t i=1; i<vlfWaypoints_.size()-1; ++i)
        {
            if(LocationField *locfld = dynamic_cast<LocationField*>(vlfWaypoints_[i]))
                newFlight->setLanding(locfld->getLocation());
            else if(NewLocationField *locfld = dynamic_cast<NewLocationField*>(vlfWaypoints_[i]))
            {
                if(!locfld->getLocationName().length())
                    throw std::runtime_error("Need a name for the waypoints");
                inout_igc::Trackpoints::iterator tpit = igcfile_.Trackpoints.begin();
                std::advance(tpit, locfld->trackpntIdx());

                shared_ptr<Location> locWpt(new Location(locfld->getArea(),
                                                            locfld->getLocationName(),
                                                            0,
                                                            tpit->pos_,
                                                            Location::UA_WAYPNT));
                flightDb_->addLocation(locWpt);
                newFlight->addWaypoint(locWpt);
            }
            else
                throw std::runtime_error("Wether existing nor new location for landing");
        }

        newFlight->setTrack(igcname_);

        // close the dialog
        accept();

        // edit the new flight
        Wt::WBorderLayout *blyout = dynamic_cast<Wt::WBorderLayout*>(Wt::WApplication::instance()->root()->layout());
        assert(blyout);
        TabControl *tabCtrl = dynamic_cast<TabControl*>(blyout->widgetAt(Wt::WBorderLayout::Center));
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
