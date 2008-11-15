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

    Wt::WText               *txt_;
    Wt::WLineEdit           *username_;
    Wt::WLineEdit           *password_;
    Wt::WLineEdit           *password2_;
    Wt::WCheckBox           *useStdLoc_;
    Wt::WFileUpload         *uploader_;
    Wt::Ext::ProgressDialog *progress_;

};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
} // namespace flbwt
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#endif // WELCOMESCREEN_H
