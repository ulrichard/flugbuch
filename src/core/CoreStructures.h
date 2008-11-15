#ifndef CORESTRUCTURES_H
#define CORESTRUCTURES_H

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <utility>


using boost::shared_ptr;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flb
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class FlightArea
{
	friend class boost::serialization::access;
public:
    FlightArea(const std::string &name, const std::string &country, const std::string desc) : name_(name), country_(country), descript_(desc) {}
    FlightArea(const FlightArea &cpy) : name_(cpy.name_), country_(cpy.country_), descript_(cpy.descript_) {}
    ~FlightArea() {};
	const FlightArea & operator=(const FlightArea &flar) {FlightArea cpy(flar); std::swap(*this, cpy); return *this; };
	bool operator==(const FlightArea &flar) { return (flar.name_ == name_ && flar.country_ == country_); }
	bool operator<(const FlightArea &rhs)   { return (name_ < rhs.name_ || country_ < rhs.country_); }
    template<class Archive> void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(name_);
		ar & BOOST_SERIALIZATION_NVP(country_);
		ar & BOOST_SERIALIZATION_NVP(descript_);
	}
    // getters
	const std::string & name()        const { return name_; }
	const std::string & country()     const { return country_; }
	const std::string & description() const { return descript_; }
    // setters
	void setName(const std::string &nam)         { name_ = nam; }
	void setCountry(const std::string &cnt)      { country_ = cnt; }
	void setDescription(const std::string &desc) { descript_ = desc; }
private:
	FlightArea()  {}
	// members
    std::string name_;
    std::string country_;
    std::string descript_;
};
BOOST_SERIALIZATION_SHARED_PTR(FlightArea);
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class Location
{
	friend class boost::serialization::access;
public:
	Location(boost::shared_ptr<FlightArea> area, const std::string &name, unsigned short height, double lat, double lon, int usage) : area_(area), name_(name), height_(height), latitude_(lat), longitude_(lon), usageas_(usage) {}
	Location(const Location &cpy) :  name_(cpy.name_), height_(cpy.height_), latitude_(cpy.latitude_), longitude_(cpy.longitude_), usageas_(cpy.usageas_) {}
    ~Location() {};
	const Location & operator=(const Location &loc) { Location cpy(loc); std::swap(*this, cpy); return *this; };
	bool operator==(const Location &loc) { return (loc.name() == name_ && loc.area() == area_); }
	// getters
	const boost::shared_ptr<FlightArea> area() const { return area_; }
//	boost::shared_ptr<FlightArea>       getArea() { return area_; }
	const std::string & name()      const { return name_; }
	unsigned short      height()    const { return height_; }
	double              latitude()  const { return latitude_; }
	double              longitude() const { return longitude_; }
	std::pair<double, double> pos() const { return std::make_pair(latitude_, longitude_); }
	int                 usage()     const { return usageas_; }
	std::string         identity()  const { return area_->name() + " " + name_; }

	// setters
	void setArea(boost::shared_ptr<FlightArea> flar) { area_     = flar; }
	void setName(const std::string &nam)             { name_     = nam; }
	void setHeight(unsigned short height);
	void setPosition(double lat, double lon);
	void setPosition(std::pair<double, double> pos)  { setPosition(pos.first, pos.second); }
	void setUsage(int usg)                           { usageas_  = usg; }
	void addUsage(int usg)                           { usageas_ |= usg; }

	// calculation
	double getDistance(const Location &rhs) const;
	bool   isEquivalent(const Location &rhs) const;
	bool   isEquivalentSp(const boost::shared_ptr<Location> &rhs) const { return isEquivalent(*rhs); };

	template<class Archive> void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(area_);
		ar & BOOST_SERIALIZATION_NVP(name_);
		ar & BOOST_SERIALIZATION_NVP(height_);
		ar & BOOST_SERIALIZATION_NVP(latitude_);
		ar & BOOST_SERIALIZATION_NVP(longitude_);
		ar & BOOST_SERIALIZATION_NVP(usageas_);
	}

    enum UseAs
    {
        UA_TAKEOFF = 1 << 0,
        UA_LANDING = 1 << 1,
        UA_WAYPNT  = 1 << 2
    };


private:
	Location() {}
	// members
	boost::shared_ptr<FlightArea>  	area_;
	std::string 					name_;
	unsigned short					height_;
	double      					latitude_;
	double      					longitude_;
	int         					usageas_;
};
BOOST_SERIALIZATION_SHARED_PTR(Location);
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class Glider
{
	friend class boost::serialization::access;
public:
    Glider(const std::string &brand, const std::string &model, const std::string &size,
            const std::string &color, unsigned short year, const std::string &classi, const std::string &desc = "",
            const std::string &img = "") :
            brand_(brand), model_(model), size_(size), color_(color), year_(year), classi_(classi),
            desc_(desc), img_(img) {}
    Glider(const Glider &cpy) : brand_(cpy.brand_), model_(cpy.model_), size_(cpy.size_),
            color_(cpy.color_), year_(cpy.year_), classi_(cpy.classi_), desc_(cpy.desc_), img_(cpy.img_) {}
    ~Glider() {}
   const Glider & operator=(const Glider& gld) { Glider cpy(gld); std::swap(*this, cpy); return *this; }
   bool operator==(const Glider &gld) { return (gld.brand_ == brand_ && gld.model_ == model_); }
    template<class Archive> void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(brand_);
		ar & BOOST_SERIALIZATION_NVP(model_);
		ar & BOOST_SERIALIZATION_NVP(size_);
		ar & BOOST_SERIALIZATION_NVP(color_);
		ar & BOOST_SERIALIZATION_NVP(year_);
		ar & BOOST_SERIALIZATION_NVP(classi_);
		ar & BOOST_SERIALIZATION_NVP(desc_);
		ar & BOOST_SERIALIZATION_NVP(img_);
	}

    // getters
    const std::string & brand()          const { return brand_; }
    const std::string & model()          const { return model_; }
    const std::string & size()           const { return size_; }
    const std::string & color()          const { return color_; }
    unsigned short      year()           const { return year_; }
    const std::string & classification() const { return classi_; }
    const std::string & description()    const { return desc_; }
    const std::string & image()          const { return img_; }
	std::string         identity()       const { return brand_ + " " + model_; }

	// setters
	void setBrand(const std::string &txt) { brand_ = txt; }
	void setModel(const std::string &txt) { model_  = txt; }
	void setSize(const std::string &txt)  { size_  = txt; }
	void setColor(const std::string &txt) { color_ = txt; }
	void setYear(int val)                 { if(val < 1970 || val > 2050) throw std::range_error("Year out of range"); year_ = val; }
	void setClassification(const std::string &txt) { classi_ = txt; }
	void setDescription(const std::string &txt)    { desc_   = txt; }

