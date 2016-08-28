// flugbuch
#include "FlightDatabase.h"
#include "../util/FormatStr.h"
// boost
#include <boost/lexical_cast.hpp>
#include <boost/functional/hash.hpp>
#include <boost/range/algorithm.hpp>
// std lib
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <numeric>

// flugbuch
using namespace flb;
/*
// ggl
using geometry::point_ll_deg;
using boost::geometry::latitude;
using boost::geometry::longitude;
using boost::geometry::dms;
using boost::geometry::north;
using boost::geometry::south;
using boost::geometry::east;
using boost::geometry::west;
*/
// boost
using boost::gregorian::date;
using boost::posix_time::time_duration;
using boost::posix_time::minutes;
namespace brng = boost::range;
// standard library

typedef std::pair<double, double> point_ll_deg;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::addFlightArea(shared_ptr<FlightArea> flar)
{
	if(brng::find(FlightAreas_, flar) != FlightAreas_.end())
		throw std::invalid_argument("Fluggebiet existiert bereits");

	FlightAreas_.push_back(flar);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::addLocation(shared_ptr<Location> loc)
{
	for(const auto& los : Locations_)
		if(*loc == *los)
			throw std::invalid_argument(("Ort existiert bereits : " + loc->name()).c_str());

	Locations_.push_back(loc);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::addGlider(shared_ptr<Glider> gld)
{
	for(const auto& gls : Gliders_)
		if(*gld == *gls)
			throw std::invalid_argument("Flugger�t existiert bereits");

	Gliders_.push_back(gld);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::addFlight(shared_ptr<Flight> fl)
{
	for(const auto& fs : Flights_)
		if(*fl == *fs)
			throw std::invalid_argument("Flugnummer schon vergeben");

	Flights_.insert(fl);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteFlightArea(FlightDatabase::SeqFlightAreas::iterator itdel)
{
	for(const auto& loc : Locations_)
		if(*loc->area() == **itdel)
			throw std::runtime_error(FormatStr() << "Das Gebiet wird noch verwendet. Zum Beispiel bei Ort " << loc->name());

	FlightAreas_.erase(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteFlightArea(boost::shared_ptr<FlightArea> flar)
{
    const auto itdel = brng::find(FlightAreas_, flar);
    if(itdel == FlightAreas_.end())
        throw std::runtime_error("Fluggebiet ist nicht in dieser Datenbank");
    else
        deleteFlightArea(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteLocation(FlightDatabase::SeqLocations::iterator itdel)
{
	static const std::string errmsg("Der Ort wird noch verwendet. Zum Beispiel bei Flug Nummer ");

	for(const auto& flt : Flights_)
	{
		if(*flt->takeoff() == **itdel)
			throw std::runtime_error(FormatStr() << errmsg << flt->number() << " als Startplatz");
		if(*flt->landing() == **itdel)
			throw std::runtime_error(FormatStr() << errmsg << flt->number() << " als Landeplatz");
		for(const auto& wpt : flt->Waypoints)
			if(*wpt == **itdel)
				throw std::runtime_error(FormatStr() << errmsg << flt->number() << " als Wegpunkt");
	}

	Locations_.erase(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteLocation(boost::shared_ptr<Location> loc)
{
    const auto itdel = brng::find(Locations_, loc);
    if(itdel == Locations_.end())
        throw std::runtime_error("Ort ist nicht in dieser Datenbank");
    else
        deleteLocation(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteGlider(FlightDatabase::SeqGliders::iterator itdel)
{
	for(const auto& flight : Flights_)
		if(*flight->glider() == **itdel)
			throw std::runtime_error(FormatStr() << "Das Fluggerät wird noch verwendet. Zum Beispiel bei Flug Nummer " << flight->number());

	Gliders_.erase(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::deleteGlider(boost::shared_ptr<Glider> gld)
{
    const auto itdel = brng::find(Gliders_, gld);
    if(itdel == Gliders_.end())
        throw std::runtime_error("Schirm ist nicht in dieser Datenbank");
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
    const auto itdel = brng::find(Flights_, fl);
    if(itdel == Flights_.end())
        throw std::runtime_error("Flug ist nicht in dieser Datenbank");
    else
        deleteFlight(itdel);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
const FlightDatabase::SeqFlightAreas FlightDatabase::getFlightAreasEx(Location::UseAs use) const
{
    std::set<shared_ptr<FlightArea>> selset;
    for(auto& loc : Locations_)
        if(loc->usage() & use)
            selset.insert(loc->area());

    SeqFlightAreas sel;
    brng::copy(selset, back_inserter(sel));

    return sel;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
const FlightDatabase::SeqLocations FlightDatabase::getLocationsEx(const shared_ptr<FlightArea> flar, Location::UseAs useas) const
{
    SeqLocations sel;

    for(auto& loc : Locations_)
    {
        if(loc->area() != flar)
            continue;
        if((loc->usage() & static_cast<int>(useas)) == 0)
            continue;
        sel.push_back(loc);
    }

    return sel;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
const boost::shared_ptr<FlightArea> FlightDatabase::getArea(const std::string &nam) const
{
    const auto fit = brng::find_if(FlightAreas_, [&nam](const boost::shared_ptr<FlightArea>& area)
        { return area->name() == nam; });
    if(fit != FlightAreas_.end())
        return *fit;
    throw std::runtime_error("no FlightArea found with name" + nam);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
shared_ptr<FlightDatabase> FlightDatabase::makeTestDb(void)
{
    boost::shared_ptr<FlightDatabase> fldb(new flb::FlightDatabase("*testdb*"));
	// flight areas
	boost::shared_ptr<flb::FlightArea> arRothenflue(new flb::FlightArea("Rothenflue", "Schweiz", "Das Baehnli faehrt leider nicht mehr."));
	fldb->addFlightArea(arRothenflue);
	boost::shared_ptr<flb::FlightArea> arFronalpstock(new flb::FlightArea("Fronalpstock", "Schweiz", "Schoene Aussicht."));
	fldb->addFlightArea(arFronalpstock);
	boost::shared_ptr<flb::FlightArea> arSchwyz(new flb::FlightArea("Schwyz", "Schweiz", "Zentralschweiz"));
	fldb->addFlightArea(arSchwyz);
	boost::shared_ptr<flb::FlightArea> arSteinen(new flb::FlightArea("Steinen", "Schweiz", "am Lauerzersee"));
	fldb->addFlightArea(arSteinen);
	boost::shared_ptr<flb::FlightArea> arLauchern(new flb::FlightArea("Lauchernalp", "Schweiz", "Loetschental im Wallis"));
	fldb->addFlightArea(arLauchern);
	boost::shared_ptr<flb::FlightArea> arEngelberg(new flb::FlightArea("Engelberg", "Schweiz", "Obwalden"));
	fldb->addFlightArea(arEngelberg);
	boost::shared_ptr<flb::FlightArea> arStoos(new flb::FlightArea("Stoos", "Schweiz", "Schwyz"));
	fldb->addFlightArea(arStoos);
	boost::shared_ptr<flb::FlightArea> arEngelstock(new flb::FlightArea("Engelstock", "Schweiz", "Schwyz"));
	fldb->addFlightArea(arEngelstock);
	boost::shared_ptr<flb::FlightArea> arFamatina(new flb::FlightArea("Famatina", "Argentinien", "vor-Anden"));
	fldb->addFlightArea(arFamatina);
	boost::shared_ptr<flb::FlightArea> arMunGyeong(new flb::FlightArea("MunGyeong", "Sued Korea", "Asien"));
	fldb->addFlightArea(arMunGyeong);
	// locations
	boost::shared_ptr<flb::Location> locWeststart(new flb::Location(arRothenflue, "Westwindstartplatz", 1540, point_ll_deg(47.018337296086003, 8.7019931164065607), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locWeststart);
	boost::shared_ptr<flb::Location> locSteisteg(new flb::Location(arSchwyz, "Steisteg", 485, point_ll_deg(47.018877777788397, 8.6518888889418708), flb::Location::UA_LANDING));
	fldb->addLocation(locSteisteg);
	boost::shared_ptr<flb::Location> locFronalpstock(new flb::Location(arFronalpstock, "Fronalpstock", 1920, point_ll_deg(46.9650366568072, 8.6389761374447307), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locFronalpstock);
	boost::shared_ptr<flb::Location> locBietstoeckli(new flb::Location(arFronalpstock, "Bietstoeckli", 1860, point_ll_deg(46.9650366568072, 8.6389761374447307), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locBietstoeckli);
	boost::shared_ptr<flb::Location> locStoosSki(new flb::Location(arStoos, "Skischule", 1325, point_ll_deg(46.977499999999999, 8.6627777777777801), flb::Location::UA_LANDING));
	fldb->addLocation(locStoosSki);
	boost::shared_ptr<flb::Location> locFrauholz(new flb::Location(arSteinen, "Frauholz", 465, point_ll_deg(47.045461111068697, 8.6064027775658491), flb::Location::UA_LANDING));
	fldb->addLocation(locFrauholz);
	boost::shared_ptr<flb::Location> locKlMythen(new flb::Location(arRothenflue, "kleiner Mythen", 1811, point_ll_deg(47.041024816277201, 8.6839721295372101), flb::Location::UA_TAKEOFF | flb::Location::UA_WAYPNT));
	fldb->addLocation(locKlMythen);
	boost::shared_ptr<flb::Location> locZwMythen(new flb::Location(arRothenflue, "Zwischenmythen", 1375, point_ll_deg(47.034203693080499, 8.68249797236105), flb::Location::UA_WAYPNT));
	fldb->addLocation(locZwMythen);
	boost::shared_ptr<flb::Location> locEngelstock(new flb::Location(arEngelstock, "Engelstock", 1210, point_ll_deg(47.063547222349399, 8.6473694441053599), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locEngelstock);
	boost::shared_ptr<flb::Location> locBormPoint(new flb::Location(arSchwyz, "Borm Point", 600, point_ll_deg(47.0475, 8.6294), flb::Location::UA_LANDING));
	fldb->addLocation(locBormPoint);
	boost::shared_ptr<flb::Location> locHockenhorn(new flb::Location(arLauchern, "HockenhornGrat", 3111, point_ll_deg(46.42901, 7.7472), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locHockenhorn);
	boost::shared_ptr<flb::Location> locWiler(new flb::Location(arLauchern, "Wiler", 1000, point_ll_deg(46.4003, 7.777), flb::Location::UA_LANDING));
	fldb->addLocation(locWiler);
	boost::shared_ptr<flb::Location> locBrunni(new flb::Location(arEngelberg, "Brunni", 1600, point_ll_deg(46.84, 8.4053), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locBrunni);
	boost::shared_ptr<flb::Location> locOchsenmatte(new flb::Location(arEngelberg, "Ochsenmatte", 1100, point_ll_deg(46.819, 8.416), flb::Location::UA_LANDING));
	fldb->addLocation(locOchsenmatte);
	boost::shared_ptr<flb::Location> locFamatina(new flb::Location(arFamatina, "Famatina", 1685, point_ll_deg(-28.9034, -67.4960), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locFamatina);
	boost::shared_ptr<flb::Location> locFamaLande(new flb::Location(arFamatina, "CHA154", 1100, point_ll_deg(-28.5819, -67.5928), flb::Location::UA_LANDING));
	fldb->addLocation(locFamaLande);
	boost::shared_ptr<flb::Location> locFamaWp1(new flb::Location(arFamatina, "Pituil", 0, point_ll_deg(-28.580333, -67.487722), flb::Location::UA_WAYPNT));
	fldb->addLocation(locFamaWp1);
	boost::shared_ptr<flb::Location> locFamaWp2(new flb::Location(arFamatina, "Campanas", 0, point_ll_deg(-28.543888, -67.6233055), flb::Location::UA_WAYPNT));
	fldb->addLocation(locFamaWp2);
	boost::shared_ptr<flb::Location> locMunGyTo(new flb::Location(arMunGyeong, "Mun Gyeong", 985, point_ll_deg(36.7299166, 128.17425), flb::Location::UA_TAKEOFF));
	fldb->addLocation(locMunGyTo);
	boost::shared_ptr<flb::Location> locMunGyARA(new flb::Location(arMunGyeong, "ARA", 90, point_ll_deg(36.570222, 128.2911388), flb::Location::UA_LANDING));
	fldb->addLocation(locMunGyARA);
	boost::shared_ptr<flb::Location> locMunGyWP1(new flb::Location(arMunGyeong, "MGWP1", 0, point_ll_deg(36.785055, 128.21), flb::Location::UA_WAYPNT));
	fldb->addLocation(locMunGyWP1);
	boost::shared_ptr<flb::Location> locMunGyWP2(new flb::Location(arMunGyeong, "MGWP2", 0, point_ll_deg(36.667, 128.11666), flb::Location::UA_WAYPNT));
	fldb->addLocation(locMunGyWP2);
	boost::shared_ptr<flb::Location> locMunGyWP3(new flb::Location(arMunGyeong, "MGWP3", 0, point_ll_deg(36.56122, 128.232), flb::Location::UA_WAYPNT));
	fldb->addLocation(locMunGyWP3);
	// gliders
	boost::shared_ptr<flb::Glider> glMagus4(new flb::Glider("MacPara", "Magus 4", "25", "yellow", 2006, "comp") );
	fldb->addGlider(glMagus4);
	boost::shared_ptr<flb::Glider> glMagus5(new flb::Glider("MacPara", "Magus 5", "26", "grey orange", 2007, "comp") );
	fldb->addGlider(glMagus5);
	boost::shared_ptr<flb::Glider> glHellracer(new flb::Glider("JN", "Hellracer", "15", "black red yellow", 2008, "speed") );
	fldb->addGlider(glHellracer);
	boost::shared_ptr<flb::Glider> glU4Sport(new flb::Glider("Aircross", "Ultima Sport", "23", "black orange", 2008, "LTF 2-3") );
	fldb->addGlider(glU4Sport);
	boost::shared_ptr<flb::Glider> glAspen(new flb::Glider("Gradient", "Aspen", "26", "red", 2003, "DHV 2") );
	fldb->addGlider(glAspen);
	boost::shared_ptr<flb::Glider> glBiGolden(new flb::Glider("Gradient", "Bi Golden", "42", "blue", 2004, "DHV 1-2 Bi") );
	fldb->addGlider(glBiGolden);
	// flights
	std::vector<boost::shared_ptr<flb::Location> > wptsFamatina;
	wptsFamatina.push_back(locFamaWp1);
	wptsFamatina.push_back(locFamaWp2);
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(1,  date(2007,  3, 22), minutes(120), glMagus4, locFamatina, locFamaLande, "Campeonato Argentino -> Tagessieg", wptsFamatina)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(2,  date(2008,  9, 10), minutes( 15), glMagus5, locWeststart, locSteisteg)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(3,  date(2008,  9, 26), minutes(  5), glHellracer, locBietstoeckli, locSteisteg)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(4,  date(2008,  9, 27), minutes( 25), glMagus5, locBietstoeckli, locFrauholz)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(5,  date(2008, 10, 12), minutes(  5), glHellracer, locKlMythen, locSteisteg)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(6,  date(2008, 10, 20), minutes( 30), glU4Sport, locWeststart, locSteisteg)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(7,  date(2008, 12,  7), minutes(  5), glHellracer, locHockenhorn, locWiler)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(8,  date(2008, 12,  7), minutes(  5), glHellracer, locHockenhorn, locWiler)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(9,  date(2008, 12,  7), minutes(  5), glHellracer, locHockenhorn, locWiler)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(10, date(2009,  1, 10), minutes( 15), glBiGolden,  locBrunni, locOchsenmatte)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(11, date(2009,  1, 11), minutes(  5), glAspen, locFronalpstock, locStoosSki)));
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(12, date(2009,  1, 15), minutes( 10), glMagus5, locEngelstock, locBormPoint)));
	std::vector<boost::shared_ptr<flb::Location> > wptsMunGyeong;
	wptsMunGyeong.push_back(locMunGyWP1);
	wptsMunGyeong.push_back(locMunGyWP2);
	wptsMunGyeong.push_back(locMunGyWP3);
	fldb->addFlight(boost::shared_ptr<flb::Flight>(new flb::Flight(13, date(2009,  5,  1), minutes(160), glMagus5, locMunGyTo, locMunGyARA, "PWC", wptsMunGyeong)));

    for(auto& flight : fldb->flights())
        flight->setDistance(flight->calcDistance());

	return fldb;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::printCounts(std::ostream &strm)
{
    strm << "pilot name : "   << PilotName_.size()   << std::endl;
	strm << "flight areas : " << FlightAreas_.size() << std::endl;
	strm << "locations : "    << Locations_.size()   << std::endl;
	strm << "gliders : " <<	Gliders_.size() << std::endl;
	strm << "flights : " << Flights_.size() << std::endl;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
std::string FlightDatabase::encryptPassword(const std::string &pwd)
{
    // todo : use an md5 or some other hashing algorithm here
    int sum = std::accumulate(pwd.begin(), pwd.end(), 0);
    int has = boost::hash<std::string>()(pwd);

    std::string pwh = boost::lexical_cast<std::string>(sum) + boost::lexical_cast<std::string>(has);

    return pwh;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void FlightDatabase::setPilotNameAndPwd(const std::string &usr, const std::string &pwd)
{
    PilotName_ = usr;
    Password_  = encryptPassword(pwd);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

