// flugbuch
#include <WelcomeScreen.h>
#include <main.h>
#include <FormatStr.h>
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

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
WelcomeScreen::WelcomeScreen(Wt::WContainerWidget *parent)
 : Wt::WTabWidget(parent)
{
    nl_txt_ = rg_txt_ = td_txt_ = im_txt_ = NULL;
    nl_username_  = nl_password_ = rg_username_  = rg_password_ = rg_password2_ = NULL;
    im_uploader_  = NULL;
    rg_useStdLoc_ = NULL;
    im_progress_  = NULL;

 //   setContentAlignment(AlignMiddle | AlignCenter);
    resize(400, 300);

    addTab(makeNormalLoginTab(), "Login");
    addTab(makeRegistrationTab(), "Registrieren");
    addTab(makeTestDbTab(), "Testen");
    addTab(makeImportTab(), "Import");

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Wt::WWidget * WelcomeScreen::makeNormalLoginTab()
{
    Wt::WContainerWidget *cont = new Wt::WContainerWidget();
    nl_txt_ = new Wt::WText("Hier koennen Sie ihr bestehendes Flugbuch aufrufen.", cont);
    Wt::WBreak *brk1 = new Wt::WBreak(cont);
    Wt::WBreak *brk2 = new Wt::WBreak(cont);

    Wt::WTable *layout = new Wt::WTable(cont);
    Wt::WLabel *usernameLabel = new Wt::WLabel("Benutzername: ", layout->elementAt(0, 0));
    layout->elementAt(0, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
    nl_username_ = new Wt::WLineEdit(layout->elementAt(0, 1));
    usernameLabel->setBuddy(nl_username_);

    Wt::WLabel *passwordLabel = new Wt::WLabel("Password: ", layout->elementAt(1, 0));
    nl_password_ = new Wt::WLineEdit(layout->elementAt(1, 1));
    nl_password_->setEchoMode(Wt::WLineEdit::Password);
    passwordLabel->setBuddy(nl_password_);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Login", layout->elementAt(2, 1));
    LoginButton->clicked.connect(SLOT( this, WelcomeScreen::doLogin));

    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Wt::WWidget * WelcomeScreen::makeRegistrationTab()
{
    Wt::WContainerWidget *cont = new Wt::WContainerWidget();
    rg_txt_ = new Wt::WText("Hier koennen Sie ihr neues Flugbuch eroeffnen.", cont);
    Wt::WBreak *brk1 = new Wt::WBreak(cont);
    Wt::WBreak *brk2 = new Wt::WBreak(cont);

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
    passwordLabel2->setBuddy(rg_password2_);

    Wt::WLabel *stdflareas = new Wt::WLabel("Standardfluggebiete importieren: ", layout->elementAt(3, 0));
    rg_useStdLoc_ = new Wt::WCheckBox(layout->elementAt(3, 1));
    stdflareas->setBuddy(rg_useStdLoc_);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Registrieren", layout->elementAt(4, 1));
    LoginButton->clicked.connect(SLOT( this, WelcomeScreen::doRegister));

    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::doRegister()
{
    FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());

    if(rg_password2_->text() != rg_password_->text())
        rg_txt_->setText("Die Passwoerter stimmen nicht ueberein");
    else
        app->createFlightDb(rg_username_->text().narrow(), rg_password_->text().narrow(), rg_useStdLoc_->isChecked());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Wt::WWidget * WelcomeScreen::makeTestDbTab()
{
    Wt::WContainerWidget *cont = new Wt::WContainerWidget();
    td_txt_ = new Wt::WText("Hier koennen sie mit einer kleinen Test-Datenbank das Programm ausprobieren.", cont);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Testdatenbank laden", cont);
    LoginButton->clicked.connect(SLOT( this, WelcomeScreen::doTest));

    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::doTest()
{
   FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());

    app->loadTestDb();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Wt::WWidget * WelcomeScreen::makeImportTab()
{
    Wt::WContainerWidget *cont = new Wt::WContainerWidget();
    im_txt_ = new Wt::WText("Hier koennen Sie ihre mdb Datenbank vom alten Flugbuchprogramm importieren.", cont);

    Wt::WBreak *brk1 = new Wt::WBreak(cont);

    im_uploader_ = new Wt::WFileUpload(cont);
    im_uploader_->uploaded.connect(SLOT(this, WelcomeScreen::doImport));
    im_uploader_->fileTooLarge.connect(SLOT(this, WelcomeScreen::uploadTooBig));

    Wt::WBreak *brk2 = new Wt::WBreak(cont);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Importieren", cont);
    LoginButton->clicked.connect(SLOT(this, WelcomeScreen::startUpload));


    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::startUpload()
{
    im_progress_ = new Wt::Ext::ProgressDialog("Die Datei wird hochgeladen...", "Abbrechen", 0, 100);
    im_progress_->setWindowTitle("Upload");

    im_uploader_->upload();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::uploadTooBig(int size)
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
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::doImport()
{
    assert(im_uploader_);
    assert(im_progress_);

    im_progress_->cancel();
    delete im_progress_;
    im_progress_ = NULL;

    im_uploader_->stealSpooledFile();
    FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());
    app->importFlightDb(im_uploader_->spoolFileName(), true);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
