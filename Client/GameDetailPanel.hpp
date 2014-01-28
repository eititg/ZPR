#ifndef GAMEDETAILPANEL_HPP
#define GAMEDETAILPANEL_HPP

#include <boost/shared_ptr.hpp>
#include <wx/listctrl.h>
#include <wx/wx.h>
#include "GameManager.hpp"
#include "../Util/Observer.hpp"

namespace gui
{
    using boost::shared_ptr;

    /** Klasa panelu wyswietlajacego szczegółowe dane o wybranej grze.
     */
    class GameDetailPanel : public wxPanel, public util::Observer
    {
        public:
            /** Konstruktor tworzący panel.
             *  \param parent wskaźnik do okna nadrzędnego
             *  \param gameManager wskaźnik do menadżera gier
             */
            GameDetailPanel(wxWindow *parent, shared_ptr<game::GameManager> gameManager);
            /** Destruktor.
             */
            ~GameDetailPanel();

            /** Metoda aktualizująca stan panelu. Wywoływana przez klasę menadżera gier.
             */
            void update();

        private:
            /** Potrzebne identyfikatory kontrolek panelu.
             */
            enum { ID_JOIN_BUTTON = wxID_HIGHEST + 1, ID_DISJOIN_BUTTON, ID_START_BUTTON };

            /** Metoda obsługująca zdarzenie kliknięcia przycisku Dołącz do gry.
             *  \param event obiekt zdarzenia
             */
            void onJoinButton(wxCommandEvent &event);
            /** Metoda obsługująca zdarzenie kliknięcia przycisku Opuść grę.
             *  \param event obiekt zdarzenia
             */
            void onDisjoinButton(wxCommandEvent &event);

            /** Metoda obsługująca zdarzenie kliknięcia przycisku Rozpocznij grę.
             *  \param event obiekt zdarzenia
             */
            void onStartButton(wxCommandEvent &event);

            wxTextCtrl *nameTextCtrl;   /**< wskaźnik do kontrolki wyświetlającej nazwę gry */

            wxListCtrl *playerList; /**< wskaźnik do kontrolki wyświetlającej listę graczy, którzy dołączyli do gry */

            wxButton *joinButton;   /**< wskaźnik do przycisku Dołącz do gry*/
            wxButton *disjoinButton;    /**< wskaźnik do przycisku Opuść grę*/
            wxButton *startButton;  /**< wskaźnik do przycisku Rozpocznij grę*/

            shared_ptr<game::GameManager> gameManager_; /**< wskaźnik do obiektu menadżera gier*/
    };
}

#endif // GAMEDETAILPANEL_HPP
