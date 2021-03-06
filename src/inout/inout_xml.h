#ifndef INOUT_XML_H
#define INOUT_XML_H

#include "inout.h"

// input output to xml files using boost::serialize
// the xml format is specific to boost::serialize
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class inout_xml : public inout_flb
{
public:
	inout_xml() : inout_flb() {}
	virtual ~inout_xml() {}

	virtual FlightDatabase read(const boost::filesystem::path &source);
	virtual void write(const FlightDatabase &fdb, const boost::filesystem::path &destination);
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flb
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // INOUT_XML_H
