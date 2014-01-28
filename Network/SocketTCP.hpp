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

    /** Klasa gniazda TCP.
     */
    class SocketTCP
    {
        friend AcceptorTCP;

        public:
            /** Konstruktor.
             *  \param netService wskaźnik do obiektu obsługującego komunikację asynchroniczną
             *  \param recvBufferSize rozmiar bufora na odebrane dane
             */
            SocketTCP(shared_ptr<NetService> netService, std::size_t recvBufferSize = 1024);
            /** Destruktor.
             */
            virtual ~SocketTCP();

            /** Połącz z hostem.
             *  \param host adres lub nazwa hosta
             *  \param port numer portu
             */
            void connect(const std::string &host, unsigned short port);

            /** Wyślij pakiet danych.
             *  \param buffer bufor z danymi do wysłania
             */
            void send(std::vector<boost::uint8_t> &buffer);
            /** Odbierz dane.
             *  \param bytes liczb bajtów które mają zostać odebrane (jeśli wynosi 0 to zostanie
             *  odebrana nieokreślona liczba bajtów)
             */
            void recv(std::size_t bytes = 0);

            /** Zamknij gniazdo.
             */
            void close();

            /** Zwróć rozmiar bufora na odebrane dane.
             *  \return rozmiar bufora
             */
            std::size_t getRecvBufferSize() const;
            /** Ustaw rozmiar bufora na odbrane dane.
             *  \param size nowy rozmiar bufora
             */
            void setRecvBufferSize(std::size_t size);

        private:
            /** Metoda obsługi asynchronicznego zadania połączenia.
             *  \param error kod błedu
             */
            void handleConnect(const error_code &error);

            /** Rozpocznij asynchroniczne wysyłanie danych.
             *  \param it iterator wskazujący bufor danych do wysłania
             */
            void dispatchSend(std::list<std::vector<boost::uint8_t> >::iterator it);
            /** Metoda obsługi asynchronicznego zadania wysyłania.
             *  \param it iterator wskazujący bufor wysłanych danych
             *  \param error kod błędu
             */
            void handleSend(std::list<std::vector<boost::uint8_t> >::iterator it, const error_code &error);

            /** Rozpocznij asynchroniczne odbieranie danych.
             *  \param bytes liczba bajtów do odebrania
             */
            void dispatchRecv(std::size_t bytes);
            /** Metoda obsługi asynchronicznego zadania wysyłąnia.
             *  \param bytes liczba odebranych danych
             *  \param errro kod błędu
             */
            void handleRecv(std::size_t bytes, const error_code &error);

            /** Metoda obsługi błędów.
             *  \param error kod błedu
             */
            void startError(const error_code &error);

        protected:
            /** Metoda wywoływana po nawiązaniu połączenia.
             */
            virtual void onConnect();
            /** Metoda wywoływna po wysłaniu pakietu danych.
             *  \param buffer bufor z wysłanymi danymi
             */
            virtual void onSend(const std::vector<boost::uint8_t> &buffer);
            /** Metoda wywoływana po odebraniu pakietu danych.
             *  \param buffer bufor z odebranymi danymi
             */
            virtual void onRecv(std::vector<boost::uint8_t> &buffer);

            /** Metoda wywoływana po wykryciu błędu komunikacji.
             *  \param error kod błędu
             */
            virtual void onError(const error_code &error);

        private:
            /** Konstruktor kopiujący.
             */
            SocketTCP(const SocketTCP&);
            /** Operator przypiania.
             */
            SocketTCP& operator=(const SocketTCP&);

            shared_ptr<NetService> netService_; /**< wskaźnik do obiektu obsługującego komunikację*/
            shared_ptr<io_service::strand> strand_; /**< wskaźnik do obiektu pozwalającego na sekwencyjne wykonywanie zadań asynchronicznych*/
            shared_ptr<ip::tcp::socket> socket_;    /**< wskaźnik na niskopoziomowy obiekt gniazda*/

            std::list<std::vector<boost::uint8_t> > sendBuffer_;    /**< lista buforów wysłanych danych*/
            std::vector<boost::uint8_t> recvBuffer_;    /**< bufor na odebrane dane*/

            std::size_t recvBufferSize_;    /**< rozmiar bufora na odebrane dane*/
    };
}

#endif // SOCKETTCP_HPP
