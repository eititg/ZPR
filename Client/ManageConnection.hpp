#ifndef MANAGECONNECTION_HPP
#define MANAGECONNECTION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/logic/tribool.hpp>
#include "GameManager.hpp"
#include "../Network/Network.hpp"
#include "../Util/Subject.hpp"

namespace game
{
    using boost::logic::tribool;
    using boost::shared_ptr;
    using boost::system::error_code;

    /** Klasa połączenia TCP z serwerem. Służy do przekazywania informacji związanych logowaniem, tworzeniem gier
     *  oraz dołączaniem i opuszczaniem już utworzonych gier. Jest to połączenie TCP.
     */
    class ManageConnection : public net::SocketTCP, public util::Subject
    {
        public:
            /** Konstruktor.
             *  \param netService główny obiekt obsługujący komunikację
             *  \param gameManager menadżer gier
             */
            ManageConnection(shared_ptr<net::NetService> netService, shared_ptr<game::GameManager> gameManager);
            /** Destruktor.
             */
            ~ManageConnection();

            /** Zaloguj się do serwera.
             *  \param login nazwa gracza
             */
            void logIn(const std::string &login);
            /** Wyloguj z serwera. Metoda wywoływana przy zamykaniu aplikacji.
             */
            void logOut();

            /** Wyślij żądanie pobrania listy gier z serwera.
             */
            void getGameList();
            /** Wyślij żądanie utworzenia nowej gry.
             *  \param name nazwa gry, która ma zostać utworzona
             */
            void newGame(const std::string &name);

            /** Wyślij żądanie dołączenia do gry.
             *  \param name nazwa gry do której gracz chce dołączyć
             */
            void joinGame(const std::string &name);
            /** Wyślij żądanie opuszczenia gry.
             *  \param name nazwa gry, którą gracz chce opuścić
             */
            void disjoinGame(const std::string &name);

            /** Wyślij żądanie pobrania szczegółowych informacji o grze.
             *  \param name nazwa gry, o której informacje mają zostać pobrane
             */
            void getGameDetails(const std::string &name);

            /** Sprawdza czy udało się połączyć z serwerem.
             *  \return true jeśli tak, false jeśli nie lub boost::logic::indeterminate jeśli nie było próby połączenia
             */
            tribool isConnected() const;
            /** Sprawdza czy udało się zalogować.
             *  \return true jeśli tak, false jeśli nie lub boost::logic::indeterminate jeśli nie było próby logowania
             */
            tribool isLogged() const;
            /** Sprawdza czy udało się utworzyć grę.
             *  \return true jeśli tak, false jeśli nie lub boost::logic::indeterminate jeśli nie było próby utowrzenia gry
             */
            tribool isGameCreated() const;

        private:
            /** Metoda wywoływana po udanym połączeniu z serwerem.
             */
            void onConnect();
            /** Metoda wywoływana po udanym wysłaniu pakietu danych.
             *  \parm buffer bufor z wysłanymi danymi
             */
            void onSend(const std::vector<boost::uint8_t> &buffer);
            /** Metoda wywoływana po udanym odebraniu pakietu danych.
             *  \param buffer bufor z odebranymi danymi
             */
            void onRecv(std::vector<boost::uint8_t> &buffer);

            /** Metoda wywoływana po wystąpieniu błędu z komunikacją.
             *  \param error kod błędu
             */
            void onError(const error_code &error);

            /** Metoda parsująca pakietu odebrane od serwera.
             */
            void parsePacket();

            std::vector<boost::uint8_t> buffer_;    /**< bufor przechowujący dane, które zostały odebrane od serwera, ale nie zostały jeszcze sparsowane*/

            tribool connected_; /**< czy próba połączania się udała?*/
            tribool logged_;    /**< czy próba logowania się udała?*/
            tribool gameCreated_;   /**< czy próba utworzenie nowej gry się udała?*/

            shared_ptr<game::GameManager> gameManager_; /**< wskaźnik do manadżera gier*/
    };
}

#endif // MANAGECONNECTION_HPP
