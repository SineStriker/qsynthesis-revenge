#ifndef CMENUBAR_H
#define CMENUBAR_H

#include <QMenuBar>

#include "QMWidgetsGlobal.h"
#include "QSvgIconEx.h"

class QToolButton;

// Since the QMenu class doesn't inherit font from QApplication
// This class is designed to change default menu bar's extension menu font

class CMenuBarPrivate;

class QMWIDGETS_EXPORT CMenuBar : public QMenuBar {
    Q_OBJECT
    Q_PROPERTY(QSvgIconEx extensionIcon READ extensionIcon WRITE setExtensionIcon)
public:
    explicit CMenuBar(QWidget *parent = nullptr);
    ~CMenuBar();

    QSvgIconEx extensionIcon() const;
    void setExtensionIcon(const QSvgIconEx &extensionIcon);

    QMenu *extensionMenu() const;

private:
    CMenuBarPrivate *d;
};

#endif // CMENUBAR_H
