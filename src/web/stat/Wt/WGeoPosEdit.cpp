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
#include <boost/lexical_cast.hpp>
// standard library
#include <cmath>
#include <iostream>

using boost::lexical_cast;
using boost::get;
using std::string;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

using namespace Wt;
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
WGeoPosEdit::WGeoPosEdit(WContainerWidget *parent, WGeoPosEdit::PositionFormat fmt)
  : Wt::WCompositeWidget(parent), format_(fmt), impl_(new Wt::WContainerWidget()), changed_(this)
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
            {
                boost::tuple<int, char, double, double> utm = LatLonToUtm(pos);
                leZone_->setText(get<1>(utm) + lexical_cast<string>(get<0>(utm)));
                nfGridX_->setValue(get<2>(utm));
                nfGridY_->setValue(get<3>(utm));
            }
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
            ppair = make_pair(NorthSouth * (nfLatMin_->value() / 60.0 + nfLatDeg_->value()),
                              EastWest   * (nfLonMin_->value() / 60.0 + nfLonDeg_->value()));
            break;
        case WGS84_DEC:
            ppair = make_pair(NorthSouth * nfLatDeg_->value(),
                              EastWest   * nfLonDeg_->value());
            break;
        case WGS84_UTM:
        {
            string zone = leZone_->text().narrow();
            ppair = UtmToLatLon(lexical_cast<int>(zone.substr(1, zone.length())), zone[0], nfGridX_->value(), nfGridY_->value());
            break;
        }
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
            tmpval = fabs(pos.second);
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
            tmpval = fabs(pos.second);
            sstr << floor(tmpval) << "° ";
            sstr << ((tmpval - floor(tmpval)) * 60.0) << "' ";
            break;
        case WGS84_DEC:
            sstr << (pos.first >= 0.0 ? "N " : "S ");
            sstr << fabs(pos.first) << "° ";
            sstr << (pos.second >= 0.0 ? "O " : "W ");
            sstr << fabs(pos.second) << "° ";
            break;
        case WGS84_UTM:
        {
            boost::tuple<int, char, double, double> utm = LatLonToUtm(pos);
            sstr << get<1>(utm) << get<0>(utm) << " " << get<2>(utm) << " " << get<3>(utm);
            break;
        }
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
    mapDlg_->resize(508, 445);
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
// Purpose:
//  This function converts the specified lat/lon coordinate to a UTM
//  coordinate.
// tuple return value:
//  int utmXZone:
//      this field will contain the hotizontal zone number of
//      the UTM coordinate.  The returned value for this parameter is a number
//      within the range 1 to 60, inclusive.
//  char utmYZone:
//      this field will contain the zone letter of the UTM
//      coordinate.  The returned value for this parameter will be one of:
//      CDEFGHJKLMNPQRSTUVWX.
//  double easting:
//      this field will contain the UTM easting, in meters.
//  double northing:
//      this field will contain the UTM northing, in meters.
// Notes:
//  - The code in this function is a C conversion of some of the source code
//    from the Mapping Datum Transformation Software (MADTRAN) program,
//    written in PowerBasic.  To get the source code for MADTRAN, go to:
//
//      http://164.214.2.59/publications/guides/MADTRAN/index.html
//
//    and download MADTRAN.ZIP
//  - If the UTM zone is out of range, the y-zone character is set to the
//    asterisk character ('*').
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
boost::tuple<int, char, double, double> WGeoPosEdit::LatLonToUtm(pair<double, double> latlon)
{
    // Some constants used by these functions.
    static const double fe = 500000.0;
    static const double ok = 0.9996;
    static const double PI(3.14159265);
    static const double deg2rad(PI / 180.0);
    static const double rad2deg(180.0 / PI);
    // An array containing each vertical UTM zone.
    static char cArray[] = "CDEFGHJKLMNPQRSTUVWX";
    // parameters for wgs84
    static const double a = 6378137.0;          // Ellipsoid semi-major axis, in meters.
    static const double f = 1 / 298.257223563;  // Ellipsoid flattening.

    int utmXZone = latlon.second <= 0.0 ? 30 + static_cast<int>(latlon.second / 6.0) : 31 + static_cast<int>(latlon.second / 6.0);

    // Special case: zone X is 12 degrees from north to south, not 8.
    char utmYZone = cArray[(latlon.first < 84.0 && latlon.first >= 72.0) ? 19 : static_cast<int>((latlon.first + 80.0) / 8.0)];
	if(latlon.first >= 84.0 || latlon.first < -80.0)
		utmYZone = '*'; // Invalid coordinate; the vertical zone is set to the invalid character

	double latRad = latlon.first  * deg2rad;
	double lonRad = latlon.second * deg2rad;
	double recf = 1.0 / f;
	double b = a * (recf - 1.0) / recf;
	double eSquared  = ((a * a) - (b * b)) / (a * a);
	double e2Squared = ((a * a) - (b * b)) / (b * b);
	double tn = (a - b) / (a + b);
	double ap = a * (1.0 - tn + 5.0 * ((tn * tn) - (tn * tn * tn)) / 4.0 + 81.0 *
		((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 64.0);
	double bp = 3.0 * a * (tn - (tn * tn) + 7.0 * ((tn * tn * tn)
		- (tn * tn * tn * tn)) / 8.0 + 55.0 * (tn * tn * tn * tn * tn) / 64.0)
		/ 2.0;
	double cp = 15.0 * a * ((tn * tn) - (tn * tn * tn) + 3.0 * ((tn * tn * tn * tn)
		- (tn * tn * tn * tn * tn)) / 4.0) / 16.0;
	double dp = 35.0 * a * ((tn * tn * tn) - (tn * tn * tn * tn) + 11.0
		* (tn * tn * tn * tn * tn) / 16.0) / 48.0;
	double ep = 315.0 * a * ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 512.0;
	double olam = (utmXZone * 6 - 183) * deg2rad;
	double dlam = lonRad - olam;
	double s = sin (latRad);
	double c = cos (latRad);
	double t = s / c;
	double eta = e2Squared * (c * c);
	double sn = a / sqrt (1.0 - eSquared * std::pow(sin(latRad), 2));
	double tmd = (ap * latRad) - (bp * sin(2.0 * latRad)) + (cp * sin(4.0 * latRad)) - (dp * sin(6.0 * latRad)) + (ep * sin(8.0 * latRad));
	double t1 = tmd * ok;
	double t2 = sn * s * c * ok / 2.0;
	double t3 = sn * s * (c * c * c) * ok * (5.0 - (t * t) + 9.0 * eta + 4.0
		* (eta * eta)) / 24.0;
    double nfn = (latRad < 0.0) ? 10000000.0 : 0.0;
	double northing = nfn + t1 + (dlam * dlam) * t2 + (dlam * dlam * dlam
		* dlam) * t3 + (dlam * dlam * dlam * dlam * dlam * dlam) + 0.5;
	double t6 = sn * c * ok;
	double t7 = sn * (c * c * c) * (1.0 - (t * t) + eta) / 6.0;
	double easting = fe + dlam * t6 + (dlam * dlam * dlam) * t7 + 0.5;
	if(northing >= 9999999.0)
        northing = 9999999.0;

    return boost::make_tuple(utmXZone, utmYZone, easting, northing);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
// Purpose:
//  This function converts the specified UTM coordinate to a lat/lon
//  coordinate.
// Pre:
//  - utmXZone must be between 1 and 60, inclusive.
//  - utmYZone must be one of: CDEFGHJKLMNPQRSTUVWX
// Parameters:
//  int utmXZone:
//      The horizontal zone number of the UTM coordinate.
//  char utmYZone:
//      The vertical zone letter of the UTM coordinate.
//  double easting, double northing:
//      The UTM coordinate to convert.
// Notes:
//  The code in this function is a C conversion of some of the source code
//  from the Mapping Datum Transformation Software (MADTRAN) program, written
//  in PowerBasic.  To get the source code for MADTRAN, go to:
//
//    http://164.214.2.59/publications/guides/MADTRAN/index.html
//
//  and download MADTRAN.ZIP
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
pair<double, double> WGeoPosEdit::UtmToLatLon(int utmXZone, char utmYZone, double easting, double northing)
{
    // Some constants used by these functions.
    static const double fe = 500000.0;
    static const double ok = 0.9996;
    static const double PI(3.14159265);
    static const double deg2rad(PI / 180.0);
    static const double rad2deg(180.0 / PI);
    // An array containing each vertical UTM zone.
    static char cArray[] = "CDEFGHJKLMNPQRSTUVWX";
    // parameters for wgs84
    static const double a = 6378137.0;          // Ellipsoid semi-major axis, in meters.
    static const double f = 1 / 298.257223563;  // Ellipsoid flattening.


	double nfn;
	double ftphi;
	double recf = 1.0 / f;
	double b = a * (recf - 1) / recf;
	double eSquared = ((a * a) - (b * b)) / (a * a);
	double e2Squared = ((a * a) - (b * b)) / (b * b);
	double tn = (a - b) / (a + b);
	double ap = a * (1.0 - tn + 5.0 * ((tn * tn) - (tn * tn * tn)) / 4.0 + 81.0 *
		((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 64.0);
	double bp = 3.0 * a * (tn - (tn * tn) + 7.0 * ((tn * tn * tn)
		- (tn * tn * tn * tn)) / 8.0 + 55.0 * (tn * tn * tn * tn * tn) / 64.0)
		/ 2.0;
	double cp = 15.0 * a * ((tn * tn) - (tn * tn * tn) + 3.0 * ((tn * tn * tn * tn)
		- (tn * tn * tn * tn * tn)) / 4.0) / 16.0;
	double dp = 35.0 * a * ((tn * tn * tn) - (tn * tn * tn * tn) + 11.0
		* (tn * tn * tn * tn * tn) / 16.0) / 48.0;
	double ep = 315.0 * a * ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 512.0;
	if ((utmYZone <= 'M' && utmYZone >= 'C') || (utmYZone <= 'm' && utmYZone >= 'c'))
		nfn = 10000000.0;
	else
		nfn = 0;
	double tmd = (northing - nfn) / ok;
	double sr = a * (1.0 - eSquared);
	ftphi = tmd / sr;
	double t10, t11, t14, t15;
	for(int i = 0; i < 5; i++)
	{
		t10 = (ap * ftphi) - (bp * sin (2.0 * ftphi)) + (cp * sin (4.0 * ftphi)) - (dp * sin (6.0 * ftphi)) + (ep * sin (8.0 * ftphi));
		sr = a * (1.0 - eSquared) / std::pow(sqrt(1.0 - eSquared * std::pow(sin(ftphi), 2)), 3);
		ftphi = ftphi + (tmd - t10) / sr;
	}
	sr = a * (1.0 - eSquared) / std::pow(sqrt(1.0 - eSquared * std::pow(sin(ftphi), 2)), 3);
	double sn = a / sqrt(1.0 - eSquared * std::pow(sin(ftphi), 2));
	double s = sin(ftphi);
	double c = cos(ftphi);
	double t = s / c;
	double eta = e2Squared * (c * c);
	double de = easting - fe;
	t10 = t / (2.0 * sr * sn * (ok * ok));
	t11 = t * (5.0 + 3.0 * (t * t) + eta - 4.0 * (eta * eta) - 9.0 * (t * t)
		* eta) / (24.0 * sr * (sn * sn * sn) * (ok * ok * ok * ok));
	double lat = ftphi - (de * de) * t10 + (de * de * de * de) * t11;
	t14 = 1.0 / (sn * c * ok);
	t15 = (1.0 + 2.0 * (t * t) + eta) / (6 * (sn * sn * sn) * c
		* (ok * ok * ok));
	double dlam = de * t14 - (de * de * de) * t15;
	double olam = (utmXZone * 6 - 183.0) * deg2rad;
	double lon = olam + dlam;
	lon *= rad2deg;
	lat *= rad2deg;

	return make_pair(lat, lon);
}
