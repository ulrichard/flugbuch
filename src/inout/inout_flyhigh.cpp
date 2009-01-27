// flugbuch
#include "inout_flyhigh.h"
#include "SystemInformation.h"
// mdb odbc
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
using boost::lexical_cast;
using boost::bind;
using boost::ref;
namespace bfs = boost::filesystem;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
FlightDatabase inout_flyhigh::read(const bfs::path &source)
{
	FlightDatabase fldb("");



	return fldb;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void inout_flyhigh::write(const FlightDatabase &fdb, const boost::filesystem::path &destination)
{


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
