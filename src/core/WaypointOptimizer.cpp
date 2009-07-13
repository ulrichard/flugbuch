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

// static members
boost::ptr_vector<WaypointOptimizerStrategyBase> WaypointOptimizer::strategies_;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
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
WaypointOptimizerOpenDistance::OptRes WaypointOptimizerOpenDistance::optimize(const WaypointOptimizerStrategyBase::DistMatrixT &mx) const
{
    boost::array<size_t, 5> idx_wpt = {0, 0, 0, 0, 0};
    unsigned int currdist = 0;

    // brute force for the moment
    // maybe we can use the travveling salesman approache from the boost::graph later on
    for(size_t i0=0; i0<mx.size1(); ++i0)
    {
        std::cout << "WaypointOptimizerOpenDistance::optimize " << i0 << " of " << mx.size1() << std::endl;
        for(size_t i1=i0+1; i1<mx.size1(); ++i1)
            for(size_t i2=i1+1; i2<mx.size1(); ++i2)
                for(size_t i3=i2+1; i3<mx.size1(); ++i3)
                    for(size_t i4=i3+1; i4<mx.size1(); ++i4)
                    {
                        unsigned int newdist = mx(i0, i1) + mx(i1, i2) + mx(i2, i3) + mx(i3, i4);
                        if(newdist > currdist)
                        {
                            std::cout << "dist(" << i0 << "," << i1 << "," << i2 << "," << i3 << "," << i4 << ") = " << newdist << std::endl;
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
WaypointOptimizerRichi::OptRes WaypointOptimizerRichi::optimize(const WaypointOptimizerStrategyBase::DistMatrixT &mx) const
{
    boost::array<size_t, 5> idx_wpt = {0, 0, 0, 0, 0};
    unsigned int currdist = 0;

    // brute force for the moment
    // maybe we can use the travveling salesman approache from the boost::graph later on
    size_t i0 = 0;
    size_t i4 = mx.size1()-1;
    for(size_t i1=i0+1; i1<mx.size1(); ++i1)
    {
        std::cout << "WaypointOptimizerRichi::optimize " << i1 << " of " << mx.size1() << std::endl;
        for(size_t i2=i1+1; i2<mx.size1(); ++i2)
            for(size_t i3=i2+1; i3<mx.size1(); ++i3)
                {
                    unsigned int newdist = mx(i0, i1) + mx(i1, i2) + mx(i2, i3) + mx(i3, i4);
                    if(newdist > currdist)
                    {
                        std::cout << "dist(" << i0 << "," << i1 << "," << i2 << "," << i3 << "," << i4 << ") = " << newdist << std::endl;
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



