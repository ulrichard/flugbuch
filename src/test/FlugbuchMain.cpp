#include "FlightDatabase.h"
#include "inout_xml.h"
#include "inout_mdb.h"
#include "SystemInformation.h"

#include <stdlib.h>
#include <iostream>

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

//#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/filesystem.hpp>

using namespace boost::unit_test;
using boost::shared_ptr;
using boost::gregorian::date;
using std::string;
namespace bfs = boost::filesystem;
//
//
//
/*
int main(int argc, char** argv)
{
	return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}
*/

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
BOOST_AUTO_TEST_CASE(SerialisationTestXml)
{
    shared_ptr<flb::FlightDatabase> fldb = flb::FlightDatabase::makeTestDb();

	BOOST_CHECK_EQUAL(fldb->flights().size(),      8);
	BOOST_CHECK_EQUAL(fldb->flightAreas ().size(), 5);
	BOOST_CHECK_EQUAL(fldb->locations().size(),    8);
	BOOST_CHECK_EQUAL(fldb->gliders().size(),      3);

	flb::inout_xml ioxml;

	bfs::path tmpfile = flb::SystemInformation::tempDir() / "Flugbuch2Test.xml";
	ioxml.write(*fldb, tmpfile);

	flb::FlightDatabase fldb2 = ioxml.read(tmpfile);

	BOOST_CHECK_EQUAL(fldb2.flights().size(),      8);
	BOOST_CHECK_EQUAL(fldb2.flightAreas ().size(), 5);
	BOOST_CHECK_EQUAL(fldb2.locations().size(),    8);
	BOOST_CHECK_EQUAL(fldb2.gliders().size(),      3);


    // location selection
    shared_ptr<flb::FlightArea> arSchwyz = fldb->getArea("Schwyz");
    flb::FlightDatabase::SeqLocations selSchwyz = fldb->getLocationsEx(arSchwyz, flb::Location::UA_LANDING);
    BOOST_CHECK_EQUAL(1, selSchwyz.size());
    BOOST_CHECK_EQUAL("Steisteg", selSchwyz[0]->name());

    shared_ptr<flb::FlightArea> arSteinen = fldb->getArea("Steinen");
    flb::FlightDatabase::SeqLocations selSteinen = fldb->getLocationsEx(arSteinen, flb::Location::UA_LANDING);
    BOOST_CHECK_EQUAL(1, selSteinen.size());
    BOOST_CHECK_EQUAL("Frauholz", selSteinen[0]->name());

    shared_ptr<flb::FlightArea> arRothenflue = fldb->getArea("Rothenflue");
    flb::FlightDatabase::SeqLocations selRothenflue = fldb->getLocationsEx(arRothenflue, flb::Location::UA_TAKEOFF);
    BOOST_CHECK_EQUAL(2, selRothenflue.size());
    BOOST_CHECK_EQUAL("Westwindstartplatz", selRothenflue[0]->name());
    BOOST_CHECK_EQUAL("kleiner Mythen", selRothenflue[1]->name());

    flb::FlightDatabase::SeqLocations selRothenflueWpt = fldb->getLocationsEx(arRothenflue, flb::Location::UA_WAYPNT);
    BOOST_CHECK_EQUAL(2, selRothenflueWpt.size());
    BOOST_CHECK_EQUAL("kleiner Mythen", selRothenflueWpt[0]->name());
    BOOST_CHECK_EQUAL("Zwischenmythen", selRothenflueWpt[1]->name());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
BOOST_AUTO_TEST_CASE(ReadMdbTest)
{
    flb::inout_mdb iomdb;
    bfs::path flbfile = flb::SystemInformation::homeDir() / "Flugbuch.mdb";
    BOOST_REQUIRE(bfs::exists(flbfile));
	boost::shared_ptr<flb::FlightDatabase> fldb(new flb::FlightDatabase(iomdb.read(flbfile)));

    BOOST_CHECK_GE(fldb->gliders().size(),      59);
    BOOST_CHECK_GE(fldb->flightAreas().size(), 150);
    BOOST_CHECK_GE(fldb->locations().size(),  1173);
    BOOST_CHECK_GE(fldb->flights().size(),    1580);

}
