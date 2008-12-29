#ifndef GLIDERTABLE_H
#define GLIDERTABLE_H

// flugbuch
#include "CoreStructures.h"
#include "FlightDatabase.h"
#include "DetailTableBase.h"
// witty
#include <Wt/WTable>

// some forward declarations
namespace Wt
{
    namespace Ext
    {
        class ComboBox;
        class NumberField;
        class LineEdit;
    }
    class WImage;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class GliderTableRow : public DetailTableRowBase
{
public:
	GliderTableRow(shared_ptr<flb::Glider> glider, Wt::WTable *table, const boost::shared_ptr<flb::FlightDatabase>  flightDb, size_t rowNr, bool newEntry)
	: DetailTableRowBase(table, flightDb, rowNr, newEntry), glider_(glider) {  };
	virtual ~GliderTableRow() {  }

	virtual void show();
	virtual void edit();
	virtual void save();
	virtual void remove();

private:
	virtual void clearRow();

	shared_ptr<flb::Glider> glider_;
	// gui elements
	Wt::Ext::LineEdit    *edBrand_;
	Wt::Ext::LineEdit    *edModel_;
	Wt::Ext::LineEdit    *edSize_;
    Wt::Ext::LineEdit    *edColor_;
    Wt::Ext::NumberField *edYear_;
    Wt::Ext::LineEdit    *edClassi_;
    Wt::Ext::LineEdit    *edDesc_;
    Wt::WImage           *img_;

	enum ColPositons
	{
	    colOp = 0,
        colBrand,
        colModel,
        colSize,
        colColor,
        colYear,
        colClassi,
        colDesc,
        colImg
	};
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class GliderTable : public DetailTableBase
{
public:
	GliderTable(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
	virtual ~GliderTable() {  }

    virtual void loadPage(unsigned int page);
	GliderTableRow * addGlider(shared_ptr<flb::Glider> gl, size_t row, bool newEntry = false);
    void addNewGlider();
    virtual unsigned int entriesCount() const { return gliders_.size(); };
    void filter(const std::string &brand, const std::string &classification);

private:
	void createHeaderRow();
	void createFooterRow();

	std::vector<boost::shared_ptr<flb::Glider> > gliders_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class GliderPanel : public Wt::WCompositeWidget
{
public:
    GliderPanel(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
    virtual ~GliderPanel() { }

    void load();
    void filter();
private:
    boost::shared_ptr<flb::FlightDatabase>  flightDb_;
    GliderTable          *table_;
    Wt::WContainerWidget *impl_;
    Wt::Ext::ComboBox    *cbBrand_;
    Wt::Ext::ComboBox    *cbClassi_;
	PagesList            *pglist_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}
#endif // GLIDERTABLE_H
