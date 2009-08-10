#ifndef IGC_STORAGE_H_INCLUDED
#define IGC_STORAGE_H_INCLUDED

// flugbuch
#include "FlightDatabase.h"
// boost
#include <boost/filesystem.hpp>
#include <boost/utility.hpp>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class igc_storage : boost::noncopyable
{
public:
	igc_storage(const boost::filesystem::path &storage_dir, bool dir_per_year = true)
	 : storage_dir_(storage_dir), dir_per_year_(dir_per_year) {}
	virtual ~igc_storage() {}

	boost::filesystem::path make_igc_filename(const std::string &name, int year);

private:
    boost::filesystem::path storage_dir_;
    bool                    dir_per_year_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} // namespace flb


#endif // IGC_STORAGE_H_INCLUDED