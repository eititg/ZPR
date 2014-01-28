#ifndef GAMELISTCTRL_HPP
#define GAMELISTCTRL_HPP

#include <boost/shared_ptr.hpp>
#include <wx/listctrl.h>
#include <wx/wx.h>
#include "GameManager.hpp"
#include "../Util/Observer.hpp"

namespace gui
{
    using boost::shared_ptr;

    /** Lista wyświetlająca gry utworzone na serwerze.
     */
    class GameListCtrl : public wxListCtrl, public util::Observer
    {
        public:
            /** Konstruktor tworzący listę.
             *  \param parent wskaźnik do okna nadrzędnego
             *  \param gameManager wskaźnik do menadżera gier
             */
            GameListCtrl(wxWindow *parent, shared_ptr<game::GameManager> gameManager);

            /** Metoda zwracająca tekst do wyświetlenia w wybranej kolumnie i wierszu listy. Wywoływana
             *  automatycznie, gdy zachodzi potrzeba przyrysowania listy.
             *  \param item indeks wiersza
             *  \param column indeks kolumny
             *  \return string, który powinien znaleźć się w wybranym miejscu
             */
            wxString OnGetItemText(long item, long column) const;

            /** Metoda aktualizująca stan listy. Wywoływana przez menadżera gier.
             */
            void update();

        private:
            /** Metoda obsługująca zdarzenie kliknięcia pozycji na liście.
             *  \param event obiekt zdarzenia
             */
            void onItemSelected(wxListEvent &event);

            shared_ptr<game::GameManager> gameManager_; /**< wskaźnik do menadżera gier*/
    };
}

#endif // GAMELISTCTRL_HPP
