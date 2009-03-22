// witty
#include <Wt/WGeoPosEdit>
#include <Wt/WGoogleMap>
#include <Wt/WCompositeWidget>
#include <Wt/WContainerWidget>
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/NumberField>
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/Button>
#include <Wt/Ext/Dialog>
#include <Wt/WHBoxLayout>
#include <Wt/WImage>
// boost
// standard library
#include <cmath>
#include <iostream>

using std::string;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

using namespace Wt;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
WGeoPosEdit::WGeoPosEdit(WContainerWidget *parent, WGeoPosEdit::PositionFormat fmt)
  : Wt::WCompositeWidget(parent), format_(fmt), impl_(new Wt::WContainerWidget())
{
    cbNoSo_ = cbEaWe_ = NULL;
    nfLatDeg_ = nfLatMin_ = nfLatSec_ = nfLonDeg_ = nfLonMin_ = nfLonSec_ = nfGridX_ = nfGridY_ = NULL;
    leZone_ = NULL;


    switch(format_)
    {
        case WGS84_SEC:
            nfLatSec_ = new Wt::Ext::NumberField();
            nfLonSec_ = new Wt::Ext::NumberField();
            // no break
        case WGS84_MIN:
            nfLatMin_ = new Wt::Ext::NumberField();
            nfLonMin_ = new Wt::Ext::NumberField();
            // no break
        case WGS84_DEC:
            nfLatDeg_ = new Wt::Ext::NumberField();
            nfLonDeg_ = new Wt::Ext::NumberField();
            cbNoSo_   = new Wt::Ext::ComboBox();
            cbNoSo_->addItem("N");
            cbNoSo_->addItem("S");
            cbNoSo_->setEditable(false);
            cbEaWe_   = new Wt::Ext::ComboBox();
            cbEaWe_->addItem("E");
            cbEaWe_->addItem("W");
            cbEaWe_->setEditable(false);
            break;
        case WGS84_UTM:
            leZone_   = new Wt::Ext::LineEdit();
            // no break
        case SWISSGRID:
            nfGridX_  = new Wt::Ext::NumberField();
            nfGridY_  = new Wt::Ext::NumberField();
            break;
        default:
            assert(!"unknown position format");
    }

    setImplementation(impl_);
    impl_->setLayout(new Wt::WHBoxLayout());
    // here, the order is important
    Wt::WWidget *widgets[] = {cbNoSo_, nfLatDeg_, nfLatMin_, nfLatSec_,
                              cbEaWe_, nfLonDeg_, nfLonMin_, nfLonSec_, leZone_, nfGridX_, nfGridY_};
    for(size_t i=0; i<sizeof(widgets)/sizeof(Wt::WWidget*); ++i)
        if(Wt::WWidget *widget = widgets[i])
        {
            impl_->layout()->addWidget(widget);
            if(Wt::Ext::LineEdit *le = dynamic_cast<Wt::Ext::LineEdit*>(widget))
                le->keyWentUp().connect(SLOT(this, WGeoPosEdit::keyUp));
        }

    mapIcon_ = new Wt::WImage("resources/slider-thumb-v.gif"); // we might want to add an image that's better suited to wt/resources
    impl_->layout()->addWidget(mapIcon_);
    mapIcon_->setToolTip("pick position from map");
    mapIcon_->clicked().connect(SLOT(this, WGeoPosEdit::showMap));

    switch(format_)
    {
        case WGS84_SEC:
            cbNoSo_->resize(13, cbNoSo_->height());
            cbEaWe_->resize(13, cbEaWe_->height());
            nfLatDeg_->resize(20, nfLatDeg_->height());
            nfLonDeg_->resize(20, nfLonDeg_->height());
            nfLatMin_->resize(20, nfLatMin_->height());
            nfLonMin_->resize(20, nfLonMin_->height());
            nfLatSec_->resize(40, nfLatSec_->height());
            nfLonSec_->resize(40, nfLonSec_->height());
            nfLatDeg_->setDecimalPrecision(0);
            nfLonDeg_->setDecimalPrecision(0);
            nfLatMin_->setDecimalPrecision(0);
            nfLonMin_->setDecimalPrecision(0);
            nfLatSec_->setDecimalPrecision(2);
            nfLonSec_->setDecimalPrecision(2);
            impl_->resize(380, impl_->height());
            break;
        case WGS84_MIN:
            cbNoSo_->resize(13, cbNoSo_->height());
            cbEaWe_->resize(13, cbEaWe_->height());
            nfLatDeg_->resize(20, nfLatDeg_->height());
            nfLonDeg_->resize(20, nfLonDeg_->height());
            nfLatMin_->resize(40, nfLatMin_->height());
            nfLonMin_->resize(40, nfLonMin_->height());
            nfLatDeg_->setDecimalPrecision(0);
            nfLonDeg_->setDecimalPrecision(0);
            nfLatMin_->setDecimalPrecision(3);
            nfLonMin_->setDecimalPrecision(3);
            impl_->resize(260, impl_->height());
            break;
        case WGS84_DEC:
            cbNoSo_->resize(13, cbNoSo_->height());
            cbEaWe_->resize(13, cbEaWe_->height());
            nfLatDeg_->resize(60, nfLatDeg_->height());
            nfLonDeg_->resize(60, nfLonDeg_->height());
            nfLatDeg_->setDecimalPrecision(4);
            nfLonDeg_->setDecimalPrecision(4);
            impl_->resize(220, impl_->height());
            break;
        case WGS84_UTM:
            leZone_->resize(40, leZone_->height());
            // no break
        case SWISSGRID:
            nfGridX_->resize(80, nfGridX_->height());
            nfGridY_->resize(80, nfGridY_->height());
            nfGridX_->setDecimalPrecision(3);
            nfGridY_->setDecimalPrecision(3);
            impl_->resize(200, impl_->height());
            break;
        default:
            assert(!"unknown position format");
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WGeoPosEdit::setPos(std::pair<double, double> pos)
{
    double tmpval;

    cout << "WGeoPosEdit::setPos(" << pos.first << ";" << pos.second << ") -> ";

    if(format_ == WGS84_SEC || format_ == WGS84_MIN || format_ == WGS84_DEC)
    {
        cbNoSo_->setCurrentIndex(pos.first >= 0.0 ? 0 : 1);
        cbNoSo_->refresh();
        pos.first = fabs(pos.first);
        cbEaWe_->setCurrentIndex(pos.second >= 0.0 ? 0 : 1);
        cbEaWe_->refresh();
        pos.second = fabs(pos.second);
    }

    switch(format_)
    {
        case WGS84_SEC:
            nfLatDeg_->setValue(floor(pos.first));
            tmpval = (pos.first - floor(pos.first)) * 60.0;
            nfLatMin_->setValue(floor(tmpval));
            nfLatSec_->setValue((tmpval - floor(tmpval)) * 60.0);
            nfLonDeg_->setValue(floor(pos.second));
            tmpval = (pos.second - floor(pos.second)) * 60.0;
            nfLonMin_->setValue(floor(tmpval));
            nfLonSec_->setValue((tmpval - floor(tmpval)) * 60.0);
            break;
        case WGS84_MIN:
            nfLatDeg_->setValue(floor(pos.first));
            nfLatMin_->setValue((pos.first - floor(pos.first)) * 60.0);
            nfLonDeg_->setValue(floor(pos.second));
            nfLonMin_->setValue((pos.second - floor(pos.second)) * 60.0);
            break;
        case WGS84_DEC:
            nfLatDeg_->setValue(pos.first);
            nfLonDeg_->setValue(pos.second);
            break;
        case WGS84_UTM:
            throw std::logic_error("not implemented yet");
            break;
        case SWISSGRID:
            {
                const double zerox  =  26782.5  / 3600;
                const double zeroy  = 169028.66 / 3600;
                const double xt = (pos.second - zerox) * 3600.0 / 10000.0;
                const double yt = (pos.first  - zeroy) * 3600.0 / 10000.0;

                double xc  = 600.07237 * 1000;
                xc += 211455.93 * xt;
                xc -=  10938.51 * xt * yt;
                xc -=      0.36 * xt * yt * yt;
                xc -=     44.54 * xt * xt * xt;
                xc /= 1000;

                double yc  = 200.14707 * 1000;
                yc += 308807.95 * yt;
                yc +=   3745.25 * xt * xt;
                yc +=     76.63 * yt * yt;
                yc -=    194.56 * xt * xt * yt;
                yc +=    119.79 * yt * yt * yt;
                yc /= 1000;

                nfGridX_->setValue(xc);
                nfGridY_->setValue(yc);
            }
            break;
        default:
            assert(!"unknown position format");
    }

    reportFields(cout);
    cout << endl;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<double, double> WGeoPosEdit::pos() const
{
    const double NorthSouth = (cbNoSo_ && cbNoSo_->currentIndex() ? -1.0 : 1.0);
    const double EastWest   = (cbEaWe_ && cbEaWe_->currentIndex() ? -1.0 : 1.0);
    pair<double, double> ppair = make_pair(0.0, 0.0);

    switch(format_)
    {
        case WGS84_SEC:
            ppair = make_pair(NorthSouth * ((nfLatSec_->value() / 60.0 + nfLatMin_->value()) / 60.0 + nfLatDeg_->value()),
                              EastWest   * ((nfLonSec_->value() / 60.0 + nfLonMin_->value()) / 60.0 + nfLonDeg_->value()));
            break;
        case WGS84_MIN:
        {
            double val1 = nfLatMin_->value() / 60.0;
            val1 += nfLatDeg_->value();
            val1 *= NorthSouth;
            double val2 = nfLonMin_->value() / 60.0;
            val2 += nfLonDeg_->value();
            val2 *= EastWest;
        }
            ppair = make_pair(NorthSouth * (nfLatMin_->value() / 60.0 + nfLatDeg_->value()),
                              EastWest   * (nfLonMin_->value() / 60.0 + nfLonDeg_->value()));
            break;
        case WGS84_DEC:
            ppair = make_pair(NorthSouth * nfLatDeg_->value(),
                              EastWest   * nfLonDeg_->value());
            break;
        case WGS84_UTM:
            throw std::logic_error("not implemented");
        case SWISSGRID:
        {
            double xval = nfGridX_->value();
            double yval = nfGridY_->value();
            if(!nfGridX_->value() && !nfGridY_->value())
                ppair = make_pair(0.0, 0.0);
            else
            {
                const double zerox  =  26782.5  / 3600;
                const double zeroy  = 169028.66 / 3600;
                const double xt = (nfGridX_->value() - 600) / 1000;
                const double yt = (nfGridY_->value() - 200) / 1000;

                double dx = 2.6779094;
                dx += 4.728982 * xt;
                dx += 0.791484 * xt * yt;
                dx += 0.1306   * xt * yt * yt;
                dx -= 0.0436   * xt * xt * xt;

                double dy = 16.9023892;
                dy +=  3.238272 * yt;
                dy -= 0.270978  * xt * xt;
                dy -= 0.002528  * yt * yt;
                dy -= 0.0447    * xt * xt * yt;
                dy -= 0.0140    * yt * yt * yt;

                ppair = make_pair(dy * 100.0 / 36.0, dx * 100.0 / 36.0);
            }
            break;
        }
        default:
            assert(!"unknown position format");
    }

    cout << "WGeoPosEdit::pos() ";
    reportFields(cout);
    cout << " -> (" << ppair.first <<";" << ppair.second << ")" << endl;

    return ppair;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
string WGeoPosEdit::format(pair<double, double> pos, Wt::WGeoPosEdit::PositionFormat fmt)
{
    std::stringstream sstr;
    double tmpval;
    switch(fmt)
    {
        case WGS84_SEC:
            sstr << (pos.first >= 0.0 ? "N " : "S ");
            tmpval = fabs(pos.first);
            sstr << floor(tmpval) << "° ";
            tmpval = (tmpval - floor(tmpval)) * 60.0;
            sstr << floor(tmpval) << "' ";
            sstr << ((tmpval - floor(tmpval)) * 60.0) << "\"   ";
            sstr << (pos.second >= 0.0 ? "O " : "W ");
            tmpval = pos.second;
            sstr << floor(tmpval) << "° ";
            tmpval = (tmpval - floor(tmpval)) * 60.0;
            sstr << floor(tmpval) << "' ";
            sstr << ((tmpval - floor(tmpval)) * 60.0) << "\"";
            break;
        case WGS84_MIN:
            sstr << (pos.first >= 0.0 ? "N " : "S ");
            tmpval = fabs(pos.first);
            sstr << floor(tmpval) << "° ";
            sstr << ((tmpval - floor(tmpval)) * 60.0) << "'   ";
            sstr << (pos.second >= 0.0 ? "O " : "W ");
            tmpval = pos.second;
            sstr << floor(tmpval) << "° ";
            sstr << ((tmpval - floor(tmpval)) * 60.0) << "' ";
            break;
        case WGS84_DEC:
            sstr << (pos.first >= 0.0 ? "N " : "S ");
            sstr << pos.first << "° ";
            sstr << (pos.second >= 0.0 ? "O " : "W ");
            sstr << pos.second << "° ";
            break;
        case WGS84_UTM:
            throw std::logic_error("not implemented");
            break;
        case SWISSGRID:
            throw std::logic_error("not implemented");
            break;
        default:
            assert(!"unknown position format");
    }
    return sstr.str();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
string WGeoPosEdit::formatDescription(PositionFormat fmt)
{
    switch(fmt)
    {
        case WGS84_SEC:
            return "WGS84 in degrees, minutes and decimal seconds";
        case WGS84_MIN:
            return "WGS84 in degrees and decimal minutes";
        case WGS84_DEC:
            return "WGS84 in decimal degrees";
        case WGS84_UTM:
            return "UTM (Universal Transverse Mercator)";
        case SWISSGRID:
            return "SwissGrid";
        default:
            assert(!"unknown position format");
            return "";
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
std::ostream & WGeoPosEdit::reportFields(std::ostream &oss) const
{
    Wt::WWidget *widgets[] = {cbNoSo_, nfLatDeg_, nfLatMin_, nfLatSec_,
                              cbEaWe_, nfLonDeg_, nfLonMin_, nfLonSec_, leZone_, nfGridX_, nfGridY_};
    for(size_t i=0; i<sizeof(widgets)/sizeof(Wt::WWidget*); ++i)
        if(Wt::WWidget *widget = widgets[i])
            if(Wt::Ext::LineEdit *le = dynamic_cast<Wt::Ext::LineEdit*>(widget))
                oss << le->text() << " ";
    return oss;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WGeoPosEdit::showMap()
{
    mapDlg_ = new Wt::Ext::Dialog("Doubleclick at the new location");
    mapDlg_->resize(508, 440);
    mapDlg_->setSizeGripEnabled(false);
    Wt::WGoogleMap *gmap = new Wt::WGoogleMap();
    mapDlg_->contents()->addWidget(gmap);
	gmap->resize(500, 380);
	gmap->enableScrollWheelZoom();
	gmap->disableDoubleClickZoom();
	gmap->enableDragging();
	gmap->addHierarchicalMapTypeControl();
    gmap->enableGoogleBar();
    gmap->doubleClicked().connect(SLOT(this, WGeoPosEdit::setPosFromDlg));
    const pair<double, double> pdpos = pos();
    if(pdpos.first != 0.0 && pdpos.second != 0.0)
    {
        WGoogleMap::Coordinate lalo(pdpos.first, pdpos.second);
        gmap->setCenter(lalo, 13);
        gmap->addMarker(lalo);
    }
    Wt::Ext::Button *btnCancel = new Wt::Ext::Button("Cancel");
    mapDlg_->addButton(btnCancel);
    btnCancel->clicked().connect(SLOT(this, WGeoPosEdit::closeDlg));

    mapDlg_->show();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WGeoPosEdit::setPosFromDlg(WGoogleMap::Coordinate pos)
{
    assert(mapDlg_);

    setPos(make_pair(pos.latitude(), pos.longitude()));
    changed_.emit();

    mapDlg_->accept();
    delete mapDlg_;
    mapDlg_ = NULL;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WGeoPosEdit::closeDlg()
{
    assert(mapDlg_);
    mapDlg_->accept();
    delete mapDlg_;
    mapDlg_ = NULL;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WGeoPosEdit::keyUp(const Wt::WKeyEvent &kev)
{
    changed_.emit();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
