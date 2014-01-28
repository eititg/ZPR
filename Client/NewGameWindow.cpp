#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include "NewGameWindow.hpp"

namespace gui
{
    using boost::shared_ptr;

    NewGameWindow::NewGameWindow(shared_ptr<game::GameManager> gameManager, shared_ptr<game::ManageConnection> manageConnection):
        wxFrame(NULL, -1, wxT("Nowa gra"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER ^ wxMAXIMIZE_BOX),
        gameManager_(gameManager),
        manageConnection_(manageConnection)
    {
        manageConnection_->add(this);

        Connect(-1, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(gui::NewGameWindow::onClose));

        Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::NewGameWindow::onOkButton));
        Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::NewGameWindow::onCancelButton));

        wxPanel *mainPanel = new wxPanel(this);

        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText *nameStaticText = new wxStaticText(mainPanel, -1, wxT("Podaj nazwę nowej gry:"));
        nameTextCtrl = new wxTextCtrl(mainPanel, -1);

        nameTextCtrl->SetMinSize(wxSize(500, -1));

        wxBoxSizer *buttonBoxSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStdDialogButtonSizer *buttonSizer = new wxStdDialogButtonSizer;

        okButton = new wxButton(mainPanel, wxID_OK, wxT("Ok"));
        wxButton *cancelButton = new wxButton(mainPanel, wxID_CANCEL, wxT("Anuluj"));

        mainPanel->SetSizer(mainSizer);

        mainSizer->Add(nameStaticText, 1, wxEXPAND | wxALL, 10);
        mainSizer->Add(nameTextCtrl, 1, (wxEXPAND | wxALL) ^ wxTOP, 10);
        mainSizer->Add(buttonBoxSizer, 1, (wxEXPAND | wxALL) ^ wxTOP, 10);

        buttonBoxSizer->AddStretchSpacer(1);
        buttonBoxSizer->Add(buttonSizer, 0, wxEXPAND);

        buttonSizer->Add(okButton);
        buttonSizer->Add(cancelButton);

        mainSizer->Fit(this);

        MakeModal(true);

        Center();
    }

    NewGameWindow::~NewGameWindow()
    {
        manageConnection_->remove(this);
    }

    void NewGameWindow::update()
    {
        if (manageConnection_->isGameCreated())
        {
            wxString nameText = nameTextCtrl->GetLineText(0);

            std::string name = std::string(nameText.mb_str());

            gameManager_->joinGame(name);

            MakeModal(false);

            Destroy();
        }
        else if (!manageConnection_->isGameCreated())
        {
            nameTextCtrl->Enable();
            okButton->Enable();

            wxMessageBox(wxT("Na serwerze jest już gra o podanej nazwie, podaj inną nazwę."), wxT("Nowa gra"), wxOK | wxICON_INFORMATION);
        }
    }

    void NewGameWindow::onClose(wxCloseEvent&)
    {
        MakeModal(false);

        Destroy();
    }

    void NewGameWindow::onOkButton(wxCommandEvent&)
    {
        wxString nameText = nameTextCtrl->GetLineText(0);

        std::string name = std::string(nameText.mb_str());

        boost::regex reg("[a-zA-Z0-9]{1,16}");

        if (boost::regex_match(name, reg))
        {
            nameTextCtrl->Disable();
            okButton->Disable();

            manageConnection_->newGame(name);
        }
        else
        {
            wxMessageBox(wxT("Nazwa gry musi mieć od jednego do szesnastu znaków i może składać się z cyfr oraz małych i dużych liter."), wxT("Nowa gra"), wxOK | wxICON_INFORMATION);
        }
    }

    void NewGameWindow::onCancelButton(wxCommandEvent&)
    {
        Close();
    }
}
