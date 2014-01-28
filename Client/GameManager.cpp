#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include "GameManager.hpp"
#include "ManageConnection.hpp"

namespace game
{
    using boost::shared_ptr;

    GameManager::GameManager():
        joined_(false)
    {
    }

    void GameManager::assignConnection(shared_ptr<game::ManageConnection> manageConnection)
    {
        manageConnection_ = manageConnection;
    }

    void GameManager::addGame(unsigned short players, std::string name)
    {
        games_.push_back(std::make_pair(players, name));

        std::sort(games_.begin(), games_.end(), boost::bind(&game::GameManager::sortCompare, this, _1, _2));

        notify();
    }

    void GameManager::removeGame(std::string name)
    {
        std::vector<std::pair<unsigned short, std::string> >::iterator it;

        for (it = games_.begin(); it != games_.end(); ++it)
        {
            if (it->second == name)
            {
                games_.erase(it);

                break;
            }
        }

        notify();
    }

    void GameManager::joinGame(std::string name)
    {
        gameName_ = name;
        newGame_ = true;

        joined_ = true;

        manageConnection_->joinGame(name);

        notify();
    }

    void GameManager::joinGame()
    {
        newGame_ = false;

        manageConnection_->joinGame(gameName_);

        notify();
    }

    void GameManager::disjoinGame()
    {
        joined_ = false;

        manageConnection_->disjoinGame(gameName_);

        notify();
    }

    void GameManager::joined()
    {
        joined_ = true;

        notify();
    }

    void GameManager::selectGame(unsigned int index)
    {
        gameName_ = getGameName(index);

        manageConnection_->getGameDetails(gameName_);

        players_.clear();

        notify();
    }

    void GameManager::setGameDetails(std::vector<std::string> namesVec)
    {
        players_ = namesVec;
    }

    std::string GameManager::getSelectedName()
    {
        return gameName_;
    }

    unsigned short GameManager::getSelectedPlayerCounter()
    {
        return players_.size();
    }

    std::string GameManager::getSelectedPlayerName(unsigned short index)
    {
        return players_[index];
    }

    unsigned short GameManager::getPlayerCount(unsigned int index) const
    {
        return games_[index].first;
    }

    std::string GameManager::getGameName(unsigned int index) const
    {
        return games_[index].second;
    }

    unsigned int GameManager::getListSize() const
    {
        return games_.size();
    }

    bool GameManager::isNewGame() const
    {
        return newGame_;
    }

    bool GameManager::isJoined() const
    {
        return joined_;
    }

    bool GameManager::sortCompare(const std::pair<unsigned short, std::string> &first, const std::pair<unsigned short, std::string> &second)
    {
        using boost::algorithm::to_lower_copy;

        return (to_lower_copy(first.second) < to_lower_copy(second.second));
    }
}
