#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <boost/shared_ptr.hpp>
#include <wx/wx.h>
#include "GameManager.hpp"
#include "ManageConnection.hpp"

/** Główna klasa aplikacji.
 */
class Application : public wxApp
{
    public:
        /** Konstruktor domyślny.
         */
        Application();

        /** Metoda inicjalizująca aplikację.
         *  \return true jeśli ma zostać uruchomiona pętla komunikatów i aplikacja ma działać dalej lub false,
         *  gdy aplikacja ma się zakończyć
         */
        virtual bool OnInit();

    private:
        /** Metoda obsługująca zdarzenie bezczynności. Odpowiada za obsługę komunikacji przez sieć
         *  \param event obiekt zdarzenia
         */
        void onIdle(wxIdleEvent &event);
        /** Metoda obsługująca timer. Jest wywoływana cyklicznie i odpowiada za obsługę komunikacji
         *  przez sieć.
         *  \param event obiekt zdarzenia
         */
        void onTimer(wxTimerEvent &event);

        boost::shared_ptr<wxTimer> networkTimer_;   /**< wskaźnik do timera wywołującego zdarzenie onTimer */

        boost::shared_ptr<net::NetService> netService_; /**< wskaźnik do głównej klasy obsługującej sieć */

        boost::shared_ptr<game::GameManager> gameManager_;  /**< wskaźnik do klasy zarządzającej listą gier */
        boost::shared_ptr<game::ManageConnection> manageConnection_;    /**< wskaźnik do klasy połączenia TCP z serwerem */
};

#endif // APPLICATION_HPP
