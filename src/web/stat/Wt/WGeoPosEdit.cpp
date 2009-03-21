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

using std::string;
using std::pair;
using std::make_pair;

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
            nfLatDeg_->resize(40, nfLatDeg_->height());
            nfLonDeg_->resize(40, nfLonDeg_->height());
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
            nfLatDeg_->setDecimalPrecision(3);
            nfLonDeg_->setDecimalPrecision(3);
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
            nfLatDeg_->setValue(static_cast<int>(pos.first));
            tmpval = (pos.first - static_cast<int>(pos.first)) * 60.0;
            nfLatMin_->setValue(static_cast<int>(tmpval));
            nfLatSec_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            nfLonDeg_->setValue(static_cast<int>(pos.second));
            tmpval = (pos.second - static_cast<int>(pos.second)) * 60.0;
            nfLonMin_->setValue(static_cast<int>(tmpval));
            nfLonSec_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            break;
        case WGS84_MIN:
            nfLatDeg_->setValue(static_cast<int>(pos.first));
            nfLatMin_->setValue((pos.first - static_cast<int>(pos.first)) * 60.0);
            nfLonDeg_->setValue(static_cast<int>(pos.second));
            nfLonMin_->setValue((pos.second - static_cast<int>(pos.second)) * 60.0);
            break;
        case WGS84_DEC:
            nfLatDeg_->setValue(pos.first);
            nfLonDeg_->setValue(pos.second);
            break;
        case WGS84_UTM:
            throw std::logic_error("not implemented yet");
            break;
        case SWISSGRID:
            throw std::logic_error("not implemented yet");
            break;
        default:
            assert(!"unknown position format");
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<double, double> WGeoPosEdit::pos() const
{
    const double NorthSouth = (cbNoSo_->currentIndex() ? -1.0 : 1.0);
    const double EastWest   = (cbEaWe_->currentIndex() ? -1.0 : 1.0);

    switch(format_)
    {
        case WGS84_SEC:
            return make_pair(NorthSouth * ((nfLatSec_->value() / 60.0 + nfLatMin_->value()) / 60.0 + nfLatDeg_->value()),
                             EastWest   * ((nfLonSec_->value() / 60.0 + nfLonMin_->value()) / 60.0 + nfLonDeg_->value()));
        case WGS84_MIN:
            return make_pair(NorthSouth * (nfLatMin_->value() / 60.0 + nfLatDeg_->value()),
                             EastWest   * (nfLonMin_->value() / 60.0 + nfLonDeg_->value()));
        case WGS84_DEC:
            return make_pair(NorthSouth * nfLatDeg_->value(),
                             EastWest   * nfLonDeg_->value());
        case WGS84_UTM:
            throw std::logic_error("not implemented");
        case SWISSGRID:
            throw std::logic_error("not implemented");
        default:
            assert(!"unknown position format");
    }
    return make_pair(0.0, 0.0);
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
            sstr << static_cast<int>(tmpval) << "° ";
            tmpval = (tmpval - static_cast<int>(tmpval)) * 60.0;
            sstr << static_cast<int>(tmpval) << "' ";
            sstr << ((tmpval - static_cast<int>(tmpval)) * 60.0) << "\"   ";
            sstr << (pos.second >= 0.0 ? "O " : "W ");
            tmpval = pos.second;
            sstr << static_cast<int>(tmpval) << "° ";
            tmpval = (tmpval - static_cast<int>(tmpval)) * 60.0;
            sstr << static_cast<int>(tmpval) << "' ";
            sstr << ((tmpval - static_cast<int>(tmpval)) * 60.0) << "\"";
            break;
        case WGS84_MIN:
            sstr << (pos.first >= 0.0 ? "N " : "S ");
            tmpval = fabs(pos.first);
            sstr << static_cast<int>(tmpval) << "° ";
            sstr << ((tmpval - static_cast<int>(tmpval)) * 60.0) << "'   ";
            sstr << (pos.second >= 0.0 ? "O " : "W ");
            tmpval = pos.second;
            sstr << static_cast<int>(tmpval) << "° ";
            sstr << ((tmpval - static_cast<int>(tmpval)) * 60.0) << "' ";
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
