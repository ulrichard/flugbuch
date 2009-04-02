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
class inout_mdb : public inout_flb
{
public:
	inout_mdb() : inout_flb() {}
	virtual ~inout_mdb() {}

	virtual FlightDatabase read(const boost::filesystem::path &source);
	virtual void write(const FlightDatabase &fdb, const boost::filesystem::path &destination)
	{
	    throw std::logic_error("not implemented");
    };

private:
    std::pair<unsigned int, boost::shared_ptr<FlightArea> > GetArea(const dtl::variant_row &row);
    std::pair<unsigned int, boost::shared_ptr<Glider> >     GetGlider(const dtl::variant_row &row);
    std::pair<unsigned int, boost::shared_ptr<Location> >   GetLocation(const dtl::variant_row &row, const std::string &idname);
    std::pair<unsigned int, boost::shared_ptr<Flight> >     GetFlight(const dtl::variant_row &row);
    void consolidateLocation(std::pair<const unsigned int, boost::shared_ptr<Location> >  &locp, Location::UseAs usage, FlightDatabase &fldb);
	std::string normalize_str(std::string str);
	char		normalize_char(char chr);

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
