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

    /** Klasa służąca do nasłuchiwania na porcie TCP na połączenia przychodzące.
     */
    class AcceptorTCP
    {
        public:
            /** Konstruktor.
             *  \param netService wskaźnik do obiektu obsługującego komunikację
             */
            AcceptorTCP(shared_ptr<NetService> netService);
            /** Destruktor.
             */
            virtual ~AcceptorTCP();

            /** Przypisz do portu.
             *  \param host interfejs sieciowy
             *  \param port numer portu
             */
            void bind(const std::string &host, unsigned short port);
            /** Rozpocznij nasłuchiwanie na połączenia.
             */
            void listen();

            /** Akceptuj połączenie.
             *  \param socketTCP wskaźnik do klasy gniazda, które będzie obsługiwało nowe połączenie
             */
            void accept(shared_ptr<SocketTCP> socketTCP);

        private:
            /** Rozpocznij asynchroniczne nasłuchiwanie.
             *  \param socketTCP wskaźnik do klasy gniazda, które będzie obsługiwało nowe połączenie
             */
            void dispatchAccept(shared_ptr<SocketTCP> socketTCP);
            /** Obsłuż zakończenie asynchronicznego nasłuchiwania.
             *  \param socketTCP gniazdo nowego połączenia
             *  \param error kod błędu
             */
            void handleAccept(shared_ptr<SocketTCP> socketTCP, const error_code &error);

        protected:
            /** Metoda wywoływana po zaakceptowaniu nowego połączenia. Może zostać przeciążona.
             *  \param socketTCP gniazdo nowego połączenia
             */
            virtual void onAccept(shared_ptr<SocketTCP> socketTCP);

        private:
            /** Konstruktor kopiujący
             */
            AcceptorTCP(const AcceptorTCP&);
            /** Operator przypisania.
             */
            AcceptorTCP& operator=(const AcceptorTCP&);

            shared_ptr<NetService> netService_; /**< wskaźnik do obiektu obsługującego komunikację*/
            shared_ptr<io_service::strand> strand_; /**< wskaźnik do obiektu pozwalającego na sekwencyjne wykonywanie zadań asynchronicznych*/
            shared_ptr<ip::tcp::acceptor> acceptor_;    /**< wskaźnik na niskopoziomowy obiekt accptora*/
    };
}

#endif // ACCEPTORTCP_HPP
