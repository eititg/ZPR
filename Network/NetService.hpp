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

    /** Klasa odpowiedzialna za obsługę sieciowych zadań asynchronicznych.
     */
    class NetService
    {
        friend class AcceptorTCP;
        friend class SocketTCP;
        friend class SocketUDP;

        public:
            /** Konstruktor.
             */
            NetService();
            /** Destruktor.
             */
            virtual ~NetService();

            /** Obsłuż wykonane zadania asynchroniczne. Metoda powinna być wywoływana cyklicznie.
             *  \return liczba obsłużonych zadań asynchronicznych
             */
            std::size_t poll();

        private:
            /** Konstruktor kopiujący.
             */
            NetService(const NetService&);
            /** Operator przypisania.
             */
            NetService& operator=(const NetService&);

            shared_ptr<io_service> io_service_; /**< wskaźnik na niskopoziomowy obiekt odpowiedzialny za obsługę zadań asynchronicznych*/
    };
}

#endif // NETSERVICE_HPP
