
#include <array_window.h>
#include <QBitmap>
#include <QFileDialog>
#include <QImageWriter>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QScrollArea>
#include <QSettings>
#include <QTextStream>
#include <QVBoxLayout>

#include <array_window_2d.h>
#include <array_window_dialogs.h>
#include <complex_array.h>
#include <complex_operations.h>
#include <eigenbroetler_window.h>
#include <scaled_plotter.h>

uint ArrayWindow::anon_count = 0;
static QCursor *curs = NULL;

static void cleanup()
{
    delete curs;
}

ArrayWindow *ArrayWindow::createWindow(QList<ComplexArray *>& data,
                                       DisplayInfo::ComplexComponent c,
                                       DisplayInfo::Scale s,
                                       DisplayInfo::ColourMap const& p)
{
    ArrayWindow *w = new ArrayWindow2D(data, c, s, p);
    if (!w->dlist.isEmpty()) {
        return w;
    }
    else {
        delete w;
        return NULL;
    }
}

ArrayWindow::ArrayWindow(QList<ComplexArray *>& cdata,
                         DisplayInfo::ComplexComponent c,
                         DisplayInfo::Scale s):
    QWidget(),
    cmp(c),
    scl(s),
    index(0),
                   left_plot(NULL),
                   right_plot(NULL)
{
    setContentsMargins (0, 0, 0, 0);
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins (0, 0, 0, 0);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setContentsMargins (0, 0, 0, 0);
    verticalLayout->addLayout(horizontalLayout);

    QVBoxLayout *pholder = new QVBoxLayout;
    pholder->setAlignment(Qt::AlignTop | Qt::AlignRight);
    horizontalLayout->addLayout(pholder);
    colour_map_display = new Plotter(40, 256);
    pholder->addWidget(colour_map_display);

    QList<ComplexArray *>::iterator dt;
    QSize mx(0, 0);
    for (dt = cdata.begin(); dt != cdata.end(); ++dt) {
        DataSet *set = new DataSet;
        mx.setWidth(std::max(mx.width(), (*dt)->width()));
        mx.setHeight(std::max(mx.height(), (*dt)->height()));
        set->d = *dt;
        dlist << set;
    }
    plotLayout = new QWidget;
    left_plot = new ScaledPlotter(mx.width(), mx.height(), this);
    right_plot = new ScaledPlotter(mx.width(), mx.height(), this);
    left_plot->setParent(plotLayout);
    left_plot->move(0, 0);
    left_plot->setCursor(Qt::CrossCursor);
    right_plot->setParent(plotLayout);
    right_plot->move(left_plot->width(), index);
    right_plot->setCursor(Qt::CrossCursor);
    for(QList<DataSet *>::iterator set = dlist.begin(); set != dlist.end(); ++set) {
        ComplexArray *d0 = (*set)->d;
        if (mx.width() > d0->width() || mx.height() > d0->height()) {
            int dw = (mx.width() - d0->width());
            int l = dw / 2;
            int r = l;
            if (r + l != dw)
                r += 1;
            int dh = (mx.width() - d0->width());
            int t = dh / 2;
            int b = l;
            if (b + t != dh)
                b += 1;
            ComplexArray *pd = Operations::padCrop(d0, l, t, r, b);
            pd->setName(d0->source());
            delete d0;
            (*set)->d = pd;
        }
        alist << (*set)->d;
    }

    plotLayout->setFixedSize(left_plot->width() + right_plot->width(),
                             left_plot->height());
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(plotLayout);
    horizontalLayout->addWidget(scrollArea);
    scrollArea->setFrameStyle(QFrame::NoFrame);

    QPalette status_palette;
    QBrush brush(QColor(192, 255, 192, 255));
    brush.setStyle(Qt::SolidPattern);
    status_palette.setBrush(QPalette::Active, QPalette::Base, brush);

    status[0] = new QLineEdit;
    status[1] = new QLineEdit;
    // Reduce font size for satus info. Might be too small?
    QFont status_font = status[0]->font();
    status_font.setPointSize(status_font.pointSize() - 2);
    QHBoxLayout *statusLayout = new QHBoxLayout;
    verticalLayout->addLayout(statusLayout);
    for (int i = 0; i < 2; ++i) {
        statusLayout->addWidget(status[i]);
        status[i]->setText("");
        status[i]->setReadOnly(true);
        status[i]->setPalette(status_palette);
        status[i]->setFont(status_font);
        //status[i]->setPreferredWidth(256);
        //status[i]->setFrame(false);
    }
    //statusLayout->addStretch();
    updateTitle();

    if (curs == NULL) {
        curs = new QCursor(QBitmap(":/resources/curs_map.pbm"),
                           QBitmap(":/resources/curs_mask.pbm"));
        atexit(cleanup);
    }
    left_plot->setCursor(*curs);
    right_plot->setCursor(*curs);
    plotLayout->setCursor(*curs);
}

void ArrayWindow::updateTitle()
{
    QString title = dlist.at(index)->d->source();
    if (!title.isEmpty()) {
        int slash = title.lastIndexOf('/');
        if (slash >= 0)
            title_base = title.mid(slash + 1);
        else
            title_base = title;
    }
    else {
        if (title_base.isEmpty())
            title_base.sprintf("eigenbrot_%04u", ++anon_count);
    }
    if (dlist.length() > 1)
        title = QString(tr("%1 (%2 of %3)").arg(title_base).arg(index + 1).arg(dlist.length()));
    else
        title = title_base;
    setWindowTitle(title);
}

void ArrayWindow::closeEvent(QCloseEvent *evt)
{
    if (!dlist.at(index)->d->source().isEmpty())
        evt->accept();
    else {
        QSettings const settings(EigenbroetlerWindow::app_owner, EigenbroetlerWindow::app_name);
        bool ask = settings.value(CloseSubwindowDialog::ask_on_unsaved, true).toBool();
        if (ask) {
            CloseSubwindowDialog msgBox(this, windowTitle());
            if (msgBox.exec() != QDialog::Rejected) {
                if (msgBox.saveRequested()) {
                    if (saveData())
                        evt->accept();
                    else
                        evt->ignore();
                }
            }
            else
                evt->ignore();
        }
        else
            evt->accept();
    }
}

ArrayWindow::~ArrayWindow()
{
    QList<DataSet *>::iterator d;
    for (d = dlist.begin(); d != dlist.end(); ++d)
        delete *d;
}

void ArrayWindow::setColourMap(DisplayInfo::ColourMap const& p)
{
    if (pal != p) {
        pal = p;
        colour_map_display->painter.setBrush(QColor(192, 192, 192));
        colour_map_display->painter.drawRect(0, 0, colour_map_display->width(), colour_map_display->height());
        colour_map_display->painter.setPen(QPen(Qt::black));
        for (int i = 0; i < DisplayInfo::COLOURMAP_SIZE; i += 64)
            colour_map_display->painter.drawLine(0, i, colour_map_display->width(), i);
        for (int i = 0; i < DisplayInfo::COLOURMAP_SIZE; ++i) {
            colour_map_display->painter.setPen(QPen(pal[DisplayInfo::COLOURMAP_SIZE - i - 1]));
            colour_map_display->painter.drawLine(10, i, colour_map_display->width() - 10, i);
        }
        colour_map_display->repaint();
        redraw();
    }
}

bool ArrayWindow::saveData()
{
    QString fileTypes(tr("FITS Files (*.fits *.fit);;All files (*.*)"));
    QSettings settings(EigenbroetlerWindow::app_owner, EigenbroetlerWindow::app_name);
    QString dir = QFile::decodeName(settings.value(EigenbroetlerWindow::last_save, QString()).toString().toAscii());
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"),
                                                    dir, fileTypes, 0, QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty()) {
        fileName = fileName.toUtf8();
        int slash = fileName.lastIndexOf('/');
        settings.setValue(EigenbroetlerWindow::last_save, fileName.left(slash + 1));
        QFileInfo fi(fileName);
        QString ext = fi.suffix();
        QString base;
        if (ext.isEmpty()) {
            ext = ".fits"; // Force FITS if no extension
            base = fileName;
        }
        else {
            ext.prepend('.');
            base = fileName.left(fileName.length() - ext.length());
        }
        QList<DataSet *>::iterator set;
        for (set = dlist.begin(); set != dlist.end(); ++set) {
            QString count;
            if (dlist.length() > 1) {
                QTextStream str(&count);
                str << '_';
                str.setFieldWidth(2);
                str.setPadChar('0');
                str << (set - dlist.begin() + 1);
            }
            QString saveName = base + count + ext;
            if (!(*set)->d->save(saveName)) {
                QMessageBox::warning(this, QString(tr("File save failed: %1")).arg(saveName),
                                     (*set)->d->errorString());
                return false;
            }
        }
        title_base.clear(); // force recalculation of title
        updateTitle();
        return true;
    }
    return false;
}

void ArrayWindow::exportComponents()
{
    ExportDialog expt(this, cmp);
    if (expt.exec() == QDialog::Accepted && expt.result()) {
        QString lext = cmp == DisplayInfo::REAL ? "real" : "magn";
        QString rext = cmp == DisplayInfo::REAL ? "imag" : "phas";
        int cmp_result = expt.result();
        QList<QByteArray> formats = QImageWriter::supportedImageFormats();
        QList<QByteArray>::iterator fmt;

        QString fileTypes(tr("Bitmap Files ("));
        for (fmt = formats.begin(); fmt != formats.end(); ++fmt)
            fileTypes += QString(" *.") + *fmt;
        fileTypes += tr(";;All files (*.*)");
        QSettings settings(EigenbroetlerWindow::app_owner, EigenbroetlerWindow::app_name);
        QString dir = QFile::decodeName(settings.value(EigenbroetlerWindow::last_save,
                                                       QString()).toString().toAscii());
        QString fileName = QFileDialog::getSaveFileName(this, tr("Export file"),
                                                        dir, fileTypes, 0, QFileDialog::DontUseNativeDialog);
        if (!fileName.isEmpty()) {
            fileName = fileName.toUtf8();
            int slash = fileName.lastIndexOf('/');
            settings.setValue(EigenbroetlerWindow::last_save, fileName.left(slash + 1));
            QFileInfo fi(fileName);
            QString fmt_str = fi.suffix();
            QString ext(fmt_str);
            QString base;
            if (fmt_str.isEmpty()) {
                ext = fmt_str = "png"; // Force PNG if no extension
                ext.prepend('.');
                base = fileName;
            }
            else {
                ext.prepend('.');
                base = fileName.left(fileName.length() - ext.length());
            }
            QList<DataSet *>::const_iterator set;
            for (set = dlist.begin(); set != dlist.end(); ++set) {
                QString count;
                if (dlist.length() > 1) {
                    QTextStream str(&count);
                    str << '_';
                    str.setFieldWidth(2);
                    str.setPadChar('0');
                    str << (set - dlist.begin() + 1);
                }
                 if (cmp_result & 1) {
                     QImage const& im = (*set)->left;
                     im.save(base + "-" + lext + count + ext, fmt_str.toAscii().data());
                 }
                 if (cmp_result & 2) {
                     QImage const& im = (*set)->right;
                     im.save(base + "-" + rext + count + ext, fmt_str.toAscii().data());
                 }
                 if (cmp_result & 4) {
                     QImage combined(2 * (*set)->left.width(),
                                     (*set)->left.height(),
                                     QImage::Format_ARGB32);
                     QPainter p(&combined);
                     p.drawImage(0, 0, (*set)->left);
                     p.drawImage((*set)->left.width(), 0, (*set)->right);
                     combined.save(base + "-" + lext + "-" + rext + count + ext, fmt_str.toAscii().data());
                 }
            }
        }
    }
}

void ArrayWindow::setViewIndex(int idx, bool force)
{
    if ((force || idx != index) && idx >= 0 && idx < dlist.length()) {
        index = idx;
        left_plot->painter.drawImage(0, 0, dlist.at(index)->left);
        right_plot->painter.drawImage(0, 0, dlist.at(index)->right);
        left_plot->repaint();
        right_plot->repaint();
        updateTitle();
    }
}

void ArrayWindow::keyPressEvent(QKeyEvent * e)
{
    int const ch = e->key();
    int const len = dlist.length();
    switch (ch) {
    case Qt::Key_0:
        setViewIndex(9);
        e->accept();
        return;
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        setViewIndex(ch - Qt::Key_1);
        e->accept();
        return;
    case Qt::Key_Minus:
    case Qt::Key_Less:
        setViewIndex((len + index - 1) % len);
        e->accept();
        return;
    case Qt::Key_Plus:
    case Qt::Key_Greater:
    case Qt::Key_Space:
        setViewIndex((index + 1) % len);
        e->accept();
        return;
    }
    e->ignore();
}

