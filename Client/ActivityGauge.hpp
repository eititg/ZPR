#ifndef ACTIVITYGAUGE_HPP
#define ACTIVITYGAUGE_HPP

#include <wx/wx.h>

namespace gui
{
    /** Klasa animowanego paska postępu.
     *  Służy do pokazania użytkownikowi, że trwa jakaś operacja.
     */
    class ActivityGauge : public wxGauge, private wxTimer
    {
        public:
            /** Konstruktor tworzący pasek.
             *  \param parent wskaźnik na okno nadrzędne
             */
            ActivityGauge(wxWindow *parent);

            /** Czy pasek ma być animowany czy nie?
             *  \param animate nowy stan
             */
            void Animate(bool animate);

        private:
            /** Funkcja atkualizująca stan paska, wywoływna cyklicznie przez timer.
             */
            void Notify();

            unsigned state; /**< procent wypełnienia paska postępu */
    };
}

#endif // ACTIVITYGAUGE_HPP
