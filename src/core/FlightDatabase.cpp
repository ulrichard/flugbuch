// flugbuch
#include "FlightDatabase.h"
#include "FormatStr.h"
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/functional/hash.hpp>
// std lib
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <numeric>

// flugbuch
using namespace flb;
// ggl
using geometry::point_ll_deg;
using geometry::latitude;
using geometry::longitude;
using geometry::dms;
using geometry::south;
using geometry::west;
// boost
using namespace boost::lambda;
using boost::shared_ptr;
using boost::lexical_cast;
using boost::gregorian::date;
using boost::posix_time::time_duration;
using boost::posix_time::minutes;
// standard library
using std::string;
using std::vector;
using std::runtime_error;
using std::remove_copy_if;
using std::remove_if;
using std::copy;
using std::endl;
 /////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::addFlightArea(shared_ptr<FlightArea> flar)
{
	if(find(FlightAreas_.begin(), FlightAreas_.end(), flar) != FlightAreas_.end())
//	BOOST_FOREACH(shared_ptr<FlightArea> fls, FlightAreas_)
//		if(*flar == *fls)
			throw std::invalid_argument("Fluggebiet existiert bereits");
	FlightAreas_.push_back(flar);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::addLocation(shared_ptr<Location> loc)
{
	BOOST_FOREACH(shared_ptr<Location> los, Locations_)
		if(*loc == *los)
			throw std::invalid_argument(("Ort existiert bereits : " + loc->name()).c_str());
	Locations_.push_back(loc);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::addGlider(shared_ptr<Glider> gld)
{
	BOOST_FOREACH(shared_ptr<Glider> gls, Gliders_)
		if(*gld == *gls)
			throw std::invalid_argument("Flugger�t existiert bereits");
	Gliders_.push_back(gld);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::addFlight(shared_ptr<Flight> fl)
{
	BOOST_FOREACH(shared_ptr<Flight> fs, Flights_)
		if(*fl == *fs)
			throw std::invalid_argument("Flugnummer schon vergeben");
	Flights_.insert(fl);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteFlightArea(FlightDatabase::SeqFlightAreas::iterator itdel)
{
	BOOST_FOREACH(shared_ptr<Location> loc, Locations_)
		if(*loc->area() == **itdel)
			throw runtime_error(FormatStr() << "Das Gebiet wird noch verwendet. Zum Beispiel bei Ort " << loc->name());
	FlightAreas_.erase(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteFlightArea(boost::shared_ptr<FlightArea> flar)
{
    SeqFlightAreas::iterator itdel = find(FlightAreas_.begin(), FlightAreas_.end(), flar);
    if(itdel == FlightAreas_.end())
        throw runtime_error("Fluggebiet ist nicht in dieser Datenbank");
    else
        deleteFlightArea(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteLocation(FlightDatabase::SeqLocations::iterator itdel)
{
	static const std::string errmsg("Der Ort wird noch verwendet. Zum Beispiel bei Flug Nummer ");

	BOOST_FOREACH(shared_ptr<Flight> flt, Flights_)
	{
		if(*flt->takeoff() == **itdel)
			throw runtime_error(FormatStr() << errmsg << flt->number() << " als Startplatz");
		if(*flt->landing() == **itdel)
			throw runtime_error(FormatStr() << errmsg << flt->number() << " als Landeplatz");
		BOOST_FOREACH(boost::shared_ptr<Location> wpt, flt->Waypoints)
			if(*wpt == **itdel)
				throw runtime_error(FormatStr() << errmsg << flt->number() << " als Wegpunkt");
	}
	Locations_.erase(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteLocation(boost::shared_ptr<Location> loc)
{
    SeqLocations::iterator itdel = find(Locations_.begin(), Locations_.end(), loc);
    if(itdel == Locations_.end())
        throw runtime_error("Ort ist nicht in dieser Datenbank");
    else
        deleteLocation(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteGlider(FlightDatabase::SeqGliders::iterator itdel)
{
	BOOST_FOREACH(shared_ptr<Flight> flight, Flights_)
		if(*flight->glider() == **itdel)
			throw runtime_error(FormatStr() << "Das Flugger�t wird noch verwendet. Zum Beispiel bei Flug Nummer " << flight->number());
	Gliders_.erase(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteGlider(boost::shared_ptr<Glider> gld)
{
    SeqGliders::iterator itdel = find(Gliders_.begin(), Gliders_.end(), gld);
    if(itdel == Gliders_.end())
        throw runtime_error("Schirm ist nicht in dieser Datenbank");
    else
        deleteGlider(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteFlight(FlightDatabase::SeqFlights::iterator itdel)
{
	Flights_.erase(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteFlight(boost::shared_ptr<Flight> fl)
{
    SeqFlights::iterator itdel = find(Flights_.begin(), Flights_.end(), fl);
    if(itdel == Flights_.end())
        throw runtime_error("Flug ist nicht in dieser Datenbank");
    else
        deleteFlight(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
const FlightDatabase::SeqFlightAreas FlightDatabase::getFlightAreasEx(Location::UseAs use) const
{
    std::set<shared_ptr<FlightArea> > selset;
    BOOST_FOREACH(shared_ptr<Location> loc, Locations_)
        if(loc->usage() & use)
            selset.insert(loc->area());

    SeqFlightAreas sel;
    copy(selset.begin(), selset.end(), back_inserter(sel));
    return sel;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
const FlightDatabase::SeqLocations FlightDatabase::getLocationsEx(const shared_ptr<FlightArea> flar, Location::UseAs useas) const
{
    SeqLocations sel;
    remove_copy_if(Locations_.begin(), Locations_.end(), back_inserter(sel), bind(&Location::area, *_1) != flar);
    sel.erase(remove_if(sel.begin(), sel.end(), !(bind(&Location::usage, *_1) & static_cast<int>(useas))), sel.end());
    return sel;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
const boost::shared_ptr<FlightArea> FlightDatabase::getArea(const std::string &nam) const
{
    SeqFlightAreas::const_iterator fit = find_if(FlightAreas_.begin(), FlightAreas_.end(), bind(&FlightArea::name, *_1) == nam);
    if(fit != FlightAreas_.end())
        return *fit;
    throw std::runtime_error("no FlightArea found with name" + nam);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
shared_ptr<FlightDatabase> FlightDatabase::makeTestDb(void)
{
    shared_ptr<FlightDatabase> fldb(new flb::FlightDatabase("*testdb*"));
	// flight areas
	shared_ptr<flb::FlightArea> arRothenflue(new flb::FlightArea("Rothenflue", "Schweiz", "Das Baehnli faehrt leider nicht mehr."));
	fldb->addFlightArea(arRothenflue);
	shared_ptr<flb::FlightArea> arFronalpstock(new flb::FlightArea("Fronalpstock", "Schweiz", "Schoene Aussicht."));
	fldb->addFlightArea(arFronalpstock);
	shared_ptr<flb::FlightArea> arSchwyz(new flb::FlightArea("Schwyz", "Schweiz", "Zentralschweiz"));
	fldb->addFlightArea(arSchwyz);
	shared_ptr<flb::FlightArea> arSteinen(new flb::FlightArea("Steinen", "Schweiz", "am Lauerzersee"));
	fldb->addFlightArea(arSteinen);
	shared_ptr<flb::FlightArea> arLauchern(new flb::FlightArea("Lauchernalp", "Schweiz", "Loetschental im Wallis"));
	fldb->addFlightArea(arLauchern);
	shared_ptr<flb::FlightArea> arEngelberg(new flb::FlightArea("Engelberg", "Schweiz", "Obwalden"));
	fldb->addFlightArea(arEngelberg);
	shared_ptr<flb::FlightArea> arStoos(new flb::FlightArea("Stoos", "Schweiz", "Schwyz"));
	fldb->addFlightArea(arStoos);
	shared_ptr<flb::FlightArea> arEngelstock(new flb::FlightArea("Engelstock", "Schweiz", "Schwyz"));
	fldb->addFlightArea(arEngelstock);
	shared_ptr<flb::FlightArea> arFamatina(new flb::FlightArea("Famatina", "Argentinien", "vor-Anden"));
	fldb->addFlightArea(arFamatina);
	// locations
	shared_ptr<flb::Location> locWeststart(new flb::Location(arRothenflue, "Westwindstartplatz", 1540, point_ll_deg(latitude<>(47.018337296086003), longitude<>(8.7019931164065607)), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locWeststart);
	shared_ptr<flb::Location> locSteisteg(new flb::Location(arSchwyz, "Steisteg", 485, point_ll_deg(latitude<>(47.018877777788397), longitude<>(8.6518888889418708)), flb::Location::UA_LANDING));
	fldb->addLocation(locSteisteg);
	shared_ptr<flb::Location> locFronalpstock(new flb::Location(arFronalpstock, "Fronalpstock", 1920, point_ll_deg(latitude<>(46.9650366568072), longitude<>(8.6389761374447307)), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locFronalpstock);
	shared_ptr<flb::Location> locBietstoeckli(new flb::Location(arFronalpstock, "Bietstoeckli", 1860, point_ll_deg(latitude<>(46.9650366568072), longitude<>(8.6389761374447307)), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locBietstoeckli);
	shared_ptr<flb::Location> locStoosSki(new flb::Location(arStoos, "Skischule", 1325, point_ll_deg(latitude<>(46.977499999999999), longitude<>(8.6627777777777801)), flb::Location::UA_LANDING));
	fldb->addLocation(locStoosSki);
	shared_ptr<flb::Location> locFrauholz(new flb::Location(arSteinen, "Frauholz", 465, point_ll_deg(latitude<>(47.045461111068697), longitude<>(8.6064027775658491)), flb::Location::UA_LANDING));
	fldb->addLocation(locFrauholz);
	shared_ptr<flb::Location> locKlMythen(new flb::Location(arRothenflue, "kleiner Mythen", 1811, point_ll_deg(latitude<>(47.041024816277201), longitude<>(8.6839721295372101)), flb::Location::UA_TAKEOFF | flb::Location::UA_WAYPNT));
	fldb->addLocation(locKlMythen);
	shared_ptr<flb::Location> locZwMythen(new flb::Location(arRothenflue, "Zwischenmythen", 1375, point_ll_deg(latitude<>(47.034203693080499), longitude<>(8.68249797236105)), flb::Location::UA_WAYPNT));
	fldb->addLocation(locZwMythen);
	shared_ptr<flb::Location> locEngelstock(new flb::Location(arEngelstock, "Engelstock", 1210, point_ll_deg(latitude<>(47.063547222349399), longitude<>(8.6473694441053599)), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locEngelstock);
	shared_ptr<flb::Location> locBormPoint(new flb::Location(arSchwyz, "Borm Point", 600, point_ll_deg(latitude<>(47.0475), longitude<>(8.6294)), flb::Location::UA_LANDING));
	fldb->addLocation(locBormPoint);
	shared_ptr<flb::Location> locHockenhorn(new flb::Location(arLauchern, "HockenhornGrat", 3111, point_ll_deg(latitude<>(46.42901), longitude<>(7.7472)), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locHockenhorn);
	shared_ptr<flb::Location> locWiler(new flb::Location(arLauchern, "Wiler", 1000, point_ll_deg(latitude<>(46.4003), longitude<>(7.777)), flb::Location::UA_LANDING));
	fldb->addLocation(locWiler);
	shared_ptr<flb::Location> locBrunni(new flb::Location(arEngelberg, "Brunni", 1600, point_ll_deg(latitude<>(46.84), longitude<>(8.4053)), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locBrunni);
	shared_ptr<flb::Location> locOchsenmatte(new flb::Location(arEngelberg, "Ochsenmatte", 1100, point_ll_deg(latitude<>(46.819), longitude<>(8.416)), flb::Location::UA_LANDING));
	fldb->addLocation(locOchsenmatte);
	shared_ptr<flb::Location> locFamatina(new flb::Location(arFamatina, "Famatina", 1685, point_ll_deg(latitude<>(-28.9034), longitude<>(-67.4960)), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locFamatina);
	shared_ptr<flb::Location> locFamaLande(new flb::Location(arFamatina, "CHA154", 1100, point_ll_deg(latitude<>(-28.5819), longitude<>(-67.5928)), flb::Location::UA_LANDING));
	fldb->addLocation(locFamaLande);
	shared_ptr<flb::Location> locFamaWp1(new flb::Location(arFamatina, "Pituil", 0, point_ll_deg(latitude<>(dms<south>(28, 34, 49.2)), longitude<>(dms<west>(68, 30, 44.2))), flb::Location::UA_WAYPNT));
	fldb->addLocation(locFamaWp1);
	shared_ptr<flb::Location> locFamaWp2(new flb::Location(arFamatina, "Campanas", 0, point_ll_deg(latitude<>(dms<south>(28, 32, 38)), longitude<>(dms<west>(68, 22, 36.1))), flb::Location::UA_WAYPNT));
	fldb->addLocation(locFamaWp2);
	// gliders
	shared_ptr<flb::Glider> glMagus4(new flb::Glider("MacPara", "Magus 4", "25", "yellow", 2006, "comp") );
	fldb->addGlider(glMagus4);
	shared_ptr<flb::Glider> glMagus5(new flb::Glider("MacPara", "Magus 5", "26", "grey orange", 2007, "comp") );
	fldb->addGlider(glMagus5);
	shared_ptr<flb::Glider> glHellracer(new flb::Glider("JN", "Hellracer", "15", "black red yellow", 2008, "speed") );
	fldb->addGlider(glHellracer);
	shared_ptr<flb::Glider> glU4Sport(new flb::Glider("Aircross", "Ultima Sport", "23", "black orange", 2008, "LTF 2-3") );
	fldb->addGlider(glU4Sport);
	shared_ptr<flb::Glider> glAspen(new flb::Glider("Gradient", "Aspen", "26", "red", 2003, "DHV 2") );
	fldb->addGlider(glAspen);
	shared_ptr<flb::Glider> glBiGolden(new flb::Glider("Gradient", "Bi Golden", "42", "blue", 2004, "DHV 1-2 Bi") );
	fldb->addGlider(glBiGolden);
	// flights
	vector<shared_ptr<flb::Location> > wptsFamatina;
	wptsFamatina.push_back(locFamaWp1);
	wptsFamatina.push_back(locFamaWp2);
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(1,  date(2007,  3, 22), minutes(120), glMagus4, locFamatina, locFamaLande, "Campeonato Argentino -> Tagessieg", wptsFamatina)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(2,  date(2008,  9, 10), minutes( 15), glMagus5, locWeststart, locSteisteg)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(3,  date(2008,  9, 26), minutes(  5), glHellracer, locBietstoeckli, locSteisteg)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(4,  date(2008,  9, 27), minutes( 25), glMagus5, locBietstoeckli, locFrauholz)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(5,  date(2008, 10, 12), minutes(  5), glHellracer, locKlMythen, locSteisteg)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(6,  date(2008, 10, 20), minutes( 30), glU4Sport, locWeststart, locSteisteg)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(7,  date(2008, 12,  7), minutes(  5), glHellracer, locHockenhorn, locWiler)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(8,  date(2008, 12,  7), minutes(  5), glHellracer, locHockenhorn, locWiler)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(9,  date(2008, 12,  7), minutes(  5), glHellracer, locHockenhorn, locWiler)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(10, date(2009,  1, 10), minutes( 15), glBiGolden,  locBrunni, locOchsenmatte)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(11, date(2009,  1, 11), minutes(  5), glAspen, locFronalpstock, locStoosSki)));
	fldb->addFlight(shared_ptr<flb::Flight>(new flb::Flight(12, date(2009,  1, 15), minutes( 10), glMagus5, locEngelstock, locBormPoint)));

    for(FlightDatabase::SeqFlights::iterator it = fldb->flights().begin(); it != fldb->flights().end(); ++it)
        (*it)->setDistance((*it)->calcDistance());

	return fldb;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::printCounts(std::ostream &strm)
{
    strm << "pilot name : "   << PilotName_.size()   << endl;
	strm << "flight areas : " << FlightAreas_.size() << endl;
	strm << "locations : "    << Locations_.size()   << endl;
	strm << "gliders : " <<	Gliders_.size() << endl;
	strm << "flights : " << Flights_.size() << endl;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
string FlightDatabase::encryptPassword(const string &pwd)
{
    // todo : use an md5 or some other hashing algorithm here
    int sum = std::accumulate(pwd.begin(), pwd.end(), 0);
    int has = boost::hash<string>()(pwd);

    string pwh = lexical_cast<string>(sum) + lexical_cast<string>(has);
    return pwh;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::setPilotNameAndPwd(const string &usr, const string &pwd)
{
    PilotName_ = usr;
    Password_  = encryptPassword(pwd);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

