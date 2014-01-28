#include <boost/lexical_cast.hpp>
#include <string>
#include "GameListCtrl.hpp"

namespace gui
{
    using boost::shared_ptr;

    GameListCtrl::GameListCtrl(wxWindow *parent, shared_ptr<game::GameManager> gameManager):
        wxListCtrl(parent, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL | wxLC_SINGLE_SEL),
        gameManager_(gameManager)
    {
        gameManager_->add(this);

        Connect(-1, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(gui::GameListCtrl::onItemSelected));

        InsertColumn(0, wxT("Nazwa"), wxLIST_FORMAT_LEFT, 250);
        InsertColumn(1, wxT("Graczy"), wxLIST_FORMAT_LEFT, 80);
    }

    wxString GameListCtrl::OnGetItemText(long item, long column) const
    {
        if (column == 0)
        {
            std::string name = gameManager_->getGameName(item);

            return wxString(name.c_str(), wxConvUTF8);
        }
        else
        {
            std::string players = boost::lexical_cast<std::string>(gameManager_->getPlayerCount(item));

            players += "/4";

            return wxString(players.c_str(), wxConvUTF8);
        }
    }

    void GameListCtrl::update()
    {
        if (gameManager_->isJoined())
            Disable();
        else
            Enable();

        unsigned int items = gameManager_->getListSize();

        SetItemCount(items);

        RefreshItems(0, items - 1);
    }

    void GameListCtrl::onItemSelected(wxListEvent &event)
    {
        unsigned int index = event.GetIndex();

        gameManager_->selectGame(index);
    }
}