private:
	Glider() {}
	// members
	std::string     brand_;
	std::string     model_;
    std::string     size_;
    std::string     color_;
    unsigned short  year_;
    std::string     classi_;
    std::string     desc_;
    std::string     img_;
};
BOOST_SERIALIZATION_SHARED_PTR(Glider);
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class Flight
{
	friend class boost::serialization::access;
public:
    typedef boost::gregorian::date dateT;
//	typedef int dateT;
	typedef std::vector<boost::shared_ptr<Location> > SeqWaypoints;
public:
	Flight(unsigned int number, const dateT &date, unsigned int airtime,
			boost::shared_ptr<Glider> glider, boost::shared_ptr<Location> takeoff,
            boost::shared_ptr<Location> landing, const SeqWaypoints &waypoints =
			std::vector<boost::shared_ptr<Location> >()):
			number_(number), date_(date), airtime_(airtime), glider_(glider), takeoff_(takeoff),
			landing_(landing), waypoints_(waypoints), distance_(0) {}
	Flight(const Flight &cpy) : number_(cpy.number_), date_(cpy.date_), airtime_(cpy.airtime_),
			glider_(cpy.glider_), takeoff_(cpy.takeoff_), landing_(cpy.landing_), waypoints_(cpy.waypoints_),
			distance_(cpy.distance_)	{}
	~Flight() {}
	// operators
	const Flight & operator=(const Flight &flt) { Flight cpy(flt); std::swap(*this, cpy); return *this; }
	bool operator==(const Flight &flt) { return (flt.number_ == number_); }
	bool operator <(const Flight &rhs) { return number_ < rhs.number(); }
	// serialization
    template<class Archive> void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(number_);
		ar & BOOST_SERIALIZATION_NVP(date_);
		ar & BOOST_SERIALIZATION_NVP(airtime_);
		ar & BOOST_SERIALIZATION_NVP(glider_);
		ar & BOOST_SERIALIZATION_NVP(takeoff_);
		ar & BOOST_SERIALIZATION_NVP(landing_);
		ar & BOOST_SERIALIZATION_NVP(waypoints_);
	}
	// getters
	unsigned int					  number()	  const { return number_; }
	const dateT	&					  date()	  const { return date_; }
	const boost::shared_ptr<Location> takeoff()	  const { return takeoff_; }
	const boost::shared_ptr<Location> landing()	  const { return landing_; }
	const SeqWaypoints  &             waypoints() const { return waypoints_; }
	const boost::shared_ptr<Glider>   glider()	  const { return glider_; }
	const unsigned int &			  duration()  const { return airtime_; }
	double							  distance()  const { return distance_; }
	bool							  hasTrack()  const { return false; }
	// setters
	void setNumber(unsigned int nbr)                { number_  = nbr; }
	void setDate(const dateT &ndate)                { date_    = ndate; }
	void setGlider(boost::shared_ptr<Glider> gld)   { glider_  = gld; }
    void setTakeoff(boost::shared_ptr<Location> to) { takeoff_ = to; }
    void setLanding(boost::shared_ptr<Location> la) { landing_ = la; }
    void setDuration(unsigned int dur)              { airtime_ = dur; }
    void setDistance(double dist)                   { distance_ = dist; }
    // waypoints
    void clearWaypoints()                               { waypoints_.clear(); }
    void addWaypoint(boost::shared_ptr<Location> wpt)   { waypoints_.push_back(wpt); }
    // misc
    double calcDistance() const;
 private:
	 Flight() {}
	 // members
	unsigned int				number_;
	dateT                      	date_;
	unsigned int               	airtime_;
	boost::shared_ptr<Glider>   glider_;
	boost::shared_ptr<Location> takeoff_;
	boost::shared_ptr<Location> landing_;
	SeqWaypoints   				waypoints_;
	double						distance_;
};
BOOST_SERIALIZATION_SHARED_PTR(Flight);
static bool operator<(const boost::shared_ptr<Flight> lhs, const boost::shared_ptr<Flight> rhs) { return *lhs < *rhs; }
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
enum PositionFormat
{
    WGS84_SEC,
    WGS84_MIN,
    WGS84_DEC,
    WGS84_UTM,
    SWISSGRID
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
} // namespace flb
#endif // CORESTRUCTURES_H
