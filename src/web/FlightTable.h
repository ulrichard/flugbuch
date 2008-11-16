#ifndef FLIGHTLIST_WT
#define FLIGHTLIST_WT

// flugbuch
#include "CoreStructures.h"
#include "FlightDatabase.h"
#include "DetailTableBase.h"
// witty
#include <Wt/WContainerWidget>
#include <Wt/WCompositeWidget>
#include <Wt/WTable>
// std lib
#include <vector>
#include <string>

// some forward declarations
namespace Wt
{
    namespace Ext
    {
        class ComboBox;
        class NumberField;
        class DateField;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class LocationField : public Wt::WCompositeWidget
{
public:
    LocationField(const boost::shared_ptr<flb::FlightDatabase>  flightDb, const flb::Location::UseAs useAs, Wt::WContainerWidget *parent = 0);

    const boost::shared_ptr<flb::Location> getLocation() const;

public slots:
    void   fillAreas();
    size_t selectArea(const std::string &area);
    void   fillLocations(int idx);
    size_t selectLocation(const std::string &loc);

private:
    const boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    const flb::Location::UseAs useAs_;
    Wt::WContainerWidget *impl_;
    Wt::Ext::ComboBox    *cbArea_;
    Wt::Ext::ComboBox    *cbLocation_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class FlightTableRow : public DetailTableRowBase
{
public:
	FlightTableRow(shared_ptr<flb::Flight> flight, Wt::WTable *table, const boost::shared_ptr<flb::FlightDatabase>  flightDb, size_t rowNr, bool newEntry)
	: DetailTableRowBase(table, flightDb, rowNr, newEntry), flight_(flight) {  };
	virtual ~FlightTableRow() {  }

	virtual void show();
	virtual void edit();
	virtual void save();
	virtual void remove();

private:
	virtual void clearRow();

	shared_ptr<flb::Flight> flight_;
	// gui elements
	Wt::Ext::NumberField *nbrEdit_;
	Wt::Ext::DateField   *dateEdit_;
	Wt::Ext::ComboBox    *cbGlider_;
	LocationField        *lfTakeoff_;
	LocationField        *lfLanding_;
	Wt::Ext::NumberField *ndDuration_;


	enum ColPositons
	{
	    colOp = 0,
        colFlightNbr,
        colDate,
        colGlider,
        colTakeoff,
        colLanding,
        colDuration,
        colHeightDiff,
        colDistance
	};
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class FlightTable : public DetailTableBase
{
public:
	FlightTable(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
	virtual ~FlightTable() {  }

    virtual void loadPage(unsigned int page);
	FlightTableRow * addFlight(shared_ptr<flb::Flight> fl, size_t row, bool newEntry = false);
    void addNewFlight();
    virtual unsigned int entriesCount() const { return flights_.size(); };
    void filter(const std::string &area);

private:
	void createHeaderRow();
	void createFooterRow();
	std::vector<shared_ptr<flb::Flight> > flights_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class FlightPanel : public Wt::WCompositeWidget
{
public:
    FlightPanel(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
    virtual ~FlightPanel() { }

    void load();
    void filter();
private:
    boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    FlightTable          *table_;
    Wt::WContainerWidget *impl_;
    Wt::Ext::ComboBox    *cbArea_;
	PagesList            *pglist_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
} // namespace flbwt
#endif
