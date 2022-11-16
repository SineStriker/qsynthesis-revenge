#ifndef MULTISTYLEHANDLE_H
#define MULTISTYLEHANDLE_H

#include <QObject>

#include "Kernel/MultimodeHandle.h"

#include "qsframework_global.h"

#undef qMMH
#define qMMH qobject_cast<MultistyleHandle *>(MultimodeHandle::instance())

#define Q_SS_NOTIFY(T)                                                                             \
    reloadScreen(qMMH->theme());                                                                   \
    connect(qMMH, &MultistyleHandle::themeChanged, this, &T::reloadScreen);

/*
 *  Initialize at constructor of CApplicationPrivate
 */

class QSFRAMEWORK_API MultistyleHandle : public MultimodeHandle {
    Q_OBJECT
public:
    explicit MultistyleHandle(QObject *parent = nullptr);
    ~MultistyleHandle();

    enum Theme {
        Light,
        Dark,
        MultiColor,
    };

    Theme theme() const;
    void setTheme(Theme theme);
    void refreshTheme();

protected:
    Theme m_theme;

signals:
    void themeChanged(int theme);
};

#endif // MULTISTYLEHANDLE_H
