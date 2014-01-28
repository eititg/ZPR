#include <boost/regex.hpp>
#include "LoginWindow.hpp"

namespace gui
{
    LoginWindow::LoginWindow(boost::shared_ptr<game::ManageConnection> manageConnection):
        wxFrame(NULL, -1, wxT("Login"), wxDefaultPosition, wxSize(300, 200), wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER ^ wxMAXIMIZE_BOX),
        manageConnection_(manageConnection)
    {
        manageConnection->add(this);

        Connect(-1, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(gui::LoginWindow::onClose));

        Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::LoginWindow::onOkButton));
        Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::LoginWindow::onCancelButton));

        wxPanel *mainPanel = new wxPanel(this);

        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText *loginStaticText = new wxStaticText(mainPanel, -1, wxT("Podaj nazwę użytkownika:"));
        loginTextCtrl = new wxTextCtrl(mainPanel, -1);

        loginTextCtrl->SetMinSize(wxSize(500, -1));

        wxBoxSizer *buttonBoxSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStdDialogButtonSizer *buttonSizer = new wxStdDialogButtonSizer;

        okButton = new wxButton(mainPanel, wxID_OK, wxT("Ok"));
        wxButton *cancelButton = new wxButton(mainPanel, wxID_CANCEL, wxT("Anuluj"));

        mainPanel->SetSizer(mainSizer);

        mainSizer->Add(loginStaticText, 1, wxEXPAND | wxALL, 10);
        mainSizer->Add(loginTextCtrl, 1, (wxEXPAND | wxALL) ^ wxTOP, 10);
        mainSizer->Add(buttonBoxSizer, 1, (wxEXPAND | wxALL) ^ wxTOP, 10);

        buttonBoxSizer->AddStretchSpacer(1);
        buttonBoxSizer->Add(buttonSizer, 0, wxEXPAND);

        buttonSizer->Add(okButton);
        buttonSizer->Add(cancelButton);

        mainSizer->Fit(this);

        MakeModal(true);

        Center();
    }

    LoginWindow::~LoginWindow()
    {
        manageConnection_->remove(this);
    }

    void LoginWindow::update()
    {
        if (manageConnection_->isLogged())
        {
            manageConnection_->getGameList();

            MakeModal(false);

            Destroy();
        }
        else if (!manageConnection_->isLogged())
        {
            loginTextCtrl->Enable();
            okButton->Enable();

            wxMessageBox(wxT("Na serwerze jest już zalogowany użytkownik o podanej nazwie, podaj inną nazwę."), wxT("Login"), wxOK | wxICON_INFORMATION);
        }
    }

    void LoginWindow::onClose(wxCloseEvent &event)
    {
        int close = wxMessageBox(wxT("Czy chcesz zakończyć aplikację?"), wxT("Koniec"), wxYES_NO | wxICON_QUESTION, this);

        if (close == wxYES)
        {
            for (wxWindowList::Node *node = wxTopLevelWindows.GetFirst(); node; node = node->GetNext())
            {
                wxWindow *window = (wxWindow*)node->GetData();

                window->Destroy();
            }
        }
        else
        {
            if (event.CanVeto())
                event.Veto();
        }
    }

    void LoginWindow::onOkButton(wxCommandEvent&)
    {
        wxString loginText = loginTextCtrl->GetLineText(0);

        std::string login = std::string(loginText.mb_str());

        boost::regex reg("[a-zA-Z0-9]{1,16}");

        if (boost::regex_match(login, reg))
        {
            loginTextCtrl->Disable();
            okButton->Disable();

            manageConnection_->logIn(login);
        }
        else
        {
            wxMessageBox(wxT("Nazwa użytkownika musi mieć od jednego do szesnastu znaków i może składać się z cyfr oraz małych i dużych liter."), wxT("Login"), wxOK | wxICON_INFORMATION);
        }
    }

    void LoginWindow::onCancelButton(wxCommandEvent&)
    {
        Close();
    }
}
