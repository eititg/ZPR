#ifndef NEWGAMEWINDOW_HPP
#define NEWGAMEWINDOW_HPP

#include <boost/shared_ptr.hpp>
#include <wx/wx.h>
#include "ManageConnection.hpp"
#include "../Util/Observer.hpp"

namespace gui
{
    using boost::shared_ptr;

    /** Klasa okna służącego do tworzenia nowej gry
     */
    class NewGameWindow : public wxFrame, public util::Observer
    {
        public:
            /** Konstruktor.
             *  \param gameManager wskaźnik do manadżera gier
             *  \param manageConnection wskaźnik do połączenia TCP z serwerem
             */
            NewGameWindow(shared_ptr<game::GameManager> gameManager, shared_ptr<game::ManageConnection> manageConnection);
            /** Destruktor.
             */
            ~NewGameWindow();

            /** Metoda aktualizująca stan okna. Wywoływana przez obiekt połączenia z serwerem.
             */
            void update();

        private:
            /** Metoda obsługująca zdarzenie zamknięcia okna.
             *  \param event obiekt zdarzenia
             */
            void onClose(wxCloseEvent &event);

            /** Metoda obsługująca zdarzenie kliknięcia przycisku OK.
             *  \param event obiekt zdarzenia
             */
            void onOkButton(wxCommandEvent &event);
            /** Metoda obsługująca zdarzenie kliknięcia przycisku Anuluj.
             *  \param event obiekt zdarzenia
             */
            void onCancelButton(wxCommandEvent &event);

            wxTextCtrl *nameTextCtrl;   /**< wskaźnik do kontrolki, do której użytkownik wpisuje nazwę gry*/
            wxButton *okButton; /**< wskaźnik do przycisku OK*/

            shared_ptr<game::GameManager> gameManager_; /**< wskaźnik do menadżera gier*/
            shared_ptr<game::ManageConnection> manageConnection_;   /**< wskaźnik do połączenia TCP z serwerem*/
    };
}

#endif // NEWGAMEWINDOW_HPP
