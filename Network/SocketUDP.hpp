#ifndef SOCKETUDP_HPP
#define SOCKETUDP_HPP

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

    class NetService;

    class SocketUDP
    {
        public:
            SocketUDP(shared_ptr<NetService> netService, std::size_t recvBufferSize = 1024);
            virtual ~SocketUDP();

            void bind(const std::string &host, unsigned short port);

            void connect(const std::string &host, unsigned short port);

            void send(std::string message);
            void sendTo(const std::string &ip, unsigned short port, std::string message);

            void recv();
            void recvFrom();

            void close();

            std::size_t getRecvBufferSize() const;
            void setRecvBufferSize(std::size_t size);

        private:
            void handleConnect(const error_code &error);

            void dispatchSend(std::list<std::string>::iterator it);
            void handleSend(std::list<std::string>::iterator it, const error_code &error);

            void dispatchSendTo(const std::string &ip, unsigned short port, std::list<std::string>::iterator it);
            void handleSendTo(const std::string &ip, unsigned short port, std::list<std::string>::iterator it, const error_code &error);

            void dispatchRecv();
            void handleRecv(std::size_t bytes, const error_code &error);

            void dispatchRecvFrom();
            void handleRecvFrom(shared_ptr<ip::udp::endpoint> endpoint, std::size_t bytes, const error_code &error);

            void startError(const error_code &error);

        protected:
            virtual void onConnect();
            virtual void onSend(const std::string &message);
            virtual void onSendTo(const std::string &ip, unsigned short port, const std::string &message);
            virtual void onRecv(std::vector<boost::uint8_t> &buffer);
            virtual void onRecvFrom(const std::string &ip, unsigned short port, std::vector<boost::uint8_t> &buffer);
            virtual void onError(const error_code &error);

        private:
            SocketUDP(const SocketUDP&);
            SocketUDP& operator=(const SocketUDP&);

            shared_ptr<NetService> netService_;
            shared_ptr<io_service::strand> strand_;
            shared_ptr<ip::udp::socket> socket_;

            std::list<std::string> sendBuffer_;
            std::vector<boost::uint8_t> recvBuffer_;

            std::size_t recvBufferSize_;
    };
}

#endif // SOCKETUDP_HPP
