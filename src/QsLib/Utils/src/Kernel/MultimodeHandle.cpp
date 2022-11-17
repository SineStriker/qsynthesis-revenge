#include "MultimodeHandle.h"

#include <QCoreApplication>
#include <QDebug>

Q_SINGLETON_DECLARE(MultimodeHandle)

MultimodeHandle::MultimodeHandle(QObject *parent) : QObject(parent) {
    construct();

    // Determine system locale
    QLocale loc;
    switch (loc.country()) {
        case QLocale::China:
            m_loc = Locale::China;
            break;
        case QLocale::HongKong:
        case QLocale::Taiwan:
            m_loc = Locale::HongKong;
            break;
        case QLocale::Japan:
            m_loc = Locale::Japan;
            break;
        default:
            m_loc = Locale::UnitedStates;
            break;
    }
}

MultimodeHandle::~MultimodeHandle() {
}

MultimodeHandle::Locale MultimodeHandle::locale() const {
    return m_loc;
}

void MultimodeHandle::setLocale(Locale loc) {
    int old = m_loc;
    m_loc = loc;
    emit localeChanged(old);
}

void MultimodeHandle::refreshLocale() {
    emit localeChanged(m_loc);
}
