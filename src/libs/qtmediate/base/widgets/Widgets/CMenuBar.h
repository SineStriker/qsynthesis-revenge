#ifndef CMENUBAR_H
#define CMENUBAR_H

#include <QMenuBar>

#include "QSvgUri.h"
#include "QMWidgetsGlobal.h"

class QToolButton;

// Since the QMenu class doesn't inherit font from QApplication
// This class is designed to change default menu bar's extension menu font
class QMWIDGETS_EXPORT CMenuBar : public QMenuBar {
    Q_OBJECT
    Q_PROPERTY(
        QSvgUri extensionIcon READ extensionIcon WRITE setExtensionIcon NOTIFY extensionIconChanged)
public:
    explicit CMenuBar(QWidget *parent = nullptr);
    ~CMenuBar();

    QSvgUri extensionIcon() const;
    void setExtensionIcon(const QSvgUri &extensionIcon);

    QMenu *extensionMenu() const;

private:
    QSvgUri m_extensionIcon;
    QToolButton *m_extension;

signals:
    void extensionIconChanged();
};

#endif // CMENUBAR_H
