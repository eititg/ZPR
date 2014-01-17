#ifndef SOCKETTCP_HPP
#define SOCKETTCP_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <list>
#include <string>
#include <vector>

namespace net
{
    using boost::shared_ptr;
    using boost::system::error_code;
    using namespace boost::asio;

    class AcceptorTCP;
    class NetService;

    class SocketTCP
    {
        friend AcceptorTCP;

        public:
            SocketTCP(shared_ptr<NetService> netService, std::size_t recvBufferSize = 1024);
            virtual ~SocketTCP();

            void connect(const std::string &host, unsigned short port);

            void send(std::string msg);
            void recv(std::size_t bytes = 0);

            void close();

            std::size_t getRecvBufferSize() const;
            void setRecvBufferSize(std::size_t size);

        private:
            void handleConnect(const error_code &error);

            void dispatchSend(std::list<std::string>::iterator it);
            void handleSend(std::list<std::string>::iterator it, const error_code &error);

            void dispatchRecv(std::size_t bytes);
            void handleRecv(std::size_t bytes, const error_code &error);

            void startError(const error_code &error);

        protected:
            virtual void onConnect();
            virtual void onSend(const std::string &message);
            virtual void onRecv(std::vector<boost::uint8_t> &buffer);
            virtual void onError(const error_code &error);

        private:
            SocketTCP(const SocketTCP&);
            SocketTCP& operator=(const SocketTCP&);

            shared_ptr<NetService> netService_;
            shared_ptr<io_service::strand> strand_;
            shared_ptr<ip::tcp::socket> socket_;

            std::list<std::string> sendBuffer_;
            std::vector<boost::uint8_t> recvBuffer_;

            std::size_t recvBufferSize_;
    };
}

#endif // SOCKETTCP_HPP
