// flugbuch
#include "StatFlightReport.h"
#include "FormatStr.h"
// witty
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WApplication>
// boost
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
// standard library
#include <algorithm>

using namespace flbwt;
using Wt::WContainerWidget;
using Wt::WText;
using boost::shared_ptr;
using boost::lexical_cast;
using std::string;
using std::vector;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void StatFlightReport::draw(Wt::WContainerWidget *parent, const flb::FlightDatabase::SeqFlights &flights) const
{
    Wt::WTable *table = new Wt::WTable(parent);

    string captions[8] = {"Nr",         "Datum", "Gleitschirm", "Startplatz",
                          "Landeplatz", "Dauer", "Hoehendiff",  "Strecke"};
    for(size_t i=0; i<sizeof(captions) / sizeof(string); ++i)
    {
        WText *labelText = new WText(captions[i]);
        labelText->setStyleClass("tableHeader");
        table->elementAt(0, i)->addWidget(labelText);
    }
    table->rowAt(0)->setStyleClass("title");

    size_t row = 1;
    for(flb::FlightDatabase::SeqFlights::const_iterator it = flights.begin(); it != flights.end(); ++it, ++row)
    {
        vector<string> vsText;

        vsText.push_back(lexical_cast<string>((*it)->number()));
        try
        {
            const string sloc = Wt::WApplication::instance()->locale();
            std::locale loc(sloc.c_str());
            vsText.push_back(FormatStr(loc) << (*it)->date());
        }
        catch(std::exception &ex)
        {
            vsText.push_back(FormatStr() << (*it)->date());
        }
        vsText.push_back((*it)->glider()->identity());
        vsText.push_back((*it)->takeoff()->identity());
        vsText.push_back((*it)->landing()->identity());
        vsText.push_back(FormatStr() << (*it)->duration());
        vsText.push_back(FormatStr() << ((*it)->takeoff()->height() - (*it)->landing()->height()) << "m");
        vsText.push_back(FormatStr() << (*it)->distance() << " km");
        // add the text widgets
        for(size_t i=0; i<vsText.size(); ++i)
        {
            WText *wtxt = new WText(vsText[i]);
            wtxt->setStyleClass("tableContent");
            table->elementAt(row, i)->addWidget(wtxt);
        }
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

