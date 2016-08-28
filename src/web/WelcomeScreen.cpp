// flugbuch
#include "WelcomeScreen.h"
#include "main.h"
#include "FormatStr.h"
// witty
#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WFileUpload>
#include <Wt/WCheckBox>
#include <Wt/WBreak>
#include <Wt/Ext/ProgressDialog>
#include <Wt/Ext/MessageBox>
#include <Wt/WApplication>

using namespace flbwt;
using std::string;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
WelcomeScreen::WelcomeScreen(Wt::WContainerWidget *parent)
 : Wt::WTabWidget(parent)
{
    nl_txt_ = rg_txt_ = td_txt_ = im_txt_ = NULL;
    nl_username_  = nl_password_ = rg_username_  = rg_password_ = rg_password2_ = im_username_ = im_password_ = NULL;
    im_uploader_  = NULL;
    rg_useStdLoc_ = NULL;
    im_progress_  = NULL;

 //   setContentAlignment(AlignMiddle | AlignCenter);
    resize(400, 300);
    setStyleClass("WelcomeScreen");

    addTab(makeNormalLoginTab(),  "Login");
    addTab(makeRegistrationTab(), "Registrieren");
    addTab(makeTestDbTab(), "Testen");

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
Wt::WWidget * WelcomeScreen::makeNormalLoginTab()
{
    Wt::WContainerWidget *cont = new Wt::WContainerWidget();
    nl_txt_ = new Wt::WText("Hier koennen Sie ihr bestehendes Flugbuch aufrufen.", cont);
    cont->addWidget(new Wt::WBreak());
    cont->addWidget(new Wt::WBreak());

    Wt::WTable *layout = new Wt::WTable(cont);
    Wt::WLabel *usernameLabel = new Wt::WLabel("Benutzername: ", layout->elementAt(0, 0));
    layout->elementAt(0, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
    nl_username_ = new Wt::WLineEdit(layout->elementAt(0, 1));
    usernameLabel->setBuddy(nl_username_);

    Wt::WLabel *passwordLabel = new Wt::WLabel("Password: ", layout->elementAt(1, 0));
    nl_password_ = new Wt::WLineEdit(layout->elementAt(1, 1));
    nl_password_->setEchoMode(Wt::WLineEdit::Password);
    nl_password_->enterPressed().connect(SLOT(this, WelcomeScreen::doLogin));
    passwordLabel->setBuddy(nl_password_);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Login", layout->elementAt(2, 1));
    LoginButton->clicked().connect(SLOT( this, WelcomeScreen::doLogin));

    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WelcomeScreen::doLogin()
{
    FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());
    assert(app);
    assert(nl_username_);
    assert(nl_password_);

    try
    {
        const string usr = nl_username_->text().narrow();
        const string pwd = nl_password_->text().narrow();
        app->loadFlightDb(usr, pwd);
    }
    catch(std::exception &ex)
    {
        nl_txt_->setText(ex.what());
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
Wt::WWidget * WelcomeScreen::makeRegistrationTab()
{
    Wt::WContainerWidget *cont = new Wt::WContainerWidget();
    rg_txt_ = new Wt::WText("Hier koennen Sie ihr neues Flugbuch eroeffnen.", cont);
    cont->addWidget(new Wt::WBreak());
    cont->addWidget(new Wt::WBreak());


    Wt::WTable *layout = new Wt::WTable(cont);

    Wt::WLabel *usernameLabel = new Wt::WLabel("Benutzername: ", layout->elementAt(0, 0));
    layout->elementAt(0, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
    rg_username_ = new Wt::WLineEdit(layout->elementAt(0, 1));
    usernameLabel->setBuddy(rg_username_);

    Wt::WLabel *passwordLabel = new Wt::WLabel("Passwort: ", layout->elementAt(1, 0));
    rg_password_ = new Wt::WLineEdit(layout->elementAt(1, 1));
    rg_password_->setEchoMode(Wt::WLineEdit::Password);
    passwordLabel->setBuddy(rg_password_);

    Wt::WLabel *passwordLabel2 = new Wt::WLabel("wiederholen: ", layout->elementAt(2, 0));
    rg_password2_ = new Wt::WLineEdit(layout->elementAt(2, 1));
    rg_password2_->setEchoMode(Wt::WLineEdit::Password);
    rg_password2_->enterPressed().connect(SLOT(this, WelcomeScreen::doRegister));
    passwordLabel2->setBuddy(rg_password2_);

    Wt::WLabel *stdflareas = new Wt::WLabel("Standardfluggebiete importieren: ", layout->elementAt(3, 0));
    rg_useStdLoc_ = new Wt::WCheckBox(layout->elementAt(3, 1));
    stdflareas->setBuddy(rg_useStdLoc_);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Registrieren", layout->elementAt(4, 1));
    LoginButton->clicked().connect(SLOT( this, WelcomeScreen::doRegister));

    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WelcomeScreen::doRegister()
{
    FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());

    if(rg_password2_->text() != rg_password_->text())
        rg_txt_->setText("Die Passwoerter stimmen nicht ueberein");
    else
        app->createFlightDb(rg_username_->text().narrow(), rg_password_->text().narrow(), rg_useStdLoc_->isChecked());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
Wt::WWidget * WelcomeScreen::makeTestDbTab()
{
    Wt::WContainerWidget *cont = new Wt::WContainerWidget();
    td_txt_ = new Wt::WText("Hier koennen sie mit einer kleinen Test-Datenbank das Programm ausprobieren.", cont);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Testdatenbank laden", cont);
    LoginButton->clicked().connect(SLOT( this, WelcomeScreen::doTest));

    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WelcomeScreen::doTest()
{
   FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());

    app->loadTestDb();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WelcomeScreen::startUpload()
{
    im_progress_ = new Wt::Ext::ProgressDialog("Die Datei wird hochgeladen...", "Abbrechen", 0, 100);
    im_progress_->setWindowTitle("Upload");

    im_uploader_->upload();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void WelcomeScreen::uploadTooBig(::int64_t size)
{
    im_progress_->cancel();
    delete im_progress_;
    im_progress_ = NULL;

    int maxreqsize = Wt::WApplication::instance()->maximumRequestSize();
    std::string msg(FormatStr() << "Die Datei ist zu gross fuer den upload :\n"
                                << "Upload  : " << size / 1024 << " kB\n"
                                << "Maximum : " << maxreqsize / 1024 << " kB");
    im_txt_->setText(msg);
//    Wt::Ext::MessageBox::show("Error", msg, Wt::Warning, true);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
