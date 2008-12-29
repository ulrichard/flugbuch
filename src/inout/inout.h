#ifndef INOUT_H
#define INOUT_H

#include "MainExternalIncludes.h"
#include "FlightDatabase.h"
// boost
#include <boost/filesystem.hpp>

namespace flb
{
class inout
{
public:
	inout() {}
	virtual ~inout() {}

	virtual FlightDatabase read(const boost::filesystem::path &source) = 0;
	virtual void write(const FlightDatabase &fdb, const boost::filesystem::path &destination) = 0;

private:
	inout(const inout &cpy);
	const inout & operator=(const inout &cpy);

};

} // namespace flb
#endif // INOUT_XML_H
