// flugbuch
#include "WaypointOptimizer.h"
// boost
#include <boost/array.hpp>
#include <boost/assign/list_of.hpp>
// standard library
#include <algorithm>

using namespace flb;
using boost::assign::list_of;
using boost::ptr_vector;
using std::string;
using std::vector;
using std::map;
using std::make_pair;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
WaypointOptimizer::WaypointOptimizer()
{
    addOptStrategy(new WaypointOptimizerNoWpt());
    addOptStrategy(new WaypointOptimizerOpenDistance());
    addOptStrategy(new WaypointOptimizerRichi());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
map<string, WaypointOptimizerStrategyBase::OptRes> WaypointOptimizer::optimize() const
{
    map<string, WaypointOptimizerStrategyBase::OptRes> res;

    for(ptr_vector<WaypointOptimizerStrategyBase>::const_iterator it = strategies_.begin(); it != strategies_.end(); ++it)
        res.insert(res.end(), make_pair(it->name(), it->optimize(*dist_mx_)));


    return res;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
WaypointOptimizerNoWpt::OptRes WaypointOptimizerNoWpt::optimize(const WaypointOptimizerStrategyBase::DistMatrixT &mx) const
{
    OptRes res;
    res.push_back(0);
    res.push_back(mx.size1() - 1);
    return res;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
WaypointOptimizerOpenDistance::OptRes WaypointOptimizerOpenDistance::optimize(const WaypointOptimizerStrategyBase::DistMatrixT &mx) const
{
    boost::array<size_t, 5> idx_wpt = {0, 0, 0, 0, 0};
    unsigned int currdist = 0;

    // brute force for the moment
    // maybe we can use the traveling salesman approach from the boost::graph later on
    // exploring the full range took too long, so we handle start and end point separately
    size_t i0 = 0;
    size_t i4 = mx.size1() - 1;

    for(size_t i1=i0+1; i1<mx.size1(); i1 += 5)
        for(size_t i2=i1+1; i2<mx.size1(); i2 += 5)
            for(size_t i3=i2+1; i3<mx.size1(); i3 += 5)
            {
                unsigned int newdist = mx(i0, i1) + mx(i1, i2) + mx(i2, i3) + mx(i3, i4);
                if(newdist > currdist)
                {
 //                   std::cout << "dist(" << i0 << "," << i1 << "," << i2 << "," << i3 << "," << i4 << ") = " << newdist << std::endl;
                    idx_wpt = list_of(i0)(i1)(i2)(i3)(i4);
                    currdist = newdist;
                }
            }
    for(i0=0; i0<idx_wpt[1]; i0 += 5)
    {
        unsigned int newdist = mx(i0, idx_wpt[1]) + mx(idx_wpt[1], idx_wpt[2]) + mx(idx_wpt[2], idx_wpt[3]) + mx(idx_wpt[3], idx_wpt[4]);
        if(newdist > currdist)
        {
            idx_wpt[0] = i0;
            currdist = newdist;
        }
    }
    for(i4=idx_wpt[3]+1; i4<mx.size1(); i4 += 5)
    {
        unsigned int newdist = mx(idx_wpt[0], idx_wpt[1]) + mx(idx_wpt[1], idx_wpt[2]) + mx(idx_wpt[2], idx_wpt[3]) + mx(idx_wpt[3], i4);
        if(newdist > currdist)
        {
            idx_wpt[4] = i4;
            currdist = newdist;
        }
    }

    OptRes res;
    std::copy(idx_wpt.begin(), idx_wpt.end(), std::back_inserter(res));
    return res;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
WaypointOptimizerRichi::OptRes WaypointOptimizerRichi::optimize(const WaypointOptimizerStrategyBase::DistMatrixT &mx) const
{
    boost::array<size_t, 5> idx_wpt = {0, 0, 0, 0, 0};
    unsigned int currdist = 0;

    // brute force for the moment
    // maybe we can use the travveling salesman approache from the boost::graph later on
    size_t i0 = 0;
    size_t i4 = mx.size1()-1;
    for(size_t i1=i0+1; i1<mx.size1(); i1 += 5)
    {
//        std::cout << "WaypointOptimizerRichi::optimize " << i1 << " of " << mx.size1() << std::endl;
        for(size_t i2=i1+1; i2<mx.size1(); i2 += 5)
            for(size_t i3=i2+1; i3<mx.size1(); i3 += 5)
                {
                    unsigned int newdist = mx(i0, i1) + mx(i1, i2) + mx(i2, i3) + mx(i3, i4);
                    if(newdist > currdist)
                    {
//                        std::cout << "dist(" << i0 << "," << i1 << "," << i2 << "," << i3 << "," << i4 << ") = " << newdist << std::endl;
                        idx_wpt = list_of(i0)(i1)(i2)(i3)(i4);
                        currdist = newdist;
                    }
                }
    }

    OptRes res;
    std::copy(idx_wpt.begin(), idx_wpt.end(), std::back_inserter(res));
    return res;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A



