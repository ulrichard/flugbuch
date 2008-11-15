#include "inout_xml.h"
#include <fstream>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/greg_serialize.hpp>

using namespace flb;

FlightDatabase inout_xml::read(const std::string &source)
{
	FlightDatabase fldb("");
	// create and open an archive for input
    std::ifstream ifs(source.c_str());
	if(!ifs.good())
        throw std::runtime_error("file not found");
	boost::archive::xml_iarchive ia(ifs);
    // read class state from archive
	ia >> BOOST_SERIALIZATION_NVP(fldb);
    // archive and stream closed when destructors are called
	return fldb;
}

void inout_xml::write(const FlightDatabase &fdb, const std::string &destination)
{
	std::ofstream ofs(destination.c_str());
	assert(ofs.good());
	boost::archive::xml_oarchive oa(ofs);
    // write class instance to archive
	oa << BOOST_SERIALIZATION_NVP(fdb);
    // archive and stream closed when destructors are called
}
