#ifndef CONNECTWINDOW_HPP
#define CONNECTWINDOW_HPP

#include <boost/shared_ptr.hpp>
#include <wx/wx.h>
#include "ManageConnection.hpp"
#include "../Util/Observer.hpp"

namespace gui
{
    using boost::shared_ptr;

    class ActivityGauge;

    /** Klasa okna służącego do nawiązywania połączenia z serwerem. Wymaga od użytkownika podania adresu
     *  IP i portu TCP wybranego serwera.
     */
    class ConnectWindow : public wxFrame, public util::Observer
    {
        public:
            /** Konstruktor tworzący okno.
             *  \param manageConnection wskaźnik do klas połączenia z TCP z serwerem
             */
            ConnectWindow(shared_ptr<game::ManageConnection> manageConnection);
            /** Desktruktor
             */
            ~ConnectWindow();

            /** Metoda aktualizująca stan okna. Wywoływana przez klasę manageConnection
             */
            void update();

        private:
            /** Metoda obsługi zdarzenia zamykania okna.
             * \param event obiekt zdarzenia
             */
            void onClose(wxCloseEvent &event);

            /** Metoda obsługi zdarzenia kliknięcia przycisku OK.
             * \param event obiekt zdarzenia
             */
            void onOkButton(wxCommandEvent &event);
            /** Metoda obłsugi kliknięcia przycisku Anuluj.
             * \param event obiekt zdarzenia
             */
            void onCancelButton(wxCommandEvent &event);

            wxTextCtrl *serverTextCtrl; /**< wskaźnik do kontrolki do wpisania adresu i portu */
            gui::ActivityGauge *gauge;  /**< wskaźnik do animowanego paska postępu */
            wxButton *okButton; /**< wskaźnik do przycisku OK */

            shared_ptr<game::ManageConnection> manageConnection_;   /**< wskaźnik do połączenia TCP z serwerem */
    };
}


#endif // CONNECTWINDOW_HPP
