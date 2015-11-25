#ifndef GENGEOMLIBSERIALIZE_H_INCLUDED
#define GENGEOMLIBSERIALIZE_H_INCLUDED

// boost
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/nvp.hpp>


namespace boost
{
namespace serialization
{

    template <class Archive>
    void save(Archive& ar, const std::pair<double, double>& pos, const unsigned int version)
    {
		double lat(pos.first), lon(pos.second);
		ar & BOOST_SERIALIZATION_NVP(lat);
		ar & BOOST_SERIALIZATION_NVP(lon);
    }

    template <class Archive>
    void load(Archive& ar, std::pair<double, double>& pos, const unsigned int version)
    {
		double lat, lon;
		ar & BOOST_SERIALIZATION_NVP(lat);
		ar & BOOST_SERIALIZATION_NVP(lon);
		pos = std::make_pair(lat, lon);
    }

} // namespace serialization
} // namespace boost
typedef std::pair<double, double> point_ll_deg;
BOOST_SERIALIZATION_SPLIT_FREE(point_ll_deg)


#endif // GENGEOMLIBSERIALIZE_H_INCLUDED
