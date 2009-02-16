// Flugbuch
#include "MainMenu.h"
#include "main.h"
#include "FormatStr.h"
// witty
#include <Wt/Ext/Menu>
#include <Wt/Ext/Button>
#include <Wt/Ext/MessageBox>
#include <Wt/WEnvironment>
// boost
#include <boost/version.hpp>


using namespace flbwt;
using std::string;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
MainMenu::MainMenu(Wt::WContainerWidget *parent)
 : Wt::Ext::ToolBar(parent)
{
    FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());

    // Create the file menu
    Wt::Ext::Menu *menu = new Wt::Ext::Menu();
    Wt::Ext::Button *btn = addButton("Flugbuch", menu);
//    b->setIcon("icons/yellow-folder-closed.png");

    Wt::Ext::MenuItem *item = menu->addItem("Flugbuch speichern");
    item->setIcon("img/save.png");
    item->activated.connect(SLOT(app, FlightLogApp::saveDb));

    item = menu->addItem("Änderungen verwerfen");
    item->setIcon("img/undo.png");
    item->activated.connect(SLOT(app, FlightLogApp::reload));

    item = menu->addItem("Logout");
    item->setIcon("img/home.png");
    item->activated.connect(SLOT(app, FlightLogApp::doLogin));


    // create the help menu
    menu = new Wt::Ext::Menu();
    btn = addButton("Hilfe", menu);
//    b->setIcon("icons/yellow-folder-closed.png");

    item = menu->addItem("Hilfe zum Flugbuch");
//    item->setIcon("icons/yellow-folder-open.png");

    item = menu->addItem("Informationen");
    item->activated.connect(SLOT(this, MainMenu::info));


}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void MainMenu::info(void)
{
    const string msg(FormatStr() << "Flugbuch2 ist ein Programm zum verwalten von Flügen mit Gleitschirm, Delta ..."
                     << "<br>Entwickelt von Richard Ulrich (richi@paraeasy.ch)"
                     << "<br>Die hier vorliegende Web - Version ist zur Verwendung im Browser. Daneben soll es in Zukunft auch eine Gui-Version geben."
                     << "<br>Folgende Drittbibliotheken wurden verwendet:"
                     << "<br>www.webtoolkit.eu  " << Wt::WEnvironment::libraryVersion()
                     << "<br>www.boost.org " << BOOST_VERSION / 100000 << "." << (BOOST_VERSION / 100) % 1000 << "." << BOOST_VERSION % 100
                     << "<br>www.extjs.com"
                     << "<br>dtemplatelib.sourceforge.net"
                     << "<br>mdbtools.sourceforge.net");

    Wt::Ext::MessageBox::show("Informationen zum Flugbuch2", msg, Wt::Information, true);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

