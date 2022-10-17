#ifndef CMENUBAR_H
#define CMENUBAR_H

#include <QMenuBar>

#include "QSvgUri.h"
#include "qsframework_global.h"

class QToolButton;

// Since the QMenu class doesn't inherit font from QApplication
// This class is designed to change default menu bar's extension menu font
class QSFRAMEWORK_API CMenuBar : public QMenuBar {
    Q_OBJECT
    Q_PROPERTY(
        QSvgUri extensionIcon READ extensionIcon WRITE setExtensionIcon NOTIFY extensionIconChanged)
public:
    explicit CMenuBar(QWidget *parent = nullptr);
    ~CMenuBar();

    QSvgUri extensionIcon() const;
    void setExtensionIcon(const QSvgUri &extensionIcon);

private:
    QSvgUri m_extensionIcon;
    QToolButton *m_extension;

signals:
    void extensionIconChanged();
};

#endif // CMENUBAR_H
