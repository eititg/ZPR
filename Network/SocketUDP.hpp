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

    /** Klasa gniazda UDP.
     */
    class SocketUDP
    {
        public:
            /** Konstruktor.
             *  \param netService wskaźnik do obiektu obsługującego komunikację asynchroniczną
             *  \param recvBufferSize rozmiar bufora na odebrane dane
             */
            SocketUDP(shared_ptr<NetService> netService, std::size_t recvBufferSize = 1024);
            /** Destruktor.
             */
            virtual ~SocketUDP();

            /** Przypisz gniazdo do interfejsu sieciowego i portu.
             *  \param host interfejs sieciowy
             *  \param port numer portu
             */
            void bind(const std::string &host, unsigned short port);

            /** Ustaw adres i port hosta, do którego będą wysyłane dane.
             *  \param host nazwa hosta zdalnego
             *  \param port numer portu hosta zdalnego
             */
            void connect(const std::string &host, unsigned short port);

            /** Wyślij pakiet danych.
             *  \param buffer bufor z danymi do wysłania
             */
            void send(std::vector<boost::uint8_t> &buffer);
            /** Wyślij pakiet danych.
             *  \param ip adres hosta zdalnego
             *  \param port numer portu hosta zdalnego
             *  \param buffer bufor z danymi do wysłania
             */
            void sendTo(const std::string &ip, unsigned short port, std::vector<boost::uint8_t> &buffer);

            /** Odbierz dane.
             */
            void recv();
            /** Odbierz dane z hosta zdalnego.
             */
            void recvFrom();

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
            /** Metoda obsługi asynchronicznego zadania połączenia (ustawienia adresu i numeru portu hosta zdalnego,
             *  do którego będą wysyłane dane).
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

            /** Rozpocznij asynchroniczne wysyłanie danych do hosta zdalnego.
             *  \param ip adres hosta zdalnego
             *  \param port numer portu hosta zdalnego
             *  \param it iterator wskazujący bufor danych do wysłania
             */
            void dispatchSendTo(const std::string &ip, unsigned short port, std::list<std::vector<boost::uint8_t> >::iterator it);
             /** Metoda obsługi asynchronicznego zadania wysyłania do hosta zdalnego.
             *  \param ip adres hosta zdalnego
             *  \param port numer portu hosta zdalnego
             *  \param it iterator wskazujący bufor wysłanych danych
             *  \param error kod błędu
             */
            void handleSendTo(const std::string &ip, unsigned short port, std::list<std::vector<boost::uint8_t> >::iterator it, const error_code &error);

            /** Rozpocznij asynchroniczne odbieranie danych.
             */
            void dispatchRecv();
            /** Metoda obsługi asynchronicznego zadania wysyłąnia.
             *  \param bytes liczba odebranych danych
             *  \param errro kod błędu
             */
            void handleRecv(std::size_t bytes, const error_code &error);

            /** Rozpocznij asynchroniczne odbieranie danych do hosta zdalnego.
             */
            void dispatchRecvFrom();
            /** Metoda obsługi asynchronicznego zadania wysyłąnia od hosta zdalnego.
             *  \param endpoint obiekt opisujący host zdalny
             *  \param bytes liczba odebranych danych
             *  \param errro kod błędu
             */
            void handleRecvFrom(shared_ptr<ip::udp::endpoint> endpoint, std::size_t bytes, const error_code &error);

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
            /** Metoda wywoływna po wysłaniu pakietu danych do hosta zdalnego.
             *  \param ip adres hosta zdalnego
             *  \param port numer portu hosta zdalnego
             *  \param buffer bufor z wysłanymi danymi
             */
            virtual void onSendTo(const std::string &ip, unsigned short port, const std::vector<boost::uint8_t> &buffer);
            /** Metoda wywoływana po odebraniu pakietu danych.
             *  \param buffer bufor z odebranymi danymi
             */
            virtual void onRecv(std::vector<boost::uint8_t> &buffer);
            /** Metoda wywoływana po odebraniu pakietu danych od hosta zdalnego.
             *  \param ip adres hosta zdalnego
             *  \param port numer portu hosta zdalnego
             *  \param buffer bufor z odebranymi danymi
             */
            virtual void onRecvFrom(const std::string &ip, unsigned short port, std::vector<boost::uint8_t> &buffer);

            /** Metoda wywoływana po wykryciu błędu komunikacji.
             *  \param error kod błędu
             */
            virtual void onError(const error_code &error);

        private:
            /** Konstruktor kopiujący.
             */
            SocketUDP(const SocketUDP&);
            /** Operator przypisania.
             */
            SocketUDP& operator=(const SocketUDP&);

            shared_ptr<NetService> netService_; /**< wskaźnik do obiektu obsługującego komunikację*/
            shared_ptr<io_service::strand> strand_; /**< wskaźnik do obiektu pozwalającego na sekwencyjne wykonywanie zadań asynchronicznych*/
            shared_ptr<ip::udp::socket> socket_;    /**< wskaźnik na niskopoziomowy obiekt gniazda*/

            std::list<std::vector<boost::uint8_t> > sendBuffer_;    /**< lista buforów wysłanych danych*/
            std::vector<boost::uint8_t> recvBuffer_;    /**< bufor na odebrane dane*/

            std::size_t recvBufferSize_;    /**< rozmiar bufora na odebrane dane*/
    };
}

#endif // SOCKETUDP_HPP
