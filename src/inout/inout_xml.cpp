#include "inout_xml.h"
// boost
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/greg_serialize.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>
#include <boost/filesystem/fstream.hpp>
// standard library
#include <fstream>

using namespace flb;
namespace bfs = boost::filesystem;

FlightDatabase inout_xml::read(const bfs::path &source)
{
	FlightDatabase fldb("");
	// create and open an archive for input
    bfs::ifstream ifs(source, std::ios_base::in | std::ios_base::binary);
	if(!ifs.good())
        throw std::runtime_error("file not found");
	boost::archive::xml_iarchive ia(ifs);
    // read class state from archive
	ia >> BOOST_SERIALIZATION_NVP(fldb);
    // archive and stream closed when destructors are called
	return fldb;
}

void inout_xml::write(const FlightDatabase &fdb, const bfs::path &destination)
{
    bfs::create_directories(destination.branch_path());
	bfs::ofstream ofs(destination, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	if(!ofs.good())
        throw std::runtime_error("could not write to " + destination.string());
	boost::archive::xml_oarchive oa(ofs);
    // write class instance to archive
	oa << BOOST_SERIALIZATION_NVP(fdb);
    // archive and stream closed when destructors are called
}
