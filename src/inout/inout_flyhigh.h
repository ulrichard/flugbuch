#ifndef INOUT_FLYHIGH_H_INCLUDED
#define INOUT_FLYHIGH_H_INCLUDED

#include "inout.h"
#include <utility>

// read (and write) the flights from a flyhigh database
// see flyhight.sourceforge.net for details
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace dtl
{
    class variant_row;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class inout_flyhigh : public inout
{
public:
	inout_flyhigh() : inout() {}
	virtual ~inout_flyhigh() {}

	virtual FlightDatabase read(const boost::filesystem::path &source);
	virtual void write(const FlightDatabase &fdb, const boost::filesystem::path &destination);

private:
	inout_flyhigh(const inout &cpy);
	const inout_flyhigh & operator=(const inout_flyhigh &cpy);

    std::pair<unsigned int, boost::shared_ptr<Glider> >     GetGlider(const dtl::variant_row &row);
    std::pair<unsigned int, boost::shared_ptr<Location> >   GetLocation(const dtl::variant_row &row, FlightDatabase &fldb);
    std::pair<unsigned int, boost::shared_ptr<Flight> >     GetFlight(const dtl::variant_row &row);

    dtl::variant_row  SetGlider(const boost::shared_ptr<Glider> &gld);

	    // members
	std::map<unsigned int, boost::shared_ptr<Glider> >              gliders_;
	std::map<unsigned int, boost::shared_ptr<Location> >            waypoints_;
	std::map<unsigned int, boost::shared_ptr<Flight> >              flights_;
//	std::map<unsigned int, std::pair<unsigned int, unsigned int> >  wptlinks_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flb
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A



#endif // INOUT_FLYHIGH_H_INCLUDED
