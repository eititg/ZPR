#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "AcceptorTCP.hpp"
#include "NetService.hpp"
#include "SocketTCP.hpp"

namespace net
{
    using boost::shared_ptr;
    using boost::system::error_code;
    using boost::lexical_cast;
    using namespace boost::asio;

    AcceptorTCP::AcceptorTCP(shared_ptr<NetService> netService):
        netService_(netService),
        strand_(new io_service::strand(*netService->io_service_)),
        acceptor_(new ip::tcp::acceptor(*netService->io_service_))
    {
    }

    AcceptorTCP::~AcceptorTCP()
    {
    }

    void AcceptorTCP::bind(const std::string &host, unsigned short port)
    {
        ip::tcp::resolver resolver(*netService_->io_service_);
        ip::tcp::resolver::query query(host, lexical_cast<std::string>(port));

        ip::tcp::endpoint endpoint = *resolver.resolve(query);

        acceptor_->open(endpoint.protocol());
        acceptor_->bind(endpoint);
    }

    void AcceptorTCP::listen()
    {
        acceptor_->listen();
    }

    void AcceptorTCP::accept(shared_ptr<SocketTCP> socketTCP)
    {
        strand_->post(boost::bind(&AcceptorTCP::dispatchAccept, this, socketTCP));
    }

    void AcceptorTCP::dispatchAccept(shared_ptr<SocketTCP> socketTCP)
    {
        acceptor_->async_accept(*socketTCP->socket_, strand_->wrap(boost::bind(&AcceptorTCP::handleAccept, this, socketTCP, _1)));
    }

    void AcceptorTCP::handleAccept(shared_ptr<SocketTCP> socketTCP, const error_code &error)
    {
        if (!error)
        {
            onAccept(socketTCP);
        }
    }

    void AcceptorTCP::onAccept(shared_ptr<SocketTCP>)
    {
    }
}
