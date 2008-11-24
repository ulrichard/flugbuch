#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

//witty
#include <Wt/WTabWidget>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// forward declarations
namespace Wt
{
    class WLineEdit;
    class WFileUpload;
    class WText;
    class WCheckBox;

    namespace Ext
    {
        class ProgressDialog;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
namespace flbwt
{
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class WelcomeScreen : public Wt::WTabWidget
{
public:
    WelcomeScreen(Wt::WContainerWidget *parent = 0);

private:
    Wt::WWidget * makeNormalLoginTab();
    Wt::WWidget * makeRegistrationTab();
    Wt::WWidget * makeTestDbTab();
    Wt::WWidget * makeImportTab();
    void doLogin();
    void doRegister();
    void doTest();
    void startUpload();
    void doImport();
    void uploadTooBig(int size);

    // normal login
    Wt::WText               *nl_txt_;
    Wt::WLineEdit           *nl_username_;
    Wt::WLineEdit           *nl_password_;
    // register
    Wt::WText               *rg_txt_;
    Wt::WLineEdit           *rg_username_;
    Wt::WLineEdit           *rg_password_;
    Wt::WLineEdit           *rg_password2_;
    Wt::WCheckBox           *rg_useStdLoc_;
    // testdb
    Wt::WText               *td_txt_;
    // import mdb
    Wt::WText               *im_txt_;
    Wt::WFileUpload         *im_uploader_;
    Wt::Ext::ProgressDialog *im_progress_;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
} // namespace flbwt
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#endif // WELCOMESCREEN_H