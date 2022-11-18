#ifndef MULTISTYLEHANDLE_H
#define MULTISTYLEHANDLE_H

#include <QObject>

#include "Kernel/MultimodeHandle.h"

#include "qsframework_global.h"

#undef qMMH
#define qMMH qobject_cast<MultistyleHandle *>(MultimodeHandle::instance())

/*
 *  Initialize at constructor of CApplicationPrivate
 */

class QSFRAMEWORK_API MultistyleHandle : public MultimodeHandle {
    Q_OBJECT
public:
    explicit MultistyleHandle(QObject *parent = nullptr);
    ~MultistyleHandle();

public:
    enum Theme { Light, Dark, MultiColor };

    Theme theme() const;
    void setTheme(Theme theme);
    void refreshTheme();

protected:
    Theme m_theme;

signals:
    void themeChanged(int theme);
};

#define Q_SS_NOTIFY(T)                                                                             \
    reloadScreen(qMMH->theme());                                                                   \
    connect(qMMH, &MultistyleHandle::themeChanged, this, &T::reloadScreen);

#define Q_SS_NOTIFY_PRIVATE(T)                                                                     \
    q_func()->reloadScreen(qMMH->theme());                                                         \
    q_func()->connect(qMMH, &MultistyleHandle::themeChanged, q_func(), &T::reloadScreen);

#endif // MULTISTYLEHANDLE_H
