#ifndef QMGUIAPPEXTENSION_H
#define QMGUIAPPEXTENSION_H

#include <QMCoreAppExtension.h>

#include "QSvgIconEx.h"

#ifdef qAppExt
#    undef qAppExt
#endif
#define qAppExt qobject_cast<QMGuiAppExtension *>(QMCoreAppExtension::instance())

class QMGuiAppExtensionPrivate;

class QMGUI_EXPORT QMGuiAppExtension : public QMCoreAppExtension {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMGuiAppExtension)
public:
    explicit QMGuiAppExtension(QObject *parent = nullptr);
    ~QMGuiAppExtension();

public:
    enum SvgIcon {
        SI_MenuIndicator,
        SI_MenuRightArrow,
        SI_MenuBarExtension,
    };

    QSvgIconEx svgIcon(SvgIcon icon) const;
    void setSvgIcon(SvgIcon icon, const QSvgIconEx &uri);

protected:
    QMGuiAppExtension(QMGuiAppExtensionPrivate &d, QObject *parent = nullptr);
};

#endif // QMGUIAPPEXTENSION_H