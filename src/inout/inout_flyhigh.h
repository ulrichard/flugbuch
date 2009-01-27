#ifndef INOUT_FLYHIGH_H_INCLUDED
#define INOUT_FLYHIGH_H_INCLUDED

#include "inout.h"

// read (and write) the flights from a flyhigh database
// see flyhight.sourceforge.net for details
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
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flb
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A



#endif // INOUT_FLYHIGH_H_INCLUDED
