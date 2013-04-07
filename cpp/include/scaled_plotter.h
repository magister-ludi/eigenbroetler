#ifndef SCALED_PLOTTER_INCLUDE
#define SCALED_PLOTTER_INCLUDE

#include <plotter.h>

class ScaledPlotter: public Plotter {
 public:
    ScaledPlotter(int w, int h);
 private:
    ScaledPlotter(); // not implemented
    ScaledPlotter(ScaledPlotter const&); // not implemented
    ScaledPlotter& operator=(ScaledPlotter const&); // not implemented
};

inline ScaledPlotter::ScaledPlotter(int w, int h):
                     Plotter(w, h)
{
}

#endif /* SCALED_PLOTTER_INCLUDE */
