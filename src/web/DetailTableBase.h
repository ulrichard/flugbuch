#ifndef DETAILTABLEBASE_H
#define DETAILTABLEBASE_H
// flugbuch
#include "CoreStructures.h"
#include "FlightDatabase.h"
// witty
#include <Wt/WTable>
#include <Wt/WCompositeWidget>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/utility.hpp>

// some forward declarations
namespace Wt
{
    class WText;

    namespace Ext
    {
        class ComboBox;
        class NumberField;
        class LineEdit;
        class DateField;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace flbwt
{
class DetailTableBase;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// abstract base class for the rows of the detail tables
class DetailTableRowBase : public Wt::WObject, boost::noncopyable
{
public:
	DetailTableRowBase(Wt::WTable *table, const boost::shared_ptr<flb::FlightDatabase>  flightDb, size_t rowNr, bool newEntry);
	virtual ~DetailTableRowBase() {  }

	virtual void show()   = 0;
	virtual void edit()   = 0;
	virtual void save()   = 0;
	virtual void remove() = 0;

protected:
	virtual void   clearRow() = 0;

    const size_t rowNr_;
    bool         isNewEntry_;  // cancel results in deletion
	Wt::WTable   *table_;      // the parent
	const boost::shared_ptr<flb::FlightDatabase>  flightDb_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class PagesList : public Wt::WCompositeWidget
{
public:
    PagesList(DetailTableBase *table);
    void load();
    void select(Wt::WMouseEvent mev);

private:
    Wt::WContainerWidget   *impl_;
    DetailTableBase        *table_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class DetailTableBase : public Wt::WTable, boost::noncopyable
{
public:
	DetailTableBase(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent = 0);
	virtual ~DetailTableBase();

    virtual void loadPage(unsigned int page) = 0;
    // getters
    unsigned int entriesPerPage(void) const { return entriesPerPage_; }
    unsigned int pageNr(void)         const { return pageNr_; }
    virtual unsigned int entriesCount() const = 0;
protected:
//	virtual void createHeaderRow() = 0;
//	virtual void createFooterRow() = 0;

	boost::ptr_vector<DetailTableRowBase>   rows_;
	boost::shared_ptr<flb::FlightDatabase>  flightDb_;
	unsigned short                          entriesPerPage_;
	unsigned short                          pageNr_;
	unsigned short                          insertRowNr_;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}
#endif // DETAILTABLEBASE_H
