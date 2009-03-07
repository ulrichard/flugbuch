// flugbuch
#include "inout_flyhigh.h"
#include "SystemInformation.h"
#include "FormatStr.h"
// mdb odbc
#define DTL_NO_UNICODE
#include <dtl/DTL.h>
// boost
#include <boost/tokenizer.hpp>
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
using std::find_if;
using boost::lexical_cast;
using boost::bind;
using boost::ref;
namespace bfs = boost::filesystem;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
FlightDatabase inout_flyhigh::read(const bfs::path &source)
{
	FlightDatabase fldb("");

    try
    {
        string connectstr = FormatStr() << "Driver={Microsoft Access Driver (*.mdb)}; Dbq=" << source.external_file_string() << ";";
        dtl::DBConnection::GetDefaultConnection().Connect(connectstr);

        // gliders can be used as is
        dtl::DynamicDBView<> viewGliders("Gliders", "*");
        transform(viewGliders.begin(), viewGliders.end(), inserter(gliders_, gliders_.end()), bind(&inout_flyhigh::GetGlider, this, _1));
        for(map<unsigned int, shared_ptr<Glider> >::iterator it = gliders_.begin(); it != gliders_.end(); ++it)
            fldb.addGlider(it->second);

        // locations can be used more or less as is
        dtl::DynamicDBView<> viewWayPnt("WayPoints", "*");
        transform(viewWayPnt.begin(), viewWayPnt.end(), inserter(waypoints_, waypoints_.end()), bind(&inout_flyhigh::GetLocation, this, _1, fldb));
        for(map<unsigned int, shared_ptr<Location> >::iterator it = waypoints_.begin(); it != waypoints_.end(); ++it)
            fldb.addLocation(it->second);

        // Flights
        dtl::DynamicDBView<> viewFlight("Fluege", "*");
        transform(viewFlight.begin(), viewFlight.end(), inserter(flights_, flights_.end()), bind(&inout_flyhigh::GetFlight, this, _1));
        for(map<unsigned int, shared_ptr<Flight> >::iterator it = flights_.begin(); it != flights_.end(); ++it)
            fldb.addFlight(it->second);

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
void inout_flyhigh::write(const FlightDatabase &fdb, const boost::filesystem::path &destination)
{
    try
    {
        string connectstr = FormatStr() << "Driver={Microsoft Access Driver (*.mdb)}; Dbq=" << destination.external_file_string() << ";";
        dtl::DBConnection::GetDefaultConnection().Connect(connectstr);

        // gliders can be used as is
        dtl::DynamicDBView<> viewGliders("Gliders", "*");
        dtl::DynamicDBView<>::insert_iterator write_it = viewGliders;
        transform(fdb.Gliders.begin(), fdb.Gliders.end(), write_it, bind(&inout_flyhigh::SetGlider, this, _1));

    }
    catch(std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<unsigned int, shared_ptr<Glider> > inout_flyhigh::GetGlider(const dtl::variant_row &row)
{
    shared_ptr<Glider> gld(new Glider(row["Manufacturer"],  // brand
                                      row["Model"],         // model
                                      "",                   // size
                                      "",                   // color
                                      0,                   // year
                                      row["Serial"],        // classification
                                      ""));                 // description
    return make_pair(static_cast<int>(row["Id"]), gld);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
dtl::variant_row  inout_flyhigh::SetGlider(const shared_ptr<Glider> &gld)
{
    dtl::variant_row row;

    row["Manufacturer"] = gld->brand();
    row["Model"]        = gld->model();

    return row;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<unsigned int, boost::shared_ptr<Location> > inout_flyhigh::GetLocation(const dtl::variant_row &row, FlightDatabase &fldb)
{
    const string areaaName = row["Spot"];

    FlightDatabase::FlightAreas::iterator itArea = find_if(fldb.FlightAreas.begin(), fldb.FlightAreas.end(),
                                                            bind(&FlightArea::name, *_1) == areaaName);
    if(itArea == fldb.FlightAreas.end())
    {
        shared_ptr<FlightArea> newArea(new FlightArea(areaaName, row["Country"], ""));
        fldb.addFlightArea(newArea);
        itArea = find_if(fldb.FlightAreas.begin(), fldb.FlightAreas.end(),
                                                            bind(&FlightArea::name, *_1) == areaaName);
    }
    if(itArea == fldb.FlightAreas.end())
        throw std::runtime_error("Flight area not found");

    shared_ptr<Location> loc(new Location(*itArea,          // area
                                          row["Name"],      // name
                                          row["Altitude"],  // height
                                          row["Latitude"],  // latitude
                                          row["Longitude"], // longitude
                                          Location::UA_TAKEOFF | Location::UA_LANDING | Location::UA_WAYPNT)); // usage will be restricted later

    return make_pair(static_cast<int>(row["Id"]), loc);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<unsigned int, boost::shared_ptr<Flight> > inout_flyhigh::GetFlight(const dtl::variant_row &row)
{
	const unsigned int flId = row["Id"];
	vector<shared_ptr<Location> > wpts;

	tagTIMESTAMP_STRUCT tms = row["Date"];
	boost::gregorian::date bgdate(tms.year, tms.month, tms.day);

    shared_ptr<Flight> flt(new Flight(row["Number"],                // flight number
                                      bgdate,						// date
                                      row["Duration"],              // airtime
                                      gliders_[row["GliderId"]],    // glider
                                      waypoints_[row["StartPtId"]], // takeoff
                                      waypoints_[row["LandPtId"]],  // landing
                                      row["Comment"],               // story
                                      wpts));                       // waypoints

    return make_pair(flId, flt);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

// for reference here is the db structure as found in  ..flyhigh/doc/sql/flyhigh.sql
/*
CREATE TABLE IF NOT EXISTS `Gliders`
(
	`Id` INT NULL AUTO_INCREMENT,
	`Manufacturer` VARCHAR(16) CHARACTER SET utf8 NULL DEFAULT '',
	`Model` VARCHAR(16) CHARACTER SET utf8 NOT NULL,
	`Serial` VARCHAR(16) CHARACTER SET utf8 NULL DEFAULT '',
	PRIMARY KEY(`Id`),
	UNIQUE KEY(`Manufacturer`, `Model`, `Serial`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `Pilots`
(
	`Id` INT NULL AUTO_INCREMENT,
	`FirstName` VARCHAR(16) CHARACTER SET utf8 NOT NULL,
	`LastName` VARCHAR(16) CHARACTER SET utf8 NOT NULL,
	`BirthDate` DATE NULL DEFAULT '0000-00-00',
	`CallSign` VARCHAR(16) CHARACTER SET utf8 NOT NULL,
	`GliderId` INT NULL DEFAULT '0',
	PRIMARY KEY(`Id`),
	UNIQUE KEY byPilot(`FirstName`, `LastName`, `BirthDate`),
	FOREIGN KEY(`GliderId`) REFERENCES Gliders(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE
)ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `WayPoints`
(
	`Id` INT NULL AUTO_INCREMENT,
	`Name` VARCHAR(16) NOT NULL,
	`Spot` VARCHAR(16) NULL DEFAULT '',
	`Country` VARCHAR(2) NULL DEFAULT '',
	`Longitude` FLOAT NOT NULL,
	`Latitude` FLOAT NOT NULL,
	`Altitude` INT NOT NULL,
	`Description` VARCHAR(200) NULL DEFAULT '',
	PRIMARY KEY(`Id`),
	UNIQUE KEY byWayPoints(`Name`, `Spot`, `Country`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `Flights`
(
	`Id` INT NULL AUTO_INCREMENT,
	`Number` INT NOT NULL,
	`PilotId` INT NOT NULL,
	`Date` DATE NOT NULL,
	`Time` TIME NOT NULL,
	`GliderId` INT NOT NULL,
	`StartPtId` INT NOT NULL,
	`LandPtId` INT NOT NULL,
	`Duration` INT NOT NULL,
	`Distance` INT NOT NULL,
	`Comment` VARCHAR(500) NULL DEFAULT '',
	`IGCFile` MEDIUMBLOB,
	PRIMARY KEY(`Id`),
	UNIQUE KEY byFlight(`PilotId`, `Number`),
	FOREIGN KEY(`PilotId`) REFERENCES Pilots(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,
	FOREIGN KEY(`GliderId`) REFERENCES Gliders(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,
	FOREIGN KEY(`StartPtId`) REFERENCES WayPoints(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,
	FOREIGN KEY(`LandPtId`) REFERENCES WayPoints(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE
)ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `Routes`
(
	`Id` INT NULL AUTO_INCREMENT,
  `Name` VARCHAR(16) NOT NULL,
	PRIMARY KEY(`Id`),
	UNIQUE KEY byRoutes(`Name`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `RouteItems`
(
	`Id` INT NULL AUTO_INCREMENT,
	`RouteId` INT NOT NULL,
	`WayPointId` INT NOT NULL,
	PRIMARY KEY(`Id`),
	FOREIGN KEY(`RouteId`) REFERENCES Routes(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,
	FOREIGN KEY(`WayPointId`) REFERENCES WayPoints(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE
)ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `Servicings`
(
	`Id` INT NULL AUTO_INCREMENT,
	`GliderId` INT NOT NULL,
	`Date` DATE NOT NULL,
	`Responsibility` VARCHAR(16) NOT NULL,
	`Comment` VARCHAR(200) NULL DEFAULT '',
	PRIMARY KEY(`Id`),
	FOREIGN KEY(`GliderId`) REFERENCES Gliders(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE
)ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `LastModified`
(
	`Id` INT NULL AUTO_INCREMENT,
	`Name` VARCHAR(16) NOT NULL,
	`Time` datetime NOT NULL,
	PRIMARY KEY(`Id`),
	UNIQUE KEY (`Name`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
*/
