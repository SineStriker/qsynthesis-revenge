#ifndef QMDECORATEDIR_H
#define QMDECORATEDIR_H

#include "QMLocaleDir.h"
#include "QMWidgetsGlobal.h"

class QMDecorateDirPrivate;

class QMWIDGETS_API QMDecorateDir : public QMLocaleDir {
    Q_DECLARE_PRIVATE(QMDecorateDir)
public:
    QMDecorateDir();
    ~QMDecorateDir();

protected:
    QMDecorateDir(QMDecorateDirPrivate &d);
};

#endif // QMDECORATEDIR_H
