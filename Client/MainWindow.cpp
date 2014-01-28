#include "GameDetailPanel.hpp"
#include "GameListCtrl.hpp"
#include "MainWindow.hpp"
#include "NewGameWindow.hpp"

namespace gui
{
    using boost::shared_ptr;

    MainWindow::MainWindow(shared_ptr<game::GameManager> gameManager, shared_ptr<game::ManageConnection> manageConnection):
        wxFrame(NULL, -1, wxT("ZPR"), wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER ^ wxMAXIMIZE_BOX),
        gameManager_(gameManager),
        manageConnection_(manageConnection)
    {
        Connect(ID_NEWGAME_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(gui::MainWindow::onNewGameButton));

        wxPanel *mainPanel = new wxPanel(this);

        wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);

        wxPanel *leftPanel = new wxPanel(mainPanel);
        gui::GameDetailPanel *gameDetailPanel = new gui::GameDetailPanel(mainPanel, gameManager);

        wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticBoxSizer *gameListSizer = new wxStaticBoxSizer(wxVERTICAL, leftPanel, wxT("Gry"));

        mainPanel->SetSizer(mainSizer);

        mainSizer->Add(leftPanel, 1, wxEXPAND);
        mainSizer->Add(gameDetailPanel, 1, wxEXPAND);

        leftPanel->SetSizer(leftSizer);

        leftSizer->Add(gameListSizer, 1, wxEXPAND | wxALL, 5);

        gui::GameListCtrl *gameListCtrl = new gui::GameListCtrl(leftPanel, gameManager);
        wxButton *newGameButton = new wxButton(leftPanel, ID_NEWGAME_BUTTON, wxT("Nowa gra"));

        gameListSizer->Add(gameListCtrl, 9, wxEXPAND | wxALL, 5);
        gameListSizer->Add(newGameButton, 1, wxEXPAND | wxALL, 5);

        Center();
    }

    void MainWindow::onNewGameButton(wxCommandEvent&)
    {
        gui::NewGameWindow *newGameWindow = new gui::NewGameWindow(gameManager_, manageConnection_);

        newGameWindow->Show();
    }
}
