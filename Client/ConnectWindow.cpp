#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include "ActivityGauge.hpp"
#include "ConnectWindow.hpp"
#include "LoginWindow.hpp"

namespace gui
{
    using boost::shared_ptr;

    ConnectWindow::ConnectWindow(shared_ptr<game::ManageConnection> manageConnection):
        wxFrame(NULL, -1, wxT("Serwer"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER ^ wxMAXIMIZE_BOX),
        manageConnection_(manageConnection)
    {
        manageConnection_->add(this);

        Connect(-1, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(gui::ConnectWindow::onClose));

        Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::ConnectWindow::onOkButton));
        Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::ConnectWindow::onCancelButton));

        wxPanel *mainPanel = new wxPanel(this);

        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText *serverStaticText = new wxStaticText(mainPanel, -1, wxT("Podaj adres serwera:"));
        serverTextCtrl = new wxTextCtrl(mainPanel, -1);

        serverTextCtrl->SetMinSize(wxSize(500, -1));

        gauge = new gui::ActivityGauge(mainPanel);

        wxBoxSizer *buttonBoxSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStdDialogButtonSizer *buttonSizer = new wxStdDialogButtonSizer;

        okButton = new wxButton(mainPanel, wxID_OK, wxT("Ok"));
        wxButton *cancelButton = new wxButton(mainPanel, wxID_CANCEL, wxT("Anuluj"));

        mainPanel->SetSizer(mainSizer);

        mainSizer->Add(serverStaticText, 1, wxEXPAND | wxALL, 10);
        mainSizer->Add(serverTextCtrl, 1, (wxEXPAND | wxALL) ^ wxTOP, 10);
        mainSizer->Add(gauge, 1, (wxEXPAND | wxALL) ^ wxTOP, 10);
        mainSizer->Add(buttonBoxSizer, 1, (wxEXPAND | wxALL) ^ wxTOP, 10);

        buttonBoxSizer->AddStretchSpacer(1);
        buttonBoxSizer->Add(buttonSizer, 0, wxEXPAND);

        buttonSizer->Add(okButton);
        buttonSizer->Add(cancelButton);

        mainSizer->Fit(this);

        MakeModal(true);

        Center();
    }

    ConnectWindow::~ConnectWindow()
    {
        manageConnection_->remove(this);
    }

    void ConnectWindow::update()
    {
        if (manageConnection_->isConnected())
        {
            MakeModal(false);

            gui::LoginWindow *loginWindow = new gui::LoginWindow(manageConnection_);

            loginWindow->Show();

            Destroy();
        }
        else if (!manageConnection_->isConnected())
        {
            serverTextCtrl->Enable();
            okButton->Enable();

            gauge->Animate(false);

            wxMessageBox(wxT("Nie można połączyć się z serwerem. Upewnij się, czy podany adres i numer portu są prawidłowe."), wxT("Serwer"), wxOK | wxICON_INFORMATION);
        }
    }

    void ConnectWindow::onClose(wxCloseEvent& event)
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

    void ConnectWindow::onOkButton(wxCommandEvent &event)
    {
        wxString serverAddress = serverTextCtrl->GetLineText(0);

        std::string address = std::string(serverAddress.mb_str());

        boost::regex reg("(([1-9]\\d{0,2})|(0)).(([1-9]\\d{0,2})|(0)).(([1-9]\\d{0,2})|(0)).(([1-9]\\d{0,2})|(0)):(([1-9]\\d{0,4})|(0))");
        boost::smatch group;

        bool good = boost::regex_match(address, group, reg);

        if (good)
        {
            unsigned int token;

            for (unsigned i = 1; i <= 10; i += 3)
            {
                token = boost::lexical_cast<unsigned int>(group[i]);

                if (token > 255)
                {
                    good = false;

                    break;
                }
            }

            token = boost::lexical_cast<unsigned int>(group[13]);

            if (token > 65535)
                good = false;

            if (good)
            {
                std::string addressIp = group[1] + "." + group[4] + "." + group[7] + "." + group[10];

                unsigned short port = boost::lexical_cast<unsigned short>(group[13]);

                manageConnection_->connect(addressIp, port);
            }
        }

        if (good)
        {
            serverTextCtrl->Disable();
            okButton->Disable();

            gauge->Animate(true);
        }
        else
        {
            wxMessageBox(wxT("Podaj adres serwera i port w formacie X.X.X.X:X."), wxT("Serwer"), wxOK | wxICON_INFORMATION);
        }
    }

    void ConnectWindow::onCancelButton(wxCommandEvent &event)
    {
        Close();
    }
}
