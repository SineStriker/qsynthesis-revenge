#ifndef PLAINWINDOW_H
#define PLAINWINDOW_H

#include <QMainWindow>

class PlainWindowPrivate;

#include "QsIntegrateGlobal.h"

class QSINTEGRATE_API PlainWindow : public QMainWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(PlainWindow)
public:
    explicit PlainWindow(QWidget *parent = nullptr);
    ~PlainWindow();

public:
    void setMenuBar(QMenuBar *menuBar);
    QMenuBar *menuBar() const;

protected:
    PlainWindow(PlainWindowPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<PlainWindowPrivate> d_ptr;
};

#endif // PLAINWINDOW_H
