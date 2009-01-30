#ifndef INOUT_MDB_WIN_H
#define INOUT_MDB_WIN_H

#include "inout.h"
#include <vector>
#include <map>
#include <utility>
#include <stdexcept>
#include <boost/function.hpp>

// input from mdb files from the original Flugbuch program
// on linux using odbctools to export the tables to csv and parse from there
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// forward declarations
namespace dtl
{
    class variant_row;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class inout_mdb : public inout
{
public:
	inout_mdb() : inout() {}
	virtual ~inout_mdb() {}

	virtual FlightDatabase read(const boost::filesystem::path &source);
	virtual void write(const FlightDatabase &fdb, const boost::filesystem::path &destination)
	{
	    throw std::logic_error("not implemented");
    };

private:
	inout_mdb(const inout &cpy);
	const inout_mdb & operator=(const inout_mdb &cpy);

    std::pair<unsigned int, boost::shared_ptr<FlightArea> > GetArea(const dtl::variant_row &row);
    std::pair<unsigned int, boost::shared_ptr<Glider> >     GetGlider(const dtl::variant_row &row);
    std::pair<unsigned int, boost::shared_ptr<Location> >   GetLocation(const dtl::variant_row &row, const std::string &idname);
    std::pair<unsigned int, boost::shared_ptr<Flight> >     GetFlight(const dtl::variant_row &row);
    void consolidateLocation(std::pair<const unsigned int, boost::shared_ptr<Location> >  &locp, Location::UseAs usage, FlightDatabase &fldb);

//	boost::filesystem::path export_csv(const boost::filesystem::path &source, const std::string &tablename);
//	void parse_csv(const boost::filesystem::path &file, boost::function<void(const std::vector<std::string>&)> lineFunc);
//	void readCountry(const std::vector<std::string> &tokens);
/*
	void readFlightArea(const std::vector<std::string> &tokens);
	void readGlider(const std::vector<std::string> &tokens);
	void readLocation(const std::vector<std::string> &tokens, std::map<unsigned int, boost::shared_ptr<Location> > &locations);
	void readFlight(const std::vector<std::string> &tokens);
    void readWptLink(const std::vector<std::string> &tokens);
*/
    // members
	std::map<unsigned int, std::string>                             countries_;
	std::map<unsigned int, boost::shared_ptr<FlightArea> >          areas_;
	std::map<unsigned int, boost::shared_ptr<Glider> >              gliders_;
	std::map<unsigned int, boost::shared_ptr<Location> >            takeoffs_, landings_, waypoints_;
	std::map<unsigned int, boost::shared_ptr<Flight> >              flights_;
	std::map<unsigned int, std::pair<unsigned int, unsigned int> >  wptlinks_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flb
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#endif // INOUT_MDB_WIN_H
