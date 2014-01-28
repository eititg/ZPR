#include <boost/lexical_cast.hpp>
#include "ManageConnection.hpp"
#include "../Network/Packet.hpp"
#include "../Util/BufferPack.hpp"

namespace game
{
    using boost::lexical_cast;
    using boost::logic::tribool;
    using boost::logic::indeterminate;
    using boost::shared_ptr;
    using boost::system::error_code;

    ManageConnection::ManageConnection(shared_ptr<net::NetService> netService, shared_ptr<game::GameManager> gameManager):
        SocketTCP(netService),
        connected_(indeterminate),
        logged_(indeterminate),
        gameCreated_(indeterminate),
        gameManager_(gameManager)
    {
    }

    ManageConnection::~ManageConnection()
    {
        logOut();
    }

    void ManageConnection::logIn(const std::string &login)
    {
        logged_ = indeterminate;

        std::vector<boost::uint8_t> buffer;

        util::pack(buffer, static_cast<boost::uint8_t>(net::PACKET_LOGIN_REQ));
        util::pack(buffer, static_cast<boost::uint16_t>(login.size()));
        util::pack(buffer, login);

        send(buffer);
    }

    void ManageConnection::logOut()
    {
        logged_ = false;

        std::vector<boost::uint8_t> buffer;

        util::pack(buffer, static_cast<boost::uint8_t>(net::PACKET_LOGOUT_REQ));
        util::pack(buffer, static_cast<boost::uint16_t>(0));

        send(buffer);
    }

    void ManageConnection::getGameList()
    {
        std::vector<boost::uint8_t> buffer;

        util::pack(buffer, static_cast<boost::uint8_t>(net::PACKET_GET_GAME_LIST_REQ));
        util::pack(buffer, static_cast<boost::uint16_t>(0));

        send(buffer);
    }

    void ManageConnection::newGame(const std::string &name)
    {
        gameCreated_ = indeterminate;

        std::vector<boost::uint8_t> buffer;

        util::pack(buffer, static_cast<boost::uint8_t>(net::PACKET_NEW_GAME_REQ));
        util::pack(buffer, static_cast<boost::uint16_t>(name.size() + sizeof(boost::uint8_t)));
        util::pack(buffer, static_cast<boost::uint8_t>(0));
        util::pack(buffer, name);

        send(buffer);
    }

    void ManageConnection::joinGame(const std::string &name)
    {
        std::vector<boost::uint8_t> buffer;

        util::pack(buffer, static_cast<boost::uint8_t>(net::PACKET_SUBSCRIBE_REQ));
        util::pack(buffer, static_cast<boost::uint16_t>(name.size()));
        util::pack(buffer, name);

        send(buffer);
    }

    void ManageConnection::disjoinGame(const std::string &name)
    {
        std::vector<boost::uint8_t> buffer;

        util::pack(buffer, static_cast<boost::uint8_t>(net::PACKET_UNSUBSCRIBE_REQ));
        util::pack(buffer, static_cast<boost::uint16_t>(name.size()));
        util::pack(buffer, name);

        send(buffer);
    }

    void ManageConnection::getGameDetails(const std::string &name)
    {
        std::vector<boost::uint8_t> buffer;

        util::pack(buffer, static_cast<boost::uint8_t>(net::PACKET_GAME_DETAILS_REQ));
        util::pack(buffer, static_cast<boost::uint16_t>(name.size()));
        util::pack(buffer, name);

        send(buffer);
    }

    tribool ManageConnection::isConnected() const
    {
        return connected_;
    }

    tribool ManageConnection::isLogged() const
    {
        return logged_;
    }

    tribool ManageConnection::isGameCreated() const
    {
        return gameCreated_;
    }

    void ManageConnection::onConnect()
    {
        connected_ = true;

        notify();
    }

    void ManageConnection::onError(const error_code &error)
    {
        connected_ = false;

        notify();
    }

    void ManageConnection::onSend(const std::vector<boost::uint8_t> &buffer)
    {
        recv();
    }

    void ManageConnection::onRecv(std::vector<boost::uint8_t> &buffer)
    {
        std::vector<boost::uint8_t>::iterator it;

        buffer_.insert(buffer_.end(), buffer.begin(), buffer.end());

        unsigned pointer = 1;

        unsigned short bufferSize = util::unpack<uint16_t>(buffer_, pointer);

        if (buffer_.size() >= bufferSize)
        {
            parsePacket();
        }
    }

    void ManageConnection::parsePacket()
    {
        unsigned pointer = 0;

        int type = util::unpack<boost::uint8_t>(buffer_, pointer);
        unsigned size = util::unpack<boost::uint16_t>(buffer_, pointer);

        switch (type)
        {
            case net::PACKET_LOGIN_RES:
            {
                unsigned short response = util::unpack<boost::uint8_t>(buffer_, pointer);

                logged_ = (response == net::PACKET_YES);

                notify();

                break;
            }

            case net::PACKET_GAME_DETAILS_RES:
            {
                unsigned short players = util::unpack<boost::uint8_t>(buffer_, pointer);

                std::vector<std::string> namesVec;

                unsigned short nameLen;
                std::string name;

                for (unsigned short i = 0; i < players; ++i)
                {
                    nameLen = util::unpack<boost::uint8_t>(buffer_, pointer);

                    name = util::unpack(buffer_, nameLen, pointer);

                    namesVec.push_back(name);
                }

                gameManager_->setGameDetails(namesVec);

                break;
            }

            case net::PACKET_GAME_NAME_RES:
            {
                unsigned short gameType = util::unpack<boost::uint8_t>(buffer_, pointer);
                unsigned short players = util::unpack<boost::uint8_t>(buffer_, pointer);
                std::string gameName = util::unpack(buffer_, size - sizeof(boost::uint8_t), pointer);

                gameManager_->addGame(players, gameName);

                recv();

                break;
            }

            case net::PACKET_NEW_GAME_RES:
            {
                unsigned short response = util::unpack<boost::uint8_t>(buffer_, pointer);

                gameCreated_ = (response == net::PACKET_YES);

                notify();

                break;
            }

            case net::PACKET_DELETE_GAME_RES:
            {
                std::string name = util::unpack(buffer_, size, pointer);

                gameManager_->removeGame(name);

                break;
            }

            case net::PACKET_SUBSCRIBE_RES:
            {
                unsigned short response = util::unpack<boost::uint8_t>(buffer_, pointer);

                if (response == net::PACKET_YES)
                    gameManager_->joined();

                break;
            }

            case net::PACKET_UNSUBSCRIBE_RES:
            {
                util::unpack<boost::uint8_t>(buffer_, pointer);

                break;
            }
        }

        buffer_.erase(buffer_.begin(), buffer_.begin() + pointer);

        pointer = 1;

        unsigned short bufferSize = util::unpack<uint16_t>(buffer_, pointer);

        if (buffer_.size() >= bufferSize)
        {
            parsePacket();
        }
    }
}
