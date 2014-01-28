#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "NetService.hpp"
#include "SocketTCP.hpp"

namespace net
{
    using boost::bind;
    using boost::lexical_cast;
    using boost::shared_ptr;
    using boost::system::error_code;
    using namespace boost::asio;

    SocketTCP::SocketTCP(shared_ptr<NetService> netService, std::size_t recvBufferSize):
        netService_(netService),
        strand_(new io_service::strand(*netService->io_service_)),
        socket_(new ip::tcp::socket(*netService->io_service_)),
        recvBufferSize_(recvBufferSize)
    {
    }

    SocketTCP::~SocketTCP()
    {
        close();
    }

    void SocketTCP::connect(const std::string &host, unsigned short port)
    {
        ip::tcp::resolver resolver(*netService_->io_service_);
        ip::tcp::resolver::query query(host, lexical_cast<std::string>(port));

        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

        socket_->async_connect(endpoint, strand_->wrap(bind(&SocketTCP::handleConnect, this, _1)));
    }

    void SocketTCP::send(std::vector<boost::uint8_t> &buffer)
    {
        std::list<std::vector<boost::uint8_t> >::iterator it = sendBuffer_.insert(sendBuffer_.end(), buffer);

        strand_->post(bind(&SocketTCP::dispatchSend, this, it));
    }

    void SocketTCP::recv(std::size_t bytes)
    {
        strand_->post(bind(&SocketTCP::dispatchRecv, this, bytes));
    }

    std::size_t SocketTCP::getRecvBufferSize() const
    {
        return recvBufferSize_;
    }

    void SocketTCP::close()
    {
        error_code error;

        socket_->shutdown(ip::tcp::socket::shutdown_both, error);
        socket_->close(error);
    }

    void SocketTCP::setRecvBufferSize(std::size_t size)
    {
        recvBufferSize_ = size;
    }

    void SocketTCP::handleConnect(const error_code &error)
    {
        if (!error)
            onConnect();
        else
            startError(error);
    }

    void SocketTCP::dispatchSend(std::list<std::vector<boost::uint8_t> >::iterator it)
    {
        socket_->async_send(boost::asio::buffer(*it), strand_->wrap(bind(&SocketTCP::handleSend, this, it, _1)));
    }

    void SocketTCP::handleSend(std::list<std::vector<boost::uint8_t> >::iterator it, const error_code &error)
    {
        if (!error)
        {
            onSend(*it);

            sendBuffer_.erase(it);
        }
        else
            startError(error);
    }

    void SocketTCP::dispatchRecv(std::size_t bytes)
    {
        if (bytes > 0)
        {
            recvBuffer_.resize(bytes);

            async_read(*socket_, buffer(recvBuffer_), strand_->wrap(bind(&SocketTCP::handleRecv, this, _2, _1)));
        }
        else
        {
            recvBuffer_.resize(recvBufferSize_);

            socket_->async_read_some(buffer(recvBuffer_), strand_->wrap(bind(&SocketTCP::handleRecv, this, _2, _1)));
        }
    }

    void SocketTCP::handleRecv(std::size_t bytes, const error_code &error)
    {
        if (!error)
        {
            recvBuffer_.resize(bytes);

            onRecv(recvBuffer_);
        }
        else if (bytes != 0)
            startError(error);
    }

    void SocketTCP::startError(const error_code &error)
    {
        close();

        onError(error);
    }

    void SocketTCP::onConnect()
    {
    }

    void SocketTCP::onSend(const std::vector<boost::uint8_t>&)
    {
    }

    void SocketTCP::onRecv(std::vector<boost::uint8_t>&)
    {
    }

    void SocketTCP::onError(const error_code&)
    {
    }
}
