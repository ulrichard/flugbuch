// flugbuch
#include "DetailTableBase.h"
#include "FormatStr.h"
// witty
#include <Wt/WText>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
// std lib
#include <stdexcept>

using namespace flbwt;
using namespace flb;
using std::string;
using boost::shared_ptr;
using boost::lexical_cast;
using Wt::WText;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
DetailTableRowBase::DetailTableRowBase(Wt::WTable *table, const boost::shared_ptr<flb::FlightDatabase>  flightDb, size_t rowNr, bool newEntry)
	: rowNr_(rowNr), isNewEntry_(newEntry), table_(table), flightDb_(flightDb)
{
    table_->rowAt(rowNr)->setStyleClass(rowNr % 2 ? "odd" : "even");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
PagesList::PagesList(DetailTableBase *table)
 : Wt::WCompositeWidget(0), impl_(new Wt::WContainerWidget()), table_(table)
{
    setImplementation(impl_);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void PagesList::load()
{
    impl_->clear();

    // item count
    WText *wtitemt = new WText("Eintraege : ", impl_);
	wtitemt->setStyleClass("tableFilter");
    WText *wtitems = new WText(lexical_cast<string>(table_->entriesCount()), impl_);
	wtitems->setStyleClass("pageNumbersCurr");

    // pages
    WText *wtxtpg = new WText("  Seiten : ", impl_);
	wtxtpg->setStyleClass("tableFilter");
	const unsigned int pagesCount = 1 + table_->entriesCount() / table_->entriesPerPage();
	if(pagesCount > 1 && table_->pageNr() > 1)
	{
        WText *previous = new WText("<", impl_);
        previous->setStyleClass("pageNumbers");
        previous->setToolTip("vorherige");
        previous->clicked.connect(SLOT(this, PagesList::select));
	}
    for(unsigned int i=0; i<pagesCount; ++i)
    {
        WText *wtxt = new WText(lexical_cast<string>(i+1), impl_);
        if(i + 1 == table_->pageNr())
            wtxt->setStyleClass("pageNumbersCurr");
        else
        {
            wtxt->setStyleClass("pageNumbers");
            wtxt->clicked.connect(SLOT(this, PagesList::select));
        }
    }
	if(pagesCount > 1 && table_->pageNr() < pagesCount)
    {
        WText *next = new WText(">", impl_);
        next->setStyleClass("pageNumbers");
        next->setToolTip("nächste");
        next->clicked.connect(SLOT(this, PagesList::select));
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void PagesList::select(Wt::WMouseEvent mev)
{
    // first, we have to find out which text was clicked
    Wt::WObject *wobj = sender();
    Wt::WText *wtxt = dynamic_cast<Wt::WText*>(wobj);
    if(!wtxt)
        return;

    const string txt = wtxt->text().narrow();
    if(txt == "<" || txt == "&lt;")
        table_->loadPage(table_->pageNr() - 1);
    else if(txt == ">" || txt == "&gt;")
        table_->loadPage(table_->pageNr() + 1);
    else
        table_->loadPage(lexical_cast<unsigned int>(txt));

    // reflect the change on the pagelist itself
    load();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
DetailTableBase::DetailTableBase(shared_ptr<FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WTable(parent), flightDb_(flightDb), entriesPerPage_(100), pageNr_(1), insertRowNr_(100)
{
	setStyleClass("tableFlights");
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
DetailTableBase::~DetailTableBase()
{
    pageNr_ = 1; // only making the linker happy here. http://gcc.gnu.org/faq.html#vtables
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A


