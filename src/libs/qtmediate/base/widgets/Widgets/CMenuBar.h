#ifndef CMENUBAR_H
#define CMENUBAR_H

#include <QMenuBar>

#include "QMWidgetsGlobal.h"

class QToolButton;

// Since the QMenu class doesn't inherit font from QApplication
// This class is designed to change default menu bar's extension menu font

class CMenuBarPrivate;

class QMWIDGETS_EXPORT CMenuBar : public QMenuBar {
    Q_OBJECT
    Q_PROPERTY(QIcon extensionIcon READ extensionIcon WRITE setExtensionIcon)
public:
    explicit CMenuBar(QWidget *parent = nullptr);
    ~CMenuBar();

public:
    QIcon extensionIcon() const;
    void setExtensionIcon(const QIcon &extensionIcon);

    QMenu *extensionMenu() const;

private:
    CMenuBarPrivate *d;
};

#endif // CMENUBAR_H
