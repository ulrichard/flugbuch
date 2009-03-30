#ifndef INOUT_H
#define INOUT_H

// flugbuch
#include "MainExternalIncludes.h"
#include "FlightDatabase.h"
// boost
#include <boost/filesystem.hpp>
#include <boost/utility.hpp>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class inout_flb : boost::noncopyable
{
public:
	inout_flb() {}
	virtual ~inout_flb() {}

	virtual FlightDatabase read(const boost::filesystem::path &source) = 0;
	virtual void write(const FlightDatabase &fdb, const boost::filesystem::path &destination) = 0;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class inout_flight : boost::noncopyable
{
public:
    inout_flight() {}
    virtual ~inout_flight() {}

    virtual void read(const boost::filesystem::path &source) = 0;
    virtual void write(const Flight &flt, const boost::filesystem::path &destination) = 0;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flb
#endif // INOUT_XML_H
