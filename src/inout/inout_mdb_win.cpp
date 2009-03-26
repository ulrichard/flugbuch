// flugbuch
#include "inout_mdb_win.h"
#include "SystemInformation.h"
#include "FormatStr.h"
// mdb odbc
#define DTL_NO_UNICODE
#include "DTL.h"
// boost
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem/fstream.hpp>
// standard library
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

using namespace flb;
//using namespace boost::lambda;
using std::string;
using std::vector;
using std::map;
using std::copy;
using std::back_inserter;
using std::pair;
using std::make_pair;
using std::count;
using boost::lexical_cast;
using boost::bind;
using boost::ref;
namespace bfs = boost::filesystem;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// db definitions
/*
DTL_TABLE3(Laender,
    int,    LandId,
    string, Name,
    string, Kontinent,
);

DTL_TABLE4(Fluggebiete,
    int,    FluggebietId,
    int,    LandId,
    string, Name,
    string, Beschreibung,
);

DTL_TABLE8(Gleitschirme,
    int,    GleitschirmId,
    string, Hersteller,
    string, Modell,
    string, Groesse,
    string, Farbe,
    string, Klassierung,
    int,    Jahrgang,
    string, Bemerkungen,
);

DTL_TABLE8(Startplaetze,
    int,    StartplatzId,
    int,    FluggebietId,
    string, Name,
    string, Beschreibung,
    int,    Hoehe,
    double, Laenge,
    double, Breite,
    string, Bemerkungen,
);

DTL_TABLE8(Landeplaetze,
    int,    LandeplatzId,
    int,    FluggebietId,
    string, Name,
    string, Beschreibung,
    int,    Hoehe,
    double, Laenge,
    double, Breite,
    string, Bemerkungen,
);

DTL_TABLE8(Wegpunkte,
    int,    WegpunktId,
    int,    FluggebietId,
    string, Name,
    string, Beschreibung,
    int,    Hoehe,
    double, Laenge,
    double, Breite,
    string, Bemerkungen,
);

DTL_TABLE3(lnkWegpFluege,
    int, LnkId,
    int, FlugId,
    int, WegpunktId
);

DTL_TABLE8(Fluege,
    int,    FlugId,
    int,    FlugNr,
    date,   Datum,
    int,    GleitschirmId,
    int,    StartplatzId,
    int,    LandeplatzId,
    int,    Flugminuten,
    string, Bemerkungen,
);
*/
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
FlightDatabase inout_mdb::read(const bfs::path &source)
{
	FlightDatabase fldb("");

    try
    {
        string connectstr = FormatStr() << "Driver={Microsoft Access Driver (*.mdb)}; Dbq=" << source.external_file_string() << ";";
        dtl::DBConnection::GetDefaultConnection().Connect(connectstr);

        // the countries are read for reference
        dtl::DynamicDBView<> viewCountry("Laender", "*");
        for(dtl::DynamicDBView<>::select_iterator it = viewCountry.begin(); it != viewCountry.end(); ++it)
            countries_[(*it)["LandId"]] = string((*it)["Name"]); // we omit the description of the country

        // the flight areas can be used as is
        dtl::DynamicDBView<> viewArea("Fluggebiete", "*");
        transform(viewArea.begin(), viewArea.end(), inserter(areas_, areas_.end()), bind(&inout_mdb::GetArea, this, _1));
//		for_each(areas_.begin(), areas_.end(), bind(&FlightDatabase::addFlightArea, fldb, ret<shared_ptr<FlightArea> >(bind(&pair<unsigned int, shared_ptr<FlightArea> >::second, _1))));
		for(map<unsigned int, shared_ptr<FlightArea> >::iterator it = areas_.begin(); it != areas_.end(); ++it)
			fldb.addFlightArea(it->second);

        // the gliders can be used as is
        dtl::DynamicDBView<> viewGlider("Gleitschirme", "*");
        transform(viewGlider.begin(), viewGlider.end(), inserter(gliders_, gliders_.end()), bind(&inout_mdb::GetGlider, this, _1));
        for(map<unsigned int, shared_ptr<Glider> >::iterator it = gliders_.begin(); it != gliders_.end(); ++it)
            fldb.addGlider(it->second);

        // we have to consolidate the locations
        dtl::DynamicDBView<> viewTakeoff("Startplaetze", "*");
        transform(viewTakeoff.begin(), viewTakeoff.end(), inserter(takeoffs_, takeoffs_.end()), bind(&inout_mdb::GetLocation, this, _1, "StartplatzId"));
        dtl::DynamicDBView<> viewLanding("Landeplaetze", "*");
        transform(viewLanding.begin(), viewLanding.end(), inserter(landings_, landings_.end()), bind(&inout_mdb::GetLocation, this, _1, "LandeplatzId"));
        dtl::DynamicDBView<> viewWaypnt("Wegpunkte", "*");
        transform(viewWaypnt.begin(), viewWaypnt.end(), inserter(waypoints_, waypoints_.end()), bind(&inout_mdb::GetLocation, this, _1, "WegpunktId"));
        dtl::DynamicDBView<> viewLink("lnkWegpFluege", "*");
        for(dtl::DynamicDBView<>::select_iterator it = viewLink.begin(); it != viewLink.end(); ++it)
            wptlinks_[(*it)["LnkId"]] = make_pair((*it)["FlugId"], (*it)["WegpunktId"]);
        for_each(takeoffs_.begin(),  takeoffs_.end(),  bind(&inout_mdb::consolidateLocation, this, _1, Location::UA_TAKEOFF, ref(fldb)));
        for_each(landings_.begin(),  landings_.end(),  bind(&inout_mdb::consolidateLocation, this, _1, Location::UA_LANDING, ref(fldb)));
        for_each(waypoints_.begin(), waypoints_.end(), bind(&inout_mdb::consolidateLocation, this, _1, Location::UA_WAYPNT,  ref(fldb)));

        // flights
        dtl::DynamicDBView<> viewFlight("Fluege", "*");
        transform(viewFlight.begin(), viewFlight.end(), inserter(flights_, flights_.end()), bind(&inout_mdb::GetFlight, this, _1));
        for(map<unsigned int, shared_ptr<Flight> >::iterator it = flights_.begin(); it != flights_.end(); ++it)
            fldb.addFlight(it->second);

        // find gaps in the flight number sequence, and calculate the flown distance
        size_t flnr = 1;
        for(FlightDatabase::SeqFlights::iterator it = fldb.flights().begin(); it != fldb.flights().end(); ++it, ++flnr)
        {
            (*it)->setDistance((*it)->calcDistance());
            if((*it)->number() != flnr)
                (*it)->setNumber(flnr);
        }
        std::cout << "read source sucesfully : " << std::endl;
        fldb.printCounts(std::cout);
    }
    catch(std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }

	return fldb;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<unsigned int, shared_ptr<FlightArea> > inout_mdb::GetArea(const dtl::variant_row &row)
{
	const string name = normalize_str(row["Name"]);
    shared_ptr<FlightArea> flar(new FlightArea(name, countries_[row["LandId"]], row["Beschreibung"]));
    return make_pair(static_cast<int>(row["FluggebietId"]), flar);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<unsigned int, shared_ptr<Glider> > inout_mdb::GetGlider(const dtl::variant_row &row)
{
    shared_ptr<Glider> gld(new Glider(row["Hersteller"],    // brand
                                      row["Modell"],        // model
                                      row["Groesse"],       // size
                                      row["Farbe"],         // color
                                      row["Jahrgang"],      // year
                                      row["Klassierung"],   // classification
                                      row["Bemerkungen"])); // description
    return make_pair(static_cast<int>(row["GleitschirmId"]), gld);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<unsigned int, boost::shared_ptr<Location> > inout_mdb::GetLocation(const dtl::variant_row &row, const string &idname)
{
    shared_ptr<Location> loc(new Location(areas_[row["FluggebietId"]], // area
                                          row["Name"],                 // name
                                          row["Hoehe"],               // height
                                          row["Breite"],              // latitude
                                          row["Laenge"],              // longitude
                                          0));                         // usage will be assigned later
    return make_pair(static_cast<int>(row[idname]), loc);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<unsigned int, boost::shared_ptr<Flight> > inout_mdb::GetFlight(const dtl::variant_row &row)
{
	const unsigned int flId = row["FlugId"];

	vector<shared_ptr<Location> > wpts;
    for(map<unsigned int, pair<unsigned int, unsigned int> >::iterator it = wptlinks_.begin(); it != wptlinks_.end(); ++it)
    {
        if(it->second.first == flId)
        {
            shared_ptr<Location> loc = waypoints_[it->second.second];
            loc->addUsage(Location::UA_WAYPNT);
            wpts.push_back(loc);
        }
    }

	tagTIMESTAMP_STRUCT tms = row["Datum"];
 	boost::gregorian::date bgdate(tms.year, tms.month, tms.day);

    shared_ptr<Flight> flt(new Flight(row["FlugNr"],                  // flight number
                                      bgdate,						  // date
                                      row["Flugminuten"],             // airtime
                                      gliders_[row["GleitschirmId"]], // glider
                                      takeoffs_[row["StartplatzId"]], // takeoff
                                      landings_[row["LandeplatzId"]], // landing
                                      row["Bemerkungen"],             // story
                                      wpts));                         // waypoints

    return make_pair(flId, flt);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_mdb::consolidateLocation(pair<const unsigned int, shared_ptr<Location> > &locp, Location::UseAs usage, FlightDatabase &fldb)
{
    FlightDatabase::Locations::const_iterator fit = find_if(fldb.Locations.begin(), fldb.Locations.end(), bind(&Location::isEquivalentSp,  *locp.second, _1));
    if(fit == fldb.Locations.end())
    {
        // we didn't find an equivalent location, but there might still exist one with the same name
        bool found = false;
        BOOST_FOREACH(shared_ptr<Location> los, fldb.Locations)
            if(*locp.second == *los)
                found = true;
        if(found)
            locp.second->setName(locp.second->name() + "cpy");
        locp.second->addUsage(usage);
        fldb.addLocation(locp.second);
    }
    else
    {
        (*fit)->addUsage(usage);
        locp.second = *fit;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
string inout_mdb::normalize_str(string str)
{
	transform(str.begin(), str.end(), str.begin(), bind(&inout_mdb::normalize_char, this, _1));
	return str;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
char inout_mdb::normalize_char(char chr)
{
	if((chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z'))
		return chr;
	return '_';
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

