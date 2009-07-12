#ifndef WAYPOINTOPTIMIZER_H_INCLUDED
#define WAYPOINTOPTIMIZER_H_INCLUDED

// ggl (boost sandbox)
#include <geometry/geometries/latlong.hpp>
#include <geometry/algorithms/distance.hpp>
// boost
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// forward declarations
class WaypointOptimizerStrategyBase;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class WaypointOptimizer : public boost::noncopyable
{
public:
    WaypointOptimizer() {}
    virtual ~WaypointOptimizer() { delete dist_mx_; }
    static void addOptStrategy(WaypointOptimizerStrategyBase *strat) { strategies_.push_back(strat); }

    template<class InputIteratorT>
    void initialize(InputIteratorT begin, InputIteratorT end)
    {
        // calculate the distances between all waypoints
        const size_t wpt_count = std::distance(begin, end);
        dist_mx_ = new boost::numeric::ublas::symmetric_matrix<double>(wpt_count);

        size_t i = 0, j = 0;
        for(InputIteratorT it1 = begin; it1 != end; ++it1, ++i)
            for(InputIteratorT it2 = begin; it2 != it1; ++it2, ++j)
                (*dist_mx_)(i, j) = geometry::distance(*it1, *it2);
    }

private:
    static boost::ptr_vector<WaypointOptimizerStrategyBase> strategies_;
    boost::numeric::ublas::symmetric_matrix<double> *dist_mx_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class WaypointOptimizerStrategyBase
{
public:
    WaypointOptimizerStrategyBase(std::string name) : name_(name) {}
    virtual ~WaypointOptimizerStrategyBase() {}

    typedef std::vector<size_t> OptRes;
 //   template <class MatrixT>
//    virtual OptRes optimize(const MatrixT &mx) = 0;

    const std::string & name() { return name_; }

private:
    std::string name_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class WaypointOptimizerOpenDistance : public WaypointOptimizerStrategyBase
{
public:
    WaypointOptimizerOpenDistance() : WaypointOptimizerStrategyBase("OpenDistance") {}
    virtual ~WaypointOptimizerOpenDistance() {}

    template <class MatrixT>
    OptRes optimize(const MatrixT &mx);

private:
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class WaypointOptimizerRichi : public WaypointOptimizerStrategyBase
{
public:
    WaypointOptimizerRichi() : WaypointOptimizerStrategyBase("Richi") {}
    virtual ~WaypointOptimizerRichi() {}

    template <class MatrixT>
    OptRes optimize(const MatrixT &mx);

private:
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
} //namespace flb
#endif // WAYPOINTOPTIMIZER_H_INCLUDED
