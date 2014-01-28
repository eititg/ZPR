#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include "../Util/Subject.hpp"

namespace game
{
    using boost::shared_ptr;

    class ManageConnection;

    /** Klasa manadżera gier. Odpowiada za przechowywanie listy gier i przekazywanie informacji o grach oraz
     *  zmianie stanu listy gier lub poszczególnych gier klasom interfesju użytkownika.
     */
    class GameManager : public util::Subject
    {
        public:
            /** Konstruktor.
             */
            GameManager();

            /** Wiąże manadżera gier z konkretnym połączeniem z serwerem.
             *  \param manageConnection wskaźnik do połączenia TCP z serwerem
             */
            void assignConnection(shared_ptr<game::ManageConnection> manageConnection);

            /** Dodaj grę do listy gier.
             *  \param players liczba graczy, którzy dołączyli do gry
             *  \param name nazwa gry
             */
            void addGame(unsigned short players, std::string name);
            /** Usuń grę o wybranej nazwie z listy.
             *  \param name nazwa wybranej gry
             */
            void removeGame(std::string name);

            /** Dołącz do wybranej gry.
             *  \param name nazwa wybranej gry
             */
            void joinGame(std::string name);
            /** Dołącz do gry, która została utworzona przez gracza.
             */
            void joinGame();
            /** Opuść grę.
             */
            void disjoinGame();

            /** Serwer potwierdził dołączenie do gry. Aktualizuje stan.
             */
            void joined();

            /** Na liście gier została wybrana jedna z gier.
             *  \param index indeks wybranej gry
             */
            void selectGame(unsigned int index);

            /** Serwer odpowiedział ramką ze szczegółami gry. Aktualizuj przechowywane informacje.
             *  \param namesVec wektor nazw graczy, którzy dołączyli do gry.
             */
            void setGameDetails(std::vector<std::string> namesVec);

            /** Zwróć nazwę gry, która została wybrana na liście gier.
             *  \return nazwa gry
             */
            std::string getSelectedName();
            /** Zwróć liczbę graczy którzy dołączyli do gry wybranej z listy gier.
             *  \return liczba graczy
             */
            unsigned short getSelectedPlayerCounter();
            /** Zwróć nazwę gracza o wybranym indeksie z listy graczy, którzy dołączyli do gry wybranej
             *  z listy gier.
             *  \return nazwa gracza
             */
            std::string getSelectedPlayerName(unsigned short index);

            /** Zwróć liczbę graczy, którzy dołączyli do gry o wybranym indeksie.
             *  \return liczba graczy
             */
            unsigned short getPlayerCount(unsigned int index) const;
            /** Zwróć nazwę gry o wybranym indeksie.
             *  \return nazwa gry
             */
            std::string getGameName(unsigned int index) const;

            /** Zwróć liczbę wszystkich gier przechowywanych przez menadżera gier.
             *  \return liczba gier
             */
            unsigned int getListSize() const;

            /** Sprawdź, czy wybrana gra została utworzona przez tego gracza.
             *  \return true jeśli gra została utworzona przez tego gracza lub false w przeciwnym wypadku
             */
            bool isNewGame() const;
            /** Sprawdź, czy gracz dołączył do jakiejś gry.
             *  \return true jeśli gracz dołączył do jakiejś gry lub false w przeciwnym wypadku
             */
            bool isJoined() const;

        private:
            /** Metoda porównująca dwie pary (liczba graczy, nazwa gry). Służy do alfabetycznego sortowania
             *  listy gier.
             *  \return true jeśli piewsza para powinna znaleźć się przed drugą lub false w przeciwnym wypadku
             */
            bool sortCompare(const std::pair<unsigned short, std::string> &first, const std::pair<unsigned short, std::string> &second);

            std::string gameName_;  /**< nazwa wybranej gry*/
            bool newGame_;  /**< czy wybrana gra jest utworzona przez tego gracza?*/
            std::vector<std::string> players_;  /**< wektor graczy, którzy dołączyli do wybranej gry*/

            bool joined_;   /**< czy gracz dołączył do jakiejś gry?*/

            std::vector<std::pair<unsigned short, std::string> > games_;    /**< wektor gier utworzonych na serwerze*/

            shared_ptr<game::ManageConnection> manageConnection_;   /**< wskaźnik do połączenia TCP z serwerem*/
    };
}

#endif // GAMEMANAGER_HPP
