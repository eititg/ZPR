#ifndef LOGINWINDOW_HPP
#define LOGINWINDOW_HPP

#include <wx/wx.h>
#include "ManageConnection.hpp"
#include "../Util/Observer.hpp"

namespace gui
{
    using boost::shared_ptr;

    /** Klasa okna logowania. Wymaga od użytkownika podania nazwy gracza.
     */
    class LoginWindow : public wxFrame, public util::Observer
    {
        public:
            /** Konsturktor.
             *  \param manageConnection wskaźnik do połączenia TCP z serwerem
             */
            LoginWindow(shared_ptr<game::ManageConnection> manageConnection);
            /** Destruktor.
             */
            ~LoginWindow();

            /** Metoda aktualizująca stan okna. Wywoływana przez obiekt połączenia z serwerem.
             */
            void update();

        private:
            /** Metoda obsługująca zdzarzenie zamknięcia okna.
             *  \param event obiekt zdarzenia
             */
            void onClose(wxCloseEvent& event);

            /** Metoda obsługująca zdarzenie kliknięcia przycisku OK.
             *  \param event obiekt zdarzenia
             */
            void onOkButton(wxCommandEvent &event);
            /** Metoda obsługująca zdarzenie kliknięcia przycisku Anuluj.
             *  \param event obiekt zdarzenia
             */
            void onCancelButton(wxCommandEvent &event);

            wxTextCtrl *loginTextCtrl;  /**< wskaźnik do kontrolki do której użytkownik wpisuje nazwę*/
            wxButton *okButton; /**< wskaźnik do przycisku OK*/

            shared_ptr<game::ManageConnection> manageConnection_;   /**< wskaźnik do połączenia TCP z serwerem*/    };
}

#endif // LOGINWINDOW_HPP
