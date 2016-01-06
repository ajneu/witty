#include <Wt/WApplication>

namespace Wt {
   class WLineEdit;
   class WText;
   class WContainerWidget;
   class WTemplate;
   class WGroupBox;
   class WPanel;
}

class Application : public Wt::WApplication
{
public:
   Application(const Wt::WEnvironment& env);
   static WApplication *createApplication(const Wt::WEnvironment& env);

private:
   Wt::WLineEdit *nameEdit_;
   Wt::WText *greeting_;

   void greet();
   Wt::WContainerWidget *addContainer() const;
   Wt::WTemplate        *addTemplate()  const;
   Wt::WGroupBox        *addGroupBox()  const;
   Wt::WPanel           *addPanel()     const;
   Wt::WContainerWidget *addLayout()    const;
   Wt::WContainerWidget *addDialog()    const;

   void showModalDialog(Wt::WText *)    const;
};
