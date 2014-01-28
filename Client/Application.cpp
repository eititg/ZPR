#include "Application.hpp"
#include "ConnectWindow.hpp"
#include "MainWindow.hpp"

IMPLEMENT_APP(Application)

Application::Application():
    networkTimer_(new wxTimer),
    netService_(new net::NetService),
    gameManager_(new game::GameManager),
    manageConnection_(new game::ManageConnection(netService_, gameManager_))
{
    gameManager_->assignConnection(manageConnection_);
}

bool Application::OnInit()
{
    Connect(-1, wxEVT_IDLE, wxIdleEventHandler(Application::onIdle));
    Connect(-1, wxEVT_TIMER, wxTimerEventHandler(Application::onTimer));

    networkTimer_->SetOwner(this);
    networkTimer_->Start(1);

    gui::MainWindow *mainWindow = new gui::MainWindow(gameManager_, manageConnection_);

    mainWindow->Show();

    gui::ConnectWindow *connectWindow = new gui::ConnectWindow(manageConnection_);

    connectWindow->Show();

    return true;
}

void Application::onIdle(wxIdleEvent &event)
{
    netService_->poll();

    event.RequestMore();
}

void Application::onTimer(wxTimerEvent&)
{
    netService_->poll();
}
