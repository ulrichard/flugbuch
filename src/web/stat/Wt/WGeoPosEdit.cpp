// witty
#include <Wt/WCompositeWidget>
#include <Wt/WContainerWidget>
#include <Wt/WGeoPosEdit>
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/NumberField>
#include <Wt/Ext/LineEdit>
#include <Wt/WHBoxLayout>
// boost
// standard library
#include <cmath>

using std::string;
using std::pair;
using std::make_pair;

using namespace Wt;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
WGeoPosEdit::WGeoPosEdit(Wt::WGeoPosEdit::PositionFormat fmt, Wt::WContainerWidget *parent)
  : Wt::WCompositeWidget(parent), format_(fmt), impl_(new Wt::WContainerWidget())
{
    cbNoSo_ = cbEaWe_ = NULL;
    nfLatDeg_ = nfLatMin_ = nfLatSec_ = nfLonDeg_ = nfLonMin_ = nfLonSec_ = nfGridX_ = nfGridY_ = NULL;
    leZone_ = NULL;

    switch(format_)
    {
        case WGS84_SEC:
            cbNoSo_   = new Wt::Ext::ComboBox(impl_);
            cbNoSo_->addItem("N");
            cbNoSo_->addItem("S");
            nfLatDeg_ = new Wt::Ext::NumberField(impl_);
            nfLatMin_ = new Wt::Ext::NumberField(impl_);
            nfLatSec_ = new Wt::Ext::NumberField(impl_);
            cbEaWe_   = new Wt::Ext::ComboBox(impl_);
            cbEaWe_->addItem("E");
            cbEaWe_->addItem("W");
            nfLonDeg_ = new Wt::Ext::NumberField(impl_);
            nfLonMin_ = new Wt::Ext::NumberField(impl_);
            nfLonSec_ = new Wt::Ext::NumberField(impl_);
            break;
        case WGS84_MIN:
            cbNoSo_   = new Wt::Ext::ComboBox(impl_);
            cbNoSo_->addItem("N");
            cbNoSo_->addItem("S");
            nfLatDeg_ = new Wt::Ext::NumberField(impl_);
            nfLatMin_ = new Wt::Ext::NumberField(impl_);
            cbEaWe_   = new Wt::Ext::ComboBox(impl_);
            cbEaWe_->addItem("E");
            cbEaWe_->addItem("W");
            nfLonDeg_ = new Wt::Ext::NumberField(impl_);
            nfLonMin_ = new Wt::Ext::NumberField(impl_);
            break;
        case WGS84_DEC:
            cbNoSo_   = new Wt::Ext::ComboBox(impl_);
            cbNoSo_->addItem("N");
            cbNoSo_->addItem("S");
            nfLatDeg_ = new Wt::Ext::NumberField(impl_);
            cbEaWe_   = new Wt::Ext::ComboBox(impl_);
            cbEaWe_->addItem("E");
            cbEaWe_->addItem("W");
            nfLonDeg_ = new Wt::Ext::NumberField(impl_);
            break;
        case WGS84_UTM:
            leZone_   = new Wt::Ext::LineEdit(impl_);
            // no break
        case SWISSGRID:
            nfGridX_  = new Wt::Ext::NumberField(impl_);
            nfGridY_  = new Wt::Ext::NumberField(impl_);
            break;
        default:
            assert(!"unknown position format");
    }

    setImplementation(impl_);
    impl_->setLayout(new Wt::WHBoxLayout());
    if(cbNoSo_)
    {
        cbNoSo_->resize(20, cbNoSo_->height());
        impl_->layout()->addWidget(cbNoSo_);
    }
    if(nfLatDeg_)
    {
        nfLatDeg_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLatDeg_);
    }
    if(nfLatMin_)
    {
        nfLatMin_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLatMin_);
    }
    if(nfLatSec_)
    {
        nfLatSec_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLatSec_);
    }
    if(cbEaWe_)
    {
        cbEaWe_->resize(20, cbNoSo_->height());
        impl_->layout()->addWidget(cbEaWe_);
    }
    if(nfLonDeg_)
    {
        nfLonDeg_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLonDeg_);
    }
    if(nfLonMin_)
    {
        nfLonMin_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLonMin_);
    }
    if(nfLonSec_)
    {
        nfLonSec_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(nfLonSec_);
    }
    if(leZone_)
    {
        leZone_->resize(40, cbNoSo_->height());
        impl_->layout()->addWidget(leZone_);
    }
    if(nfGridX_)
    {
        nfGridX_->resize(80, cbNoSo_->height());
        impl_->layout()->addWidget(nfGridX_);
    }
    if(nfGridY_)
    {
        nfGridY_->resize(80, cbNoSo_->height());
        impl_->layout()->addWidget(nfGridY_);
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WGeoPosEdit::setPos(std::pair<double, double> pos)
{
    double tmpval;
    switch(format_)
    {
        case WGS84_SEC:
            cbNoSo_->setCurrentIndex(pos.first >= 0.0 ? 0 : 1);
            tmpval = fabs(pos.first);
            nfLatDeg_->setValue(static_cast<int>(tmpval));
            tmpval = (tmpval - static_cast<int>(tmpval)) * 60.0;
            nfLatMin_->setValue(static_cast<int>(tmpval));
            nfLatSec_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            cbEaWe_->setCurrentIndex(pos.second >= 0.0 ? 0 : 1);
            tmpval = fabs(pos.second);
            nfLonDeg_->setValue(static_cast<int>(tmpval));
            tmpval = (tmpval - static_cast<int>(tmpval)) * 60.0;
            nfLonMin_->setValue(static_cast<int>(tmpval));
            nfLonSec_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            break;
        case WGS84_MIN:
            cbNoSo_->setCurrentIndex(pos.first >= 0.0 ? 0 : 1);
            tmpval = fabs(pos.first);
            nfLatDeg_->setValue(static_cast<int>(tmpval));
            nfLatMin_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            cbEaWe_->setCurrentIndex(pos.second >= 0.0 ? 0 : 1);
            tmpval = fabs(pos.second);
            nfLonDeg_->setValue(static_cast<int>(tmpval));
            nfLonMin_->setValue((tmpval - static_cast<int>(tmpval)) * 60.0);
            break;
        case WGS84_DEC:
            cbNoSo_->setCurrentIndex(pos.first >= 0.0 ? 0 : 1);
            nfLatDeg_->setValue(pos.first);
            cbEaWe_->setCurrentIndex(pos.second >= 0.0 ? 0 : 1);
            nfLonDeg_->setValue(pos.second);
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
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<double, double> WGeoPosEdit::pos() const
{
    pair<double, double> val = make_pair(0.0, 0.0);
    const double NorthSouth = (cbNoSo_->currentIndex() ? -1.0 : 1.0);
    const double EastWest   = (cbEaWe_->currentIndex() ? -1.0 : 1.0);

    switch(format_)
    {
        case WGS84_SEC:
            val = make_pair(NorthSouth * ((nfLatSec_->value() / 60.0 + nfLatMin_->value()) / 60.0 + nfLatDeg_->value()),
                            EastWest   * ((nfLonSec_->value() / 60.0 + nfLonMin_->value()) / 60.0 + nfLonDeg_->value()));
            break;
        case WGS84_MIN:
            val = make_pair(NorthSouth * (nfLatMin_->value() / 60.0 + nfLatDeg_->value()),
                            EastWest   * (nfLonMin_->value() / 60.0 + nfLonDeg_->value()));
            break;
        case WGS84_DEC:
            val = make_pair(NorthSouth * nfLatDeg_->value(),
                            EastWest   * nfLonDeg_->value());
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
    return val;
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
