#include "GameDetailPanel.hpp"

namespace gui
{
    using boost::shared_ptr;

    GameDetailPanel::GameDetailPanel(wxWindow *parent, shared_ptr<game::GameManager> gameManager):
        wxPanel(parent),
        gameManager_(gameManager)
    {
        gameManager_->add(this);

        Connect(ID_JOIN_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::GameDetailPanel::onJoinButton));
        Connect(ID_DISJOIN_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::GameDetailPanel::onDisjoinButton));

        Connect(ID_START_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::GameDetailPanel::onStartButton));

        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticBoxSizer *gameDetailSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Szczegóły gry"));

        wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText *nameStaticText = new wxStaticText(this, -1, wxT("Nazwa gry:"));
        nameTextCtrl = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_RIGHT);

        playerList = new wxListCtrl(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);

        playerList->InsertColumn(0, wxT("Gracz"));

        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

        joinButton = new wxButton(this, ID_JOIN_BUTTON, wxT("Dołącz do gry"));
        disjoinButton = new wxButton(this, ID_DISJOIN_BUTTON, wxT("Opuść grę"));
        startButton = new wxButton(this, ID_START_BUTTON, wxT("Rozpocznij grę"));

        mainSizer->Add(gameDetailSizer, 1, wxEXPAND | wxALL, 5);

        gameDetailSizer->Add(nameSizer, 0, wxEXPAND | wxALL, 5);

        nameSizer->Add(nameStaticText, 1, wxEXPAND);
        nameSizer->Add(nameTextCtrl, 1, wxEXPAND);

        gameDetailSizer->Add(playerList, 1, (wxEXPAND | wxALL) ^ wxTOP, 5);

        gameDetailSizer->Add(buttonSizer, 0, wxEXPAND);

        buttonSizer->Add(joinButton, 0, wxEXPAND | wxALL, 5);
        buttonSizer->Add(disjoinButton, 0, (wxEXPAND | wxALL) ^ wxLEFT, 5);
        buttonSizer->AddStretchSpacer(1);
        buttonSizer->Add(startButton, 0, wxEXPAND | wxALL, 5);

        SetSizer(mainSizer);
    }

    GameDetailPanel::~GameDetailPanel()
    {
        gameManager_->remove(this);
    }

    void GameDetailPanel::update()
    {
        if (gameManager_->isJoined())
        {
            joinButton->Disable();
            disjoinButton->Enable();
        }
        else
        {
            joinButton->Enable();
            disjoinButton->Disable();
        }

        if (gameManager_->isNewGame())
            startButton->Enable();
        else
            startButton->Disable();

        std::string name = gameManager_->getSelectedName();

        wxString nameText(name.c_str(), wxConvUTF8);

        nameTextCtrl->SetValue(nameText);

        unsigned short players = gameManager_->getSelectedPlayerCounter();

        std::string playerName;
        wxString playerNameText;

        playerList->DeleteAllItems();

        for (unsigned short i = 0; i < players; ++i)
        {
            playerName = gameManager_->getSelectedPlayerName(i);

            nameText = wxString(name.c_str(), wxConvUTF8);

            playerList->InsertItem(i, playerNameText);
        }
    }

    void GameDetailPanel::onJoinButton(wxCommandEvent&)
    {
        gameManager_->joinGame();
    }

    void GameDetailPanel::onDisjoinButton(wxCommandEvent&)
    {
        gameManager_->disjoinGame();
    }

    void GameDetailPanel::onStartButton(wxCommandEvent&)
    {

    }
}
