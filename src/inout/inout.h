#ifndef INOUT_H
#define INOUT_H

#include "MainExternalIncludes.h"
#include "FlightDatabase.h"

namespace flb
{
class inout
{
public:
	inout() {}
	virtual ~inout() {}

	virtual FlightDatabase read(const std::string &source) = 0;
	virtual void write(const FlightDatabase &fdb, const std::string &destination) = 0;

private:
	inout(const inout &cpy);
	const inout & operator=(const inout &cpy);

};

} // namespace flb
#endif // INOUT_XML_H
