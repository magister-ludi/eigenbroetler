#ifndef DISPLAY_INFO_INCLUDE
#define DISPLAY_INFO_INCLUDE

#include <global_defs.h>
#include <QColor>
#include <QMap>
#include <QVector>

class DisplayInfo: public QObject {
 public:
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
    static int const COLOURMAP_SIZE = 256;

    static DisplayInfo const& instance();
    //typedef QRgb ColourMap[COLOURMAP_SIZE];
    typedef QVector<QRgb> ColourMap;
    ColourMap getColourMap(QString const& pname) const;
    QString getDefaultColourMapName() const;
    QList<QString> getColourMapNames() const;
 private:
    DisplayInfo();
    DisplayInfo& operator=(DisplayInfo const&); // not implemented
    DisplayInfo(DisplayInfo const&); // not implemented
    void addColourMap(QString const& name, QRgb const *data);
    void initialise_colour_maps();

    static void delete_map();
    static DisplayInfo *display_info;
    QMap<QString, ColourMap> colour_maps;
    Q_OBJECT
};

inline DisplayInfo const& DisplayInfo::instance()
{
    if (!display_info) {
        display_info = new DisplayInfo;
        atexit(delete_map);
    }
    return *display_info;
}

inline QList<QString> DisplayInfo::getColourMapNames() const
{
    return colour_maps.keys();
}

inline QString DisplayInfo::getDefaultColourMapName() const
{
    return tr("Grey");
}

#endif /* DISPLAY_INFO_INCLUDE */
