// flugbuch
#include "igc_storage.h"
// boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>

using namespace flb;
// boost
namespace bfs = boost::filesystem;
// standard library
using std::string;
using std::vector;


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bfs::path igc_storage::make_igc_filename(const string &name, int year) const
{
    bfs::path igc_file = storage_dir_;
    if(dir_per_year_)
        igc_file /= boost::lexical_cast<string>(year);
    igc_file /= name;
    igc_file = bfs::change_extension(igc_file, ".igc");

    return  igc_file;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
vector<bfs::path> igc_storage::find_all_igc_files() const
{
    vector<bfs::path> files;

    if(bfs::exists(storage_dir_))
    {
        if(!dir_per_year_)
            search_dir(storage_dir_, files);
        else
        {
            const bfs::directory_iterator end_itr; // default construction yields past-the-end
            for(bfs::directory_iterator itr(storage_dir_); itr != end_itr; ++itr)
                if(is_directory(itr->status()) && itr->path().filename().length() == 4)
                    search_dir(itr->path(), files);
        }
    }

    return files;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void igc_storage::search_dir(const bfs::path &dir, std::vector<bfs::path> &files) const
{
    const bfs::directory_iterator end_itr; // default construction yields past-the-end
    for(bfs::directory_iterator itr(dir); itr != end_itr; ++itr)
        if(!is_directory(itr->status()) && boost::to_lower_copy(bfs::extension(itr->path())) == ".igc")
            files.push_back(itr->path());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A


