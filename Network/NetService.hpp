#ifndef NETSERVICE_HPP
#define NETSERVICE_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

namespace net
{
    using boost::shared_ptr;
    using namespace boost::asio;

    class AcceptorTCP;
    class SocketTCP;
    class SocketUDP;

    class NetService
    {
        friend class AcceptorTCP;
        friend class SocketTCP;
        friend class SocketUDP;

        public:
            NetService();
            virtual ~NetService();

            std::size_t poll();

        private:
            NetService(const NetService&);
            NetService& operator=(const NetService&);

            shared_ptr<io_service> io_service_;
    };
}

#endif // NETSERVICE_HPP
