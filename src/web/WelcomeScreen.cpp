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

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
WelcomeScreen::WelcomeScreen(Wt::WContainerWidget *parent)
 : Wt::WTabWidget(parent)
{
    txt_ = NULL;
    username_  = password_ = password2_ = NULL;
    uploader_  = NULL;
    useStdLoc_ = NULL;
    progress_  = NULL;

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
    txt_ = new Wt::WText("Hier koennen Sie ihr bestehendes Flugbuch aufrufen.", cont);
    Wt::WBreak *brk1 = new Wt::WBreak(cont);
    Wt::WBreak *brk2 = new Wt::WBreak(cont);

    Wt::WTable *layout = new Wt::WTable(cont);
    Wt::WLabel *usernameLabel = new Wt::WLabel("Benutzername: ", layout->elementAt(0, 0));
    layout->elementAt(0, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
    username_ = new Wt::WLineEdit(layout->elementAt(0, 1));
    usernameLabel->setBuddy(username_);

    Wt::WLabel *passwordLabel = new Wt::WLabel("Password: ", layout->elementAt(1, 0));
    password_ = new Wt::WLineEdit(layout->elementAt(1, 1));
    password_->setEchoMode(Wt::WLineEdit::Password);
    passwordLabel->setBuddy(password_);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Login", layout->elementAt(2, 1));
    LoginButton->clicked.connect(SLOT( this, WelcomeScreen::doLogin));

    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::doLogin()
{
    FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());

    app->loadFlightDb(username_->text().narrow(), password_->text().narrow());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Wt::WWidget * WelcomeScreen::makeRegistrationTab()
{
    Wt::WContainerWidget *cont = new Wt::WContainerWidget();
    txt_ = new Wt::WText("Hier koennen Sie ihr neues Flugbuch eroeffnen.", cont);
    Wt::WBreak *brk1 = new Wt::WBreak(cont);
    Wt::WBreak *brk2 = new Wt::WBreak(cont);

    Wt::WTable *layout = new Wt::WTable(cont);
    Wt::WLabel *usernameLabel = new Wt::WLabel("Benutzername: ", layout->elementAt(0, 0));
    layout->elementAt(0, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
    username_ = new Wt::WLineEdit(layout->elementAt(0, 1));
    usernameLabel->setBuddy(username_);

    Wt::WLabel *passwordLabel = new Wt::WLabel("Passwort: ", layout->elementAt(1, 0));
    password_ = new Wt::WLineEdit(layout->elementAt(1, 1));
    password_->setEchoMode(Wt::WLineEdit::Password);
    passwordLabel->setBuddy(password_);

    Wt::WLabel *passwordLabel2 = new Wt::WLabel("wiederholen: ", layout->elementAt(2, 0));
    password2_ = new Wt::WLineEdit(layout->elementAt(2, 1));
    password2_->setEchoMode(Wt::WLineEdit::Password);
    passwordLabel2->setBuddy(password2_);

    Wt::WLabel *stdflareas = new Wt::WLabel("Standardfluggebiete importieren: ", layout->elementAt(3, 0));
    useStdLoc_ = new Wt::WCheckBox(layout->elementAt(3, 1));
    stdflareas->setBuddy(useStdLoc_);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Registrieren", layout->elementAt(4, 1));
    LoginButton->clicked.connect(SLOT( this, WelcomeScreen::doRegister));

    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::doRegister()
{
    FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());

    if(password2_->text() != password_->text())
        txt_->setText("Die Passwoerter stimmen nicht ueberein");
    else
        app->createFlightDb(username_->text().narrow(), password_->text().narrow(), useStdLoc_->isChecked());
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Wt::WWidget * WelcomeScreen::makeTestDbTab()
{
    Wt::WContainerWidget *cont = new Wt::WContainerWidget();
    Wt::WText *txt = new Wt::WText("Hier koennen sie mit einer kleinen Test-Datenbank das Programm ausprobieren.", cont);

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
    txt_ = new Wt::WText("Hier koennen Sie ihre mdb Datenbank vom alten Flugbuchprogramm importieren.", cont);

    Wt::WBreak *brk1 = new Wt::WBreak(cont);

    uploader_ = new Wt::WFileUpload(cont);
    uploader_->uploaded.connect(SLOT(this, WelcomeScreen::doImport));
    uploader_->fileTooLarge.connect(SLOT(this, WelcomeScreen::uploadTooBig));

    Wt::WBreak *brk2 = new Wt::WBreak(cont);

    Wt::WPushButton *LoginButton = new Wt::WPushButton("Importieren", cont);
    LoginButton->clicked.connect(SLOT(this, WelcomeScreen::startUpload));


    return cont;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::startUpload()
{
    progress_ = new Wt::Ext::ProgressDialog("Die Datei wird hochgeladen...", "Abbrechen", 0, 100);
    progress_->setWindowTitle("Upload");

    uploader_->upload();
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::uploadTooBig(int size)
{
    progress_->cancel();
    delete progress_;
    progress_ = NULL;

    int maxreqsize = Wt::WApplication::instance()->maximumRequestSize();
    std::string msg(FormatStr() << "Die Datei ist zu gross fuer den upload : " << size / 1024 << " kB -> maximum is : "
                                << maxreqsize / 1024 << " kB");
    txt_->setText(msg);
//    Wt::Ext::MessageBox::show("Error", msg, Wt::Warning, true);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void WelcomeScreen::doImport()
{
    assert(uploader_);
    assert(progress_);

    progress_->cancel();
    delete progress_;
    progress_ = NULL;

    uploader_->stealSpooledFile();
    FlightLogApp *app = dynamic_cast<FlightLogApp*>(Wt::WApplication::instance());
    app->importFlightDb(uploader_->spoolFileName(), true);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
