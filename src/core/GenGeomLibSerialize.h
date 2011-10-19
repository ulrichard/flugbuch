#ifndef GENGEOMLIBSERIALIZE_H_INCLUDED
#define GENGEOMLIBSERIALIZE_H_INCLUDED

// ggl (boost sandbox)
#include <boost/geometry/extensions/gis/latlong/latlong.hpp>
// boost
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/nvp.hpp>


namespace boost
{
namespace serialization
{

    template <class Archive>
    void save(Archive &ar, const boost::geometry::model::ll::point<> &pos, const unsigned int version)
    {
		double lat(pos.lat()), lon(pos.lon());
		ar & BOOST_SERIALIZATION_NVP(lat);
		ar & BOOST_SERIALIZATION_NVP(lon);
    }

    template <class Archive>
    void load(Archive &ar, boost::geometry::model::ll::point<> &pos, const unsigned int version)
    {
		double lat, lon;
		ar & BOOST_SERIALIZATION_NVP(lat);
		ar & BOOST_SERIALIZATION_NVP(lon);
		pos = boost::geometry::model::ll::point<>(geometry::latitude<>(lat), geometry::longitude<>(lon));
    }

} // namespace serialization
} // namespace boost
BOOST_SERIALIZATION_SPLIT_FREE(boost::geometry::model::ll::point<>)


#endif // GENGEOMLIBSERIALIZE_H_INCLUDED
