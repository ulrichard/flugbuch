#ifndef INOUT_MDB_H
#define INOUT_MDB_H

#include "inout.h"
#include <vector>
#include <map>
#include <utility>
#include <stdexcept>
#include <boost/function.hpp>

// input from mdb files from the original Flugbuch program
// on linux using odbctools to export the tables to csv and parse from there
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class inout_mdb : public inout
{
public:
	inout_mdb() : inout() {}
	virtual ~inout_mdb() {}

	virtual FlightDatabase read(const std::string &source);
	virtual void write(const FlightDatabase &fdb, const std::string &destination) { throw std::logic_error("not implemented"); };

private:
	inout_mdb(const inout &cpy);
	const inout_mdb & operator=(const inout_mdb &cpy);

	std::string export_csv(const std::string &source, const std::string &tablename);
	void parse_csv(const std::string &file, boost::function<void(const std::vector<std::string>&)> lineFunc);
	void readCountry(const std::vector<std::string> &tokens);
	void readFlightArea(const std::vector<std::string> &tokens);
	void readGlider(const std::vector<std::string> &tokens);
	void readLocation(const std::vector<std::string> &tokens, std::map<unsigned int, boost::shared_ptr<Location> > &locations);
	void readFlight(const std::vector<std::string> &tokens);
    void readWptLink(const std::vector<std::string> &tokens);
    void consolidateLocation(std::pair<const unsigned int, boost::shared_ptr<Location> >  &locp, Location::UseAs usage, FlightDatabase &fldb);

    // members
	std::map<unsigned int, std::string>                             countries_;
	std::map<unsigned int, boost::shared_ptr<FlightArea> >          areas_;
	std::map<unsigned int, boost::shared_ptr<Glider> >              gliders_;
	std::map<unsigned int, boost::shared_ptr<Location> >            takeoffs_, landings_, waypoints_;
	std::map<unsigned int, boost::shared_ptr<Flight> >              flights_;
	std::map<unsigned int, std::pair<unsigned int, unsigned int> >  wptlinks_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
} // namespace flb
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#endif // INOUT_MDB_H
