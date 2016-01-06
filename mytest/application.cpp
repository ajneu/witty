#include "application.h"

#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTemplate>
#include <Wt/WBootstrapTheme>
#include <Wt/WGroupBox>
#include <Wt/WPanel>
#include <Wt/WHBoxLayout>
#include <Wt/WDialog>
#include <Wt/WRegExpValidator>
#include <Wt/WLabel>

#include <memory>
#include <functional>

using namespace Wt;

WApplication *Application::createApplication(const WEnvironment& env)
{
   return new Application(env);
}

Application::Application(const WEnvironment& env)
  : WApplication(env)
{
   WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme(this);
   bootstrapTheme->setVersion(Wt::WBootstrapTheme::Version3);
   bootstrapTheme->setResponsive(true);
   setTheme(bootstrapTheme);
   // load the default bootstrap3 (sub-)theme
   useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
   useStyleSheet("style/everywidget.css");
   useStyleSheet("style/dragdrop.css");
   useStyleSheet("style/combostyle.css");
   useStyleSheet("style/pygments.css");
   useStyleSheet("style/layout.css");
   
   messageResourceBundle().use(appRoot() + "text");
   


   
  setTitle("Hello world");                               // application title

  root()->addWidget(new WText("Your name, please ? "));  // show some text
  nameEdit_ = new WLineEdit(root());                     // allow text input
  nameEdit_->setFocus();                                 // give focus
  
  WPushButton *button
    = new WPushButton("Greet me.", root());              // create a button
  button->setMargin(5, Left);                            // add 5 pixels margin

  root()->addWidget(new WBreak());                       // insert a line break

  greeting_ = new WText(root());                         // empty text

  button->clicked().connect(this, &Application::greet);

  nameEdit_->enterPressed().connect
    (std::bind(&Application::greet, this));

  root()->addWidget(new WBreak());

  root()->addWidget(addContainer());
  root()->addWidget(addTemplate());
  root()->addWidget(addGroupBox());
  root()->addWidget(addPanel());
  root()->addWidget(addLayout());
  root()->addWidget(addDialog());
  
  /*
   * - using a c++0x lambda:
   */
  // button->clicked().connect(std::bind([=]() { 
  //       greeting_->setText("Hello there, " + nameEdit_->text());
  // }));
}

void Application::greet()
{
  greeting_->setText("Hello there, " + nameEdit_->text());
}

WContainerWidget *Application::addContainer() const
{
  WContainerWidget *mycontainer = new WContainerWidget;

  WText *tex = new WText("Yip", mycontainer);
  mycontainer->addWidget(new WBreak());
  WText *out = new WText(" ", mycontainer);

  tex->clicked().connect(std::bind([=](){out->setText("clicked");}));
  tex->doubleClicked().connect(std::bind([=](){out->setText("double clicked");}));
  
  for (int i = 0; i < 3; ++i) {
     mycontainer->addWidget(new WText(WString{"<p>x{1}</p>"}.arg(i)));
  }

  return mycontainer;
}

WTemplate *Application::addTemplate() const
{
   WTemplate *templ = new WTemplate(Wt::WString::tr("WTemplate-temperature"));

   WLineEdit   *lineEd       = new WLineEdit();
   WPushButton *saveButton   = new WPushButton("Save");
   WPushButton *cancelButton = new WPushButton("Cancel");
   WText *response = new WText();   
   
   templ->bindWidget("name-edit",     lineEd);
   templ->bindWidget("save-button",   saveButton);
   templ->bindWidget("cancel-button", cancelButton);
   templ->bindWidget("response-text", response);

   auto callable_settext = std::bind([=](){response->setText(lineEd->text());});
   saveButton->clicked().connect(callable_settext);
   //lineEd->enterPressed().connect(callable_settext);
   lineEd->keyWentUp().connect(callable_settext);
   cancelButton->clicked().connect(std::bind([=](){response->setText("");}));
   return templ;
}

WGroupBox *Application::addGroupBox() const
{
   WGroupBox *grp = new WGroupBox("MyGroup");
   grp->addStyleClass("centered-example");
   grp->addWidget(new WText("<p>asdf</p>"));
   grp->addWidget(new WText("<p>qwer</p>"));

   return grp;
}

WPanel *Application::addPanel() const
{
   WPanel *panel = new WPanel;
   panel->setTitle("Click to show");
   panel->setCentralWidget(new WText("hey!"));
   panel->setCollapsible(true);
   panel->setCollapsed(true);
   
   return panel;
}

WContainerWidget *Application::addLayout() const
{
   WContainerWidget *cont = new WContainerWidget();
   cont->setStyleClass("yellow-box");
   WHBoxLayout *hbox      = new WHBoxLayout(cont);

   WText       *text      = new WText("aaa");
   text->setStyleClass("green-box");
   hbox->addWidget(text);
   text->setWidth(40);

   text =                   new WText("bbb");
   text->setStyleClass("blue-box");
   hbox->addWidget(text);

   return cont;
}

WContainerWidget *Application::addDialog() const
{
   WContainerWidget *cont = new WContainerWidget();
   WPushButton *button = new WPushButton("Jump", cont);
   WText       *out    = new WText(cont);
   out->setStyleClass("help-block");
   
   button->clicked().connect(std::bind(&Application::showModalDialog, this, out));
   
   
   return cont;
}

class MyDialog : public WDialog {
public:
   MyDialog(const std::string &heading) : WDialog{heading}
   {
      std::cout << "########## constructor" << std::endl;
   }
   ~MyDialog()
   {
      std::cout << "########## destructor" << std::endl;
   }
};

void Application::showModalDialog(WText *out) const
{
   WDialog *dialog = new WDialog("Choose a cell");
   WLabel  *label  = new WLabel("Pick: A1 - Z999", dialog->contents());
   WLineEdit *edit = new WLineEdit(dialog->contents());
   label->setBuddy(edit);
   
   WRegExpValidator *valid = new WRegExpValidator("[A-Za-z][0-9][0-9]{0,2}");
   valid->setMandatory(true);
   edit->setValidator(valid);
   
   WPushButton *ok     = new WPushButton("ok",     dialog->footer());
   WPushButton *cancel = new WPushButton("cancel", dialog->footer());

   
   ok->setDisabled(true);

   edit->keyWentUp().connect(std::bind([=](){
            ok->setDisabled(!edit->validate());
         }));


   dialog->rejectWhenEscapePressed();
   
   ok->clicked().connect(std::bind([=](){
            if (edit->validate())
               dialog->accept();
            else
               dialog->reject();
         }));
   
   cancel->clicked().connect(std::bind([=](){dialog->reject();}));
   dialog->finished().connect(std::bind([=]() mutable{
            if (dialog->result() == Wt::WDialog::Accepted) {
               out->setText(edit->text() + " selected");
            } else {
               out->setText("nothing selected");
            }
            delete dialog;
         }));
   
   dialog->show();
}
