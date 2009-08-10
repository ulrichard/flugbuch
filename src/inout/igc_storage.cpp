// flugbuch
#include "igc_storage.h"
// boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>

using namespace flb;
// boost
namespace bfs = boost::filesystem;
// standard library
using std::string;


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bfs::path igc_storage::make_igc_filename(const string &name, int year)
{
    bfs::path igc_file = storage_dir_;
    if(dir_per_year_)
        igc_file /= boost::lexical_cast<string>(year);
    igc_file /= name;
    igc_file = bfs::change_extension(igc_file, ".igc");

    return  igc_file;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
