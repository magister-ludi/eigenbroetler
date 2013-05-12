#ifndef SCALED_PLOTTER_INCLUDE
#define SCALED_PLOTTER_INCLUDE

#include <global_defs.h>
#include <plotter.h>

class ScaledPlotter: public Plotter {
 public:
    ScaledPlotter(int w, int h, ArrayWindow *listener = NULL);
 private:
    ScaledPlotter(); // not implemented
    ScaledPlotter(ScaledPlotter const&); // not implemented
    ScaledPlotter& operator=(ScaledPlotter const&); // not implemented
};

inline ScaledPlotter::ScaledPlotter(int w, int h, ArrayWindow *listener):
                     Plotter(w, h, listener)
{
}

#endif /* SCALED_PLOTTER_INCLUDE */
