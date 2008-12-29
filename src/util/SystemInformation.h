#ifndef SYSTEMINFORMATION_H
#define SYSTEMINFORMATION_H

// standard library
#include <string>
// boost
#include <boost/filesystem.hpp>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class SystemInformation
{
public:

    static boost::filesystem::path homeDir();
    static boost::filesystem::path tempDir();
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#endif // SYSTEMINFORMATION_H
