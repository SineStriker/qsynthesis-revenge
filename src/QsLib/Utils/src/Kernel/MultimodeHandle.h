#ifndef MULTIMODEHANDLE_H
#define MULTIMODEHANDLE_H

#include <QLocale>
#include <QObject>

#include "qsutils_global.h"
#include "qsutils_macros.h"

#define qMMH MultimodeHandle::instance()

#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings(qMMH->locale());                                                                               \
    connect(qMMH, &MultimodeHandle::localeChanged, this, &T::reloadStrings);

class QSUTILS_API MultimodeHandle : public QObject {
    Q_OBJECT
    Q_SINGLETON(MultimodeHandle)
public:
    explicit MultimodeHandle(QObject *parent = nullptr);
    ~MultimodeHandle();

    QLocale::Country locale() const;
    void setLocale(QLocale::Country loc);
    void refreshLocale();

protected:
    QLocale::Country m_loc;

signals:
    void localeChanged(int old);
};

#endif // MULTIMODEHANDLE_H
