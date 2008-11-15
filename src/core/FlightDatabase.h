#ifndef FLIGHTDATABASE_H
#define FLIGHTDATABASE_H

// flugbuch
#include "MainExternalIncludes.h"
#include "CoreStructures.h"
// boost
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
// standard library
#include <fstream>
#include <set>
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class FlightDatabase
{
	friend class boost::serialization::access;
public:
	// typedefs
	typedef std::vector<boost::shared_ptr<FlightArea> > SeqFlightAreas;
	typedef std::vector<boost::shared_ptr<Location> >   SeqLocations;
	typedef std::vector<boost::shared_ptr<Glider> >		SeqGliders;
	typedef std::set   <boost::shared_ptr<Flight> >		SeqFlights;
public:
	FlightDatabase(const std::string &PilotName, const std::string &pwd = "") : PilotName_(PilotName), Password_(encryptPassword(pwd))  {}
	~FlightDatabase() {}
	static shared_ptr<FlightDatabase> makeTestDb(void);
	bool checkPassword(const std::string &pwd) { return (encryptPassword(pwd) == Password_); }
	void printCounts(std::ostream &strm);
	// adder functions
	void addFlightArea(boost::shared_ptr<FlightArea> flar);
	void addLocation(boost::shared_ptr<Location> loc);
	void addGlider(boost::shared_ptr<Glider> gld);
	void addFlight(boost::shared_ptr<Flight> fl);
	// getters
	const std::string    & pilotName(void)   const { return PilotName_; }
	const SeqFlightAreas & flightAreas(void) const { return FlightAreas_; }
	const SeqLocations   & locations(void)   const { return Locations_; }
	const SeqGliders     & gliders(void)     const { return Gliders_; }
	const SeqFlights     & flights(void)     const { return Flights_; }
    SeqFlightAreas & flightAreas(void) { return FlightAreas_; }
	SeqLocations   & locations(void)   { return Locations_; }
	SeqGliders     & gliders(void)     { return Gliders_; }
	SeqFlights     & flights(void)     { return Flights_; }

	// deleter functions
	void deleteFlightArea(SeqFlightAreas::iterator itdel);
	void deleteLocation(SeqLocations::iterator itdel);
	void deleteGlider(SeqGliders::iterator itdel);
	void deleteFlight(SeqFlights::iterator itdel);
	// selectors
	const boost::shared_ptr<FlightArea> getArea(const std::string &nam) const;
	const SeqFlightAreas getFlightAreasEx(Location::UseAs use) const;
	const SeqLocations getLocationsEx(const boost::shared_ptr<FlightArea> flar, Location::UseAs use) const;

	template<class Archive> void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(PilotName_);
		ar & BOOST_SERIALIZATION_NVP(Password_);
		ar & BOOST_SERIALIZATION_NVP(FlightAreas_);
		ar & BOOST_SERIALIZATION_NVP(Locations_);
		ar & BOOST_SERIALIZATION_NVP(Gliders_);
		ar & BOOST_SERIALIZATION_NVP(Flights_);
	}
private:
	// members
	std::string		PilotName_;
	std::string     Password_;  // encrypted
	SeqFlightAreas	FlightAreas_;
	SeqLocations   	Locations_;
	SeqGliders     	Gliders_;
	SeqFlights	 	Flights_;

	static std::string encryptPassword(const std::string &pwd);
};


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
} // namespace flb
#endif // FLIGHTDATABASE_H
