#ifndef FLIGHTDATABASE_H
#define FLIGHTDATABASE_H

// flugbuch
#include "MainExternalIncludes.h"
#include "CoreStructures.h"
// boost
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
// standard library
#include <fstream>
#include <set>
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class FlightDatabase
{
	friend class boost::serialization::access;
	typedef std::vector<boost::shared_ptr<Glider> >		SeqGliders;
public:
	// typedefs
	typedef std::set   <boost::shared_ptr<Flight> >		SeqFlights;
	typedef std::vector<boost::shared_ptr<FlightArea> > SeqFlightAreas;
	typedef std::vector<boost::shared_ptr<Location> >   SeqLocations;
public:
	FlightDatabase(const std::string &PilotName, const std::string &pwd = "")
	  : PilotName_(PilotName), Password_(encryptPassword(pwd)), FlightAreas(*this), Locations(*this), Gliders(*this)  {}
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
	const SeqFlights     & flights(void)     const { return Flights_; }
	SeqFlights     & flights(void)     { return Flights_; }

	// setters
	void setPilotNameAndPwd(const std::string &usr, const std::string &pwd);

	// deleter functions
	void deleteFlightArea(SeqFlightAreas::iterator itdel);
	void deleteFlightArea(boost::shared_ptr<FlightArea> flar);
	void deleteLocation(SeqLocations::iterator itdel);
	void deleteLocation(boost::shared_ptr<Location> loc);
	void deleteGlider(SeqGliders::iterator itdel);
	void deleteGlider(boost::shared_ptr<Glider> gld);
	void deleteFlight(SeqFlights::iterator itdel);
	void deleteFlight(boost::shared_ptr<Flight> fl);
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

public:
// memberspaces for accessing the contents
	struct FlightAreas
	{
	    typedef SeqFlightAreas::const_iterator       iterator;
	    typedef SeqFlightAreas::const_iterator const_iterator;

        iterator       begin()        { return flb_.FlightAreas_.begin(); }
        const_iterator begin() const  { return flb_.FlightAreas_.begin(); }
        iterator       end()          { return flb_.FlightAreas_.end(); }
        const_iterator end()   const  { return flb_.FlightAreas_.end(); }

        size_t size() const { return flb_.FlightAreas_.size(); }
    private:
        friend class FlightDatabase;
        FlightAreas(const FlightDatabase &flb) : flb_(flb) { }
        const FlightDatabase &flb_;
    }FlightAreas;

	struct Locations
	{
	    typedef SeqLocations::const_iterator       iterator;
	    typedef SeqLocations::const_iterator const_iterator;

        iterator       begin()        { return flb_.Locations_.begin(); }
        const_iterator begin() const  { return flb_.Locations_.begin(); }
        iterator       end()          { return flb_.Locations_.end(); }
        const_iterator end()   const  { return flb_.Locations_.end(); }

        size_t size() const { return flb_.Locations_.size(); }
    private:
        friend class FlightDatabase;
        Locations(const FlightDatabase &flb) : flb_(flb) { }
        const FlightDatabase &flb_;
    }Locations;

    struct Gliders
	{
	    typedef SeqGliders::const_iterator       iterator;
	    typedef SeqGliders::const_iterator const_iterator;

        iterator       begin()        { return flb_.Gliders_.begin(); }
        const_iterator begin() const  { return flb_.Gliders_.begin(); }
        iterator       end()          { return flb_.Gliders_.end(); }
        const_iterator end()   const  { return flb_.Gliders_.end(); }

        size_t size() const { return flb_.Gliders_.size(); }
    private:
        friend class FlightDatabase;
        Gliders(const FlightDatabase &flb) : flb_(flb) { }
        const FlightDatabase &flb_;
    }Gliders;
};


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flb
#endif // FLIGHTDATABASE_H
