#include "ActivityGauge.hpp"

namespace gui
{
    ActivityGauge::ActivityGauge(wxWindow *parent):
        wxGauge(parent, -1, 25, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL),
        state(0)
    {
    }

    void ActivityGauge::Animate(bool animate)
    {
        if (animate)
        {
            Start(50);
        }
        else
        {
            Stop();

            SetValue(0);
        }
    }

    void ActivityGauge::Notify()
    {
        state = (state + 1) % 26;

        SetValue(state);
    }
}
