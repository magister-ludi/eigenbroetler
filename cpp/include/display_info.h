#ifndef DISPLAY_INFO_INCLUDE
#define DISPLAY_INFO_INCLUDE

#include <global_defs.h>
#include <QColor>
#include <QVector>

namespace DisplayInfo {
    enum ComplexComponent {
        REAL,
        IMAG,
        MAGN,
        PHSE,
    };
    enum Scale {
        LIN,
        LOG
    };
    int const PALETTE_SIZE = 256;
    //typedef QRgb Palette[PALETTE_SIZE];
    typedef QVector<QRgb> Palette;
    Palette createPalette();
}

inline DisplayInfo::Palette DisplayInfo::createPalette()
{
    return QVector<QRgb>(PALETTE_SIZE);
}

#endif /* DISPLAY_INFO_INCLUDE */
