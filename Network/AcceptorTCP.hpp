#ifndef ACCEPTORTCP_HPP
#define ACCEPTORTCP_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <string>

namespace net
{
    using boost::shared_ptr;
    using boost::system::error_code;
    using namespace boost::asio;

    class NetService;
    class SocketTCP;

    class AcceptorTCP
    {
        public:
            AcceptorTCP(shared_ptr<NetService> netService);
            virtual ~AcceptorTCP();

            void bind(const std::string &host, unsigned short port);
            void listen();

            void accept(shared_ptr<SocketTCP> socketTCP);

        private:
            void dispatchAccept(shared_ptr<SocketTCP> socketTCP);
            void handleAccept(shared_ptr<SocketTCP> socketTCP, const error_code &error);

        protected:
            virtual void onAccept(shared_ptr<SocketTCP> socketTCP);

        private:
            AcceptorTCP(const AcceptorTCP&);
            AcceptorTCP& operator=(const AcceptorTCP&);

            shared_ptr<NetService> netService_;
            shared_ptr<io_service::strand> strand_;
            shared_ptr<ip::tcp::acceptor> acceptor_;
    };
}

#endif // ACCEPTORTCP_HPP
