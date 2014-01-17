#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "NetService.hpp"
#include "SocketUDP.hpp"

namespace net
{
    using boost::shared_ptr;
    using boost::lexical_cast;
    using namespace boost::asio;

    SocketUDP::SocketUDP(shared_ptr<NetService> netService, std::size_t recvBufferSize):
        netService_(netService),
        strand_(new io_service::strand(*netService->io_service_)),
        socket_(new ip::udp::socket(*netService->io_service_)),
        recvBufferSize_(recvBufferSize)
    {
    }

    SocketUDP::~SocketUDP()
    {
    }

    void SocketUDP::bind(const std::string &host, unsigned short port)
    {
        ip::udp::resolver resolver(*netService_->io_service_);
        ip::udp::resolver::query query(host, lexical_cast<std::string>(port));

        ip::udp::endpoint endpoint = *resolver.resolve(query);

        socket_->open(endpoint.protocol());
        socket_->bind(endpoint);
    }

    void SocketUDP::connect(const std::string &host, unsigned short port)
    {
        ip::udp::resolver resolver(*netService_->io_service_);
        ip::udp::resolver::query query(host, lexical_cast<std::string>(port));

        ip::udp::endpoint endpoint = *resolver.resolve(query);

        socket_->async_connect(endpoint, strand_->wrap(boost::bind(&SocketUDP::handleConnect, this, _1)));
    }

    void SocketUDP::send(std::string message)
    {
        std::list<std::string>::iterator it = sendBuffer_.insert(sendBuffer_.end(), message);

        strand_->post(boost::bind(&SocketUDP::dispatchSend, this, it));
    }

    void SocketUDP::sendTo(const std::string &ip, unsigned short port, std::string message)
    {
        std::list<std::string>::iterator it = sendBuffer_.insert(sendBuffer_.end(), message);

        strand_->post(boost::bind(&SocketUDP::dispatchSendTo, this, ip, port, it));
    }

    void SocketUDP::recv()
    {
        strand_->post(boost::bind(&SocketUDP::dispatchRecv, this));
    }

    void SocketUDP::recvFrom()
    {
        strand_->post(boost::bind(&SocketUDP::dispatchRecvFrom, this));
    }

    void SocketUDP::close()
    {
        error_code error;

        socket_->shutdown(ip::udp::socket::shutdown_both, error);
        socket_->close(error);
    }

    std::size_t SocketUDP::getRecvBufferSize() const
    {
        return recvBufferSize_;
    }

    void SocketUDP::setRecvBufferSize(std::size_t size)
    {
        recvBufferSize_ = size;
    }

    void SocketUDP::handleConnect(const error_code &error)
    {
        if (!error)
            onConnect();
        else
            startError(error);
    }

    void SocketUDP::dispatchSend(std::list<std::string>::iterator it)
    {
        socket_->async_send(boost::asio::buffer(*it), strand_->wrap(boost::bind(&SocketUDP::handleSend, this, it, _1)));
    }

    void SocketUDP::handleSend(std::list<std::string>::iterator it, const error_code &error)
    {
        if (!error)
        {
            onSend(*it);

            sendBuffer_.erase(it);
        }
        else
            startError(error);
    }

    void SocketUDP::dispatchSendTo(const std::string &ip, unsigned short port, std::list<std::string>::iterator it)
    {
        ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);

        socket_->async_send_to(buffer(*it), endpoint, strand_->wrap(boost::bind(&SocketUDP::handleSendTo, this, ip, port, it, _1)));
    }

    void SocketUDP::handleSendTo(const std::string &ip, unsigned short port, std::list<std::string>::iterator it, const error_code &error)
    {
        if (!error)
        {
            onSendTo(ip, port, *it);

            sendBuffer_.erase(it);
        }
        else
            startError(error);
    }

    void SocketUDP::dispatchRecv()
    {
        recvBuffer_.resize(recvBufferSize_);

        socket_->async_receive(boost::asio::buffer(recvBuffer_), strand_->wrap(boost::bind(&SocketUDP::handleRecv, this, _2, _1)));
    }

    void SocketUDP::handleRecv(std::size_t bytes, const error_code &error)
    {
        if (!error)
        {
            recvBuffer_.resize(bytes);

            onRecv(recvBuffer_);
        }
        else if (bytes != 0)
            startError(error);
    }

    void SocketUDP::dispatchRecvFrom()
    {
        shared_ptr<ip::udp::endpoint> endpoint(new ip::udp::endpoint);

        recvBuffer_.resize(recvBufferSize_);

        socket_->async_receive_from(buffer(recvBuffer_), *endpoint, strand_->wrap(boost::bind(&SocketUDP::handleRecvFrom, this, endpoint, _2, _1)));
    }

    void SocketUDP::handleRecvFrom(shared_ptr<boost::asio::ip::udp::endpoint> endpoint, std::size_t bytes, const error_code &error)
    {
        if (!error)
        {
            recvBuffer_.resize(bytes);

            ip::address address = endpoint->address();

            onRecvFrom(address.to_string(), endpoint->port(), recvBuffer_);
        }
        else if (bytes != 0)
            startError(error);
    }

    void SocketUDP::startError(const error_code &error)
    {
        close();

        onError(error);
    }

    void SocketUDP::onConnect()
    {
    }

    void SocketUDP::onSend(const std::string&)
    {
    }

    void SocketUDP::onSendTo(const std::string&, unsigned short, const std::string&)
    {
    }

    void SocketUDP::onRecv(std::vector<boost::uint8_t>&)
    {
    }

    void SocketUDP::onRecvFrom(const std::string&, unsigned short, std::vector<boost::uint8_t>&)
    {
    }

    void SocketUDP::onError(const error_code &error)
    {
    }
}
