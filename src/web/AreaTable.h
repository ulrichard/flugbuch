#ifndef AREATABLE_H
#define AREATABLE_H

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
        class LineEdit;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class AreaTableRow : public DetailTableRowBase
{
public:
	AreaTableRow(boost::shared_ptr<flb::FlightArea> flar, Wt::WTable *table, const boost::shared_ptr<flb::FlightDatabase>  flightDb, size_t rowNr, bool newEntry)
	: DetailTableRowBase(table, flightDb, rowNr, newEntry), area_(flar) {  };
	virtual ~AreaTableRow() {  }

	virtual void show();
	virtual void edit();
	virtual void save();
	virtual void remove();

private:
	virtual void clearRow();

    boost::shared_ptr<flb::FlightArea> area_;
	// gui elements
	Wt::Ext::LineEdit *edName_;
	Wt::Ext::LineEdit *edCountry_;
	Wt::Ext::LineEdit *edDesc_;

	enum ColPositons
	{
	    colOp = 0,
        colName,
        colCountry,
        colDesc
    };
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class AreaTable : public DetailTableBase
{
public:
	AreaTable(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
	virtual ~AreaTable() {  }

    virtual void loadPage(unsigned int page);
	AreaTableRow * addArea(shared_ptr<flb::FlightArea> flar, size_t row, bool newEntry = false);
    void addNewArea();
    virtual unsigned int entriesCount() const { return areas_.size(); };
    void filter(const std::string &country);

private:
	void createHeaderRow();
	void createFooterRow();

	std::vector<boost::shared_ptr<flb::FlightArea> > areas_; // filtered and sorted
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class AreaPanel : public Wt::WCompositeWidget
{
public:
    AreaPanel(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
    virtual ~AreaPanel() { }

    void load();
    void filter();
private:
    boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    AreaTable            *table_;
    Wt::WContainerWidget *impl_;
    Wt::Ext::ComboBox    *cbCountry_;
	PagesList            *pglist_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
} // namespace flbwt
#endif // AREATABLE_H
