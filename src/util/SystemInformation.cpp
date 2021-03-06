// flugbuch
#include "SystemInformation.h"
// windows
#ifdef WIN32
  #include <shlobj.h>
#endif
// boost
#include <boost/filesystem.hpp>
// standard library
#include <exception>



using namespace flb;
namespace bfs = boost::filesystem;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
bfs::path SystemInformation::homeDir()
{
#ifdef WIN32
    char tmp[MAX_PATH];
	HRESULT retval = SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, tmp);
	if(retval == S_FALSE || retval == E_FAIL)
		throw std::runtime_error("Home directory not found : The CSIDL in nFolder is valid, but the folder does not exist.");
	if(retval == E_INVALIDARG)
		throw std::runtime_error("Home directory not found : The CSIDL in nFolder is not valid..");
    return bfs::path(tmp);
#else
    return bfs::path(getenv("HOME"));
#endif
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
bfs::path SystemInformation::tempDir()
{
#ifdef WIN32
    char tmp[MAX_PATH];
    DWORD len = MAX_PATH;
    GetTempPath(len, tmp);
    return bfs::path(tmp);
#else
    return bfs::path("/tmp");
#endif
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8


