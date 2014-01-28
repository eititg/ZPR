#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <boost/shared_ptr.hpp>
#include <wx/wx.h>
#include "GameManager.hpp"
#include "ManageConnection.hpp"
#include "../Util/Observer.hpp"

namespace gui
{
    using boost::shared_ptr;

    /** Klasa głównego okna aplikacji.
     */
    class MainWindow : public wxFrame, public util::Observer
    {
        public:
            /** Konstruktor.
             *  \param gameManager wskaźnik do menadżera gier
             *  \param manageConnection wskaźnik do połączenia TCP z serwerem
             */
            MainWindow(shared_ptr<game::GameManager> gameManager, shared_ptr<game::ManageConnection> manageConnection);
            /** Destruktor.
             */
            ~MainWindow();

            /** Metoda aktualizująca stan okna. Wywoływana przez klasę manageConnection
             */
            void update();

        private:
            /** Potrzebne identyfikatory kontrolek.
             */
            enum { ID_NEWGAME_BUTTON = wxID_HIGHEST + 1};

            /** Metoda obsługująca zdarzenie kliknięcia przycisku Nowa gra.
             *  \param event obiekt zdarzenia
             */
            void onNewGameButton(wxCommandEvent &event);

            wxButton *newGameButton;    /**< wskaźnik do przycisku Nowa gra*/

            shared_ptr<game::GameManager> gameManager_; /**< wskaźnik do menadżera gier*/
            shared_ptr<game::ManageConnection> manageConnection_;   /**<wskaźnik do połączenia TCP z serwerem*/
    };
}

#endif // MAINWINDOW_HPP
