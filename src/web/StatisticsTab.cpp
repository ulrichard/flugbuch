// flugbuch
#include "StatisticsTab.h"
// witty
#include <Wt/Ext/ComboBox>
#include <Wt/WContainerWidget>
#include <Wt/Chart/WPieChart>
#include <Wt/WHBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WText>
#include <Wt/WStandardItemModel>
// boost
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
// standard library
#include <algorithm>
#include <map>

using std::string;
using std::map;
using std::for_each;
using boost::shared_ptr;
using boost::any;
using namespace boost::lambda;
using namespace flbwt;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
StatisticsPanel::StatisticsPanel(boost::shared_ptr<flb::FlightDatabase>  flightDb, Wt::WContainerWidget *parent)
 : Wt::WCompositeWidget(parent), flightDb_(flightDb), impl_(new Wt::WContainerWidget())
{
    setImplementation(impl_);
    chart_= NULL;

    // header
    Wt::WContainerWidget *topBar = new Wt::WContainerWidget();
    topBar->setLayout(new Wt::WHBoxLayout());
    Wt::WText *wtStat = new Wt::WText("Auswertung");
    wtStat->setStyleClass("FilterSubTitle");

    cbStatSel_ = new Wt::Ext::ComboBox();
    cbStatSel_->activated.connect(SLOT(this, StatisticsPanel::load));

    cbStatSel_->addItem("Fluege pro Schirm");
    cbStatSel_->addItem("Flugzeit pro Schirm");

    topBar->layout()->addWidget(wtStat);
    topBar->layout()->addWidget(cbStatSel_);
    topBar->resize(topBar->width(), 40);
    cbStatSel_->resize(150, cbStatSel_->height());

    blayout_ = new Wt::WBorderLayout();
    impl_->setLayout(blayout_);
    blayout_->addWidget(topBar, Wt::WBorderLayout::North);


}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void StatisticsPanel::load(int ind)
{
    FlightsPerGlider(ind);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void StatisticsPanel::FlightsPerGlider(bool airtime)
{
    blayout_->removeWidget(chart_);
 //   delete chart_;
    Wt::Chart::WPieChart *pie = new Wt::Chart::WPieChart();
    blayout_->addWidget(pie, Wt::WBorderLayout::Center);
    chart_ = pie;

    Wt::WStandardItemModel *model = new  Wt::WStandardItemModel(flightDb_->gliders().size(), 3);


    int i = 0;
    for(flb::FlightDatabase::SeqGliders::iterator it = flightDb_->gliders().begin(); it != flightDb_->gliders().end(); ++it, ++i)
    {
        string nam = (*it)->identity();
/*
        int    cnt = std::count_if(flightDb_->flights().begin(), flightDb_->flights().end(),
            *it == boost::bind(&flb::Flight::glider, ::_1));
*/
        int cnt = 0, dur = 0;
        for(flb::FlightDatabase::SeqFlights::iterator itf = flightDb_->flights().begin(); itf != flightDb_->flights().end(); ++itf)
            if((*itf)->glider() == *it)
            {
                cnt++;
                dur += (*itf)->duration();
            }

        model->setData(i, 0, any(nam));
        model->setData(i, 1, any(cnt));
        model->setData(i, 2, any(dur));
    }

    pie->setModel(model);
    pie->setLabelsColumn(0);
    pie->setDataColumn(airtime ? 2 : 1);
    pie->setDisplayLabels(Wt::Chart::Outside | Wt::Chart::TextLabel | Wt::Chart::TextPercentage);
    pie->setPerspectiveEnabled(true, 0.3);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
