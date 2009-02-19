#ifndef LOCATIONTABLE_H
#define LOCATIONTABLE_H

// flugbuch
#include "CoreStructures.h"
#include "FlightDatabase.h"
#include "DetailTableBase.h"
// witty
#include <Wt/WContainerWidget>
#include <Wt/WCompositeWidget>
#include <Wt/WTable>
#include <Wt/WPanel>
#include <Wt/WLatLng>
// std lib
#include <vector>
#include <string>

// some forward declarations
namespace Wt
{
    class WGoogleMap;
    class WGeoPosEdit;
    namespace Ext
    {
        class LineEdit;
        class ComboBox;
        class NumberField;
        class CheckBox;
        class Dialog;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class LocationTableRow : public DetailTableRowBase
{
public:
	LocationTableRow(shared_ptr<flb::Location> loc, Wt::WTable *table, const boost::shared_ptr<flb::FlightDatabase>  flightDb, size_t rowNr, bool newEntry)
	: DetailTableRowBase(table, flightDb, rowNr, newEntry), location_(loc) {  };
	virtual ~LocationTableRow() {  }

	virtual void show();
	virtual void edit();
	virtual void save();
	virtual void remove();

private:
	virtual void clearRow();
	void map(bool editable);
	void setPos(Wt::WLatLng pos);
	void closeDlg();

    boost::shared_ptr<flb::Location> location_;
	// gui elements
	Wt::Ext::ComboBox    *cbArea_;
	Wt::Ext::LineEdit    *edName_;
	Wt::Ext::NumberField *nfHeight_;
	Wt::WGeoPosEdit      *pfPosition_;
	Wt::Ext::CheckBox    *cbTakeoff_;
	Wt::Ext::CheckBox    *cbLanding_;
	Wt::Ext::CheckBox    *cbWayPnt_;
	Wt::Ext::Dialog      *mapDlg_;

	enum ColPositons
	{
	    colOp = 0,
        colArea,
        colName,
        colHeight,
        colPosition,
        colTakeoff,
        colLanding,
        colWaypnt
    };
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class LocationTable : public DetailTableBase
{
public:
	LocationTable(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
	virtual ~LocationTable() {  }

    virtual void loadPage(unsigned int page);
	LocationTableRow * addLocation(shared_ptr<flb::Location> loc, size_t row, bool newEntry = false);
    void addNewLocation();
    virtual unsigned int entriesCount() const { return locations_.size(); };
    void filter(const std::string &area, bool takeoff, bool landing, bool waypnt);

private:
	void createHeaderRow();
	void createFooterRow();
	std::vector<boost::shared_ptr<flb::Location> > locations_; // filtered and sorted
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class LocationPanel : public Wt::WCompositeWidget
{
public:
    LocationPanel(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
    virtual ~LocationPanel() { }

    void load();
    void filter();
private:
    boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    LocationTable        *table_;
    Wt::WContainerWidget *impl_;
    Wt::Ext::ComboBox    *cbArea_;
    Wt::Ext::CheckBox    *cbTakeoff_;
	Wt::Ext::CheckBox    *cbLanding_;
	Wt::Ext::CheckBox    *cbWayPnt_;
	PagesList            *pglist_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}

#endif // LOCATIONTABLE_H
