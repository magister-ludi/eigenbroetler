// -*- c++ -*-
#ifndef DATA_SELECTOR_INCLUDE
#define DATA_SELECTOR_INCLUDE

#include <global_defs.h>
#include <QComboBox>

class ArrayWindow;

class DataSelector: public QComboBox {
public:
    DataSelector(QWidget *p);
    static QString const& chooseFileText();
private:
    DataSelector(); // not implemented
    DataSelector(DataSelector const&); // not implemented
    DataSelector& operator=(DataSelector const&); // not implemented
    static const QString select_text;
};

inline QString const& DataSelector::chooseFileText()
{
    return select_text;
}

#endif /* DATA_SELECTOR_INCLUDE */
