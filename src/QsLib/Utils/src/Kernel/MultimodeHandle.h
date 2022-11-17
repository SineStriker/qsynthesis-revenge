#ifndef MULTIMODEHANDLE_H
#define MULTIMODEHANDLE_H

#include <QLocale>
#include <QObject>

#include "qsutils_global.h"
#include "qsutils_macros.h"

#define qMMH MultimodeHandle::instance()

#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings(qMMH->locale());                                                                 \
    connect(qMMH, &MultimodeHandle::localeChanged, this, &T::reloadStrings);

class QSUTILS_API MultimodeHandle : public QObject {
    Q_OBJECT
    Q_SINGLETON(MultimodeHandle)
public:
    explicit MultimodeHandle(QObject *parent = nullptr);
    ~MultimodeHandle();

public:
    enum Locale {
        China = QLocale::China,
        HongKong = QLocale::HongKong,
        Japan = QLocale::Japan,
        UnitedStates = QLocale::UnitedStates,
    };

    Locale locale() const;
    void setLocale(Locale loc);
    void refreshLocale();

protected:
    Locale m_loc;

signals:
    void localeChanged(int old);
};

#endif // MULTIMODEHANDLE_H
