// flugbuch
#include "inout_mdb.h"
#include "SystemInformation.h"
// boost
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
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
namespace bll = boost::lambda;
using namespace boost::lambda;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
FlightDatabase inout_mdb::read(const bfs::path &source)
{
	FlightDatabase fldb("");
    // the areas can be used as is
	parse_csv(export_csv(source, "Laender"),       ::bind(&inout_mdb::readCountry,    this, ::_1));
	parse_csv(export_csv(source, "Fluggebiete"),   ::bind(&inout_mdb::readFlightArea, this, ::_1));
    for(map<unsigned int, shared_ptr<FlightArea> >::iterator it = areas_.begin(); it != areas_.end(); ++it)
        fldb.addFlightArea(it->second);
    // the gliders can be used as is
	parse_csv(export_csv(source, "Gleitschirme"),  ::bind(&inout_mdb::readGlider,     this, ::_1));
    for(map<unsigned int, shared_ptr<Glider> >::iterator it = gliders_.begin(); it != gliders_.end(); ++it)
        fldb.addGlider(it->second);
    // we have to consolidate the locations
	parse_csv(export_csv(source, "Startplaetze"),  ::bind(&inout_mdb::readLocation,   this, ::_1, ref(takeoffs_)));
	parse_csv(export_csv(source, "Landeplaetze"),  ::bind(&inout_mdb::readLocation,   this, ::_1, ref(landings_)));
	parse_csv(export_csv(source, "Wegpunkte"),     ::bind(&inout_mdb::readLocation,   this, ::_1, ref(waypoints_)));
	parse_csv(export_csv(source, "lnkWegpFluege"), ::bind(&inout_mdb::readWptLink,    this, ::_1));
    for_each(takeoffs_.begin(),  takeoffs_.end(),  ::bind(&inout_mdb::consolidateLocation, this, ::_1, Location::UA_TAKEOFF, ref(fldb)));
    for_each(landings_.begin(),  landings_.end(),  ::bind(&inout_mdb::consolidateLocation, this, ::_1, Location::UA_LANDING, ref(fldb)));
    for_each(waypoints_.begin(), waypoints_.end(), ::bind(&inout_mdb::consolidateLocation, this, ::_1, Location::UA_WAYPNT,  ref(fldb)));
    // flights
	parse_csv(export_csv(source, "Fluege"),        ::bind(&inout_mdb::readFlight,     this, ::_1));
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

	return fldb;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bfs::path inout_mdb::export_csv(const bfs::path &source, const string &tablename)
{
    const bfs::path outfile = flb::SystemInformation::tempDir() / ("flb_imp_" + tablename + ".csv");
    bfs::remove(outfile);
    const string cmd = "mdb-export " + source.external_file_string() + " " + tablename + " -H -D %Y/%m/%d > " + outfile.external_file_string();
    system(cmd.c_str());
    // check if the file was written
    if(!bfs::exists(outfile))
        throw std::runtime_error("failed to write file " + outfile.string());
    return outfile;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_mdb::parse_csv(const bfs::path &file, boost::function<void(const vector<string>&)> lineFunc)
{
    typedef boost::tokenizer<boost::escaped_list_separator<char> > TokenizerT;
    bfs::ifstream ifs(file);
    if(!ifs.good())
        throw std::runtime_error("file not found");
    string line, adline;
    while(std::getline(ifs, line, '\n'))
    {
        // because linebreaks in the db fields are linebreaks in the csv file, we have to exercise a bit here
        size_t numdq = count(line.begin(), line.end(), '\"');
        while((numdq % 2) != 0 && std::getline(ifs, adline, '\n'))
        {
            numdq += count(adline.begin(), adline.end(), '\"');
            line  += " # " + adline;
        }

        TokenizerT tok(line);
        vector<string> tokens;
        copy(tok.begin(), tok.end(), back_inserter(tokens));
        lineFunc(tokens);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_mdb::readCountry(const vector<string> &tokens)
{
    if(tokens.size() == 3)
        countries_[lexical_cast<unsigned int>(tokens[0])] = tokens[1]; // we omit the description of the country
    else
        assert(!"wrong number of tokens for Country");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_mdb::readFlightArea(const vector<string> &tokens)
{
    if(tokens.size() == 4)
    {
        shared_ptr<FlightArea> flar(new FlightArea(tokens[2], countries_[lexical_cast<int>(tokens[1])], tokens[3])); // name, country, description
        areas_[lexical_cast<unsigned int>(tokens[0])] = flar;
    }
    else
        assert(!"wrong number of tokens for FlightArea");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_mdb::readGlider(const vector<string> &tokens)
{
    if(tokens.size() == 9)
    {
        unsigned short year = atoi(tokens[6].c_str());
        if(year == 0)
            ;
        else if(year <= boost::gregorian::day_clock::local_day().year() - 2000)
            year += 2000;
        else if(year < 70)
            year += 1900;

        shared_ptr<Glider> gld(new Glider(tokens[1],   // brand
                                          tokens[2],   // model
                                          tokens[3],   // size
                                          tokens[4],   // color
                                          year,        // year
                                          tokens[5],   // classification
                                          tokens[7])); // description

        gliders_[lexical_cast<int>(tokens[0])] = gld;
    }
    else
        assert(!"wrong number of tokens for Glider");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_mdb::readLocation(const vector<string> &tokens, std::map<unsigned int, boost::shared_ptr<Location> > &locations)
{
    if(tokens.size() == 8)
    {
        shared_ptr<Location> loc(new Location(areas_[lexical_cast<unsigned int>(tokens[1])], // area
                                              tokens[2],                                     // name
                                              atoi(tokens[4].c_str()),                       // height
                                              atof(tokens[6].c_str()),                       // latitude
                                              atof(tokens[5].c_str()),                       // longitude
                                              0));                                           // usage will be assigned later
        locations[lexical_cast<int>(tokens[0])] = loc;
    }
    else
        assert(!"wrong number of tokens for Location");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_mdb::readFlight(const vector<string> &tokens)
{
    if(tokens.size() == 8)
    {
        const unsigned int flId = lexical_cast<unsigned int>(tokens[0]);

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

        flights_[flId] = shared_ptr<Flight>((new Flight(lexical_cast<unsigned int>(tokens[1]),    // flight number
                                                boost::gregorian::from_string(tokens[2]),         // date
                                                lexical_cast<unsigned int>(tokens[6]),            // airtime
                                                gliders_[lexical_cast<unsigned int>(tokens[3])],  // glider
                                                takeoffs_[lexical_cast<unsigned int>(tokens[4])], // takeoff
                                                landings_[lexical_cast<unsigned int>(tokens[5])], // landing
                                                tokens[7],                                        // story
                                                wpts)));                                          // waypoints
    }
    else
        assert(!"wrong number of tokens for Flight");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_mdb::readWptLink(const vector<string> &tokens)
{
    if(tokens.size() == 3)
    {
        wptlinks_[lexical_cast<unsigned int>(tokens[0])] = make_pair(lexical_cast<unsigned int>(tokens[1]),
                                                                     lexical_cast<unsigned int>(tokens[2]));
    }
    else
        assert(!"wrong number of tokens for WptLink");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_mdb::consolidateLocation(pair<const unsigned int, shared_ptr<Location> > &locp, Location::UseAs usage, FlightDatabase &fldb)
{
    FlightDatabase::Locations::const_iterator fit = find_if(fldb.Locations.begin(), fldb.Locations.end(), bll::bind(&Location::isEquivalent,  *locp.second, *bll::_1));
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

