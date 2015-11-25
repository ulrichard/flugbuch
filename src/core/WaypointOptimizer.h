#ifndef WAYPOINTOPTIMIZER_H_INCLUDED
#define WAYPOINTOPTIMIZER_H_INCLUDED

// ggl (boost sandbox)
//#include <boost/geometry/extensions/gis/latlong/latlong.hpp>
#include <boost/geometry/algorithms/distance.hpp>
//#include <boost/geometry/extensions/gis/geographic/strategies/vincenty.hpp>
//#include <boost/geometry/strategies/spherical/distance_haversine.hpp>
// boost
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>
// standard library
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <iostream>


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class WaypointOptimizerStrategyBase
{
public:
    WaypointOptimizerStrategyBase(std::string name, double factor) : name_(name), factor_(factor) {}
    virtual ~WaypointOptimizerStrategyBase() {}

    typedef boost::numeric::ublas::symmetric_matrix<unsigned int> DistMatrixT;
    typedef std::vector<size_t> OptRes; // indices into the tracklog
    virtual OptRes optimize(const DistMatrixT &mx) const = 0;

    const std::string & name()   const { return name_; }
    double              factor() const { return factor_; }

private:
    std::string name_;
    double      factor_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class WaypointOptimizer : public boost::noncopyable
{
public:
    WaypointOptimizer();
    virtual ~WaypointOptimizer() { delete dist_mx_; }
    void addOptStrategy(WaypointOptimizerStrategyBase *strat) { strategies_.push_back(strat); }

    template<class InputIteratorT>
    void initialize(InputIteratorT begin, InputIteratorT end)
    {
        // calculate the distances between all waypoints, store in meters for faster calculation
        const size_t wpt_count = std::distance(begin, end);
        dist_mx_ = new WaypointOptimizerStrategyBase::DistMatrixT(wpt_count);

        size_t i = 0, j = 0;
        for(InputIteratorT it1 = begin; it1 != end; ++it1, ++i, j=0)
        {
 //           std::cout << "calc distance " << i << " of " << wpt_count << std::endl;
            for(InputIteratorT it2 = begin; it2 != it1; ++it2, ++j)
            {
//                (*dist_mx_)(i, j) = boost::geometry::distance(*it1, *it2, boost::geometry::strategy::distance::haversine<typename InputIteratorT::value_type>());
                // ToDo : re-enable distance calculation
                (*dist_mx_)(i, j) = 0.0;
            }
        }
    }

    typedef std::map<std::string, WaypointOptimizerStrategyBase::OptRes> OptMap;
    OptMap optimize() const;

private:
    boost::ptr_vector<WaypointOptimizerStrategyBase> strategies_;
    WaypointOptimizerStrategyBase::DistMatrixT *dist_mx_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class WaypointOptimizerNoWpt : public WaypointOptimizerStrategyBase
{
public:
    WaypointOptimizerNoWpt() : WaypointOptimizerStrategyBase("NoWaypoints", 1.0) {}
    virtual ~WaypointOptimizerNoWpt() {}

    virtual OptRes optimize(const DistMatrixT &mx) const;

private:
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class WaypointOptimizerOpenDistance : public WaypointOptimizerStrategyBase
{
public:
    WaypointOptimizerOpenDistance() : WaypointOptimizerStrategyBase("OpenDistance", 1.0) {}
    virtual ~WaypointOptimizerOpenDistance() {}

    virtual OptRes optimize(const DistMatrixT &mx) const;

private:
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class WaypointOptimizerRichi : public WaypointOptimizerStrategyBase
{
public:
    WaypointOptimizerRichi() : WaypointOptimizerStrategyBase("Richi", 0.0) {}
    virtual ~WaypointOptimizerRichi() {}

    virtual OptRes optimize(const DistMatrixT &mx) const;

private:
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} //namespace flb
#endif // WAYPOINTOPTIMIZER_H_INCLUDED
