#include "MultimodeHandle.h"

#include <QCoreApplication>

Q_SINGLETON_DECLARE(MultimodeHandle)

MultimodeHandle::MultimodeHandle(QObject *parent) : QObject(parent) {
    construct();

    m_loc = QLocale::UnitedStates;
}

MultimodeHandle::~MultimodeHandle() {
}

QLocale::Country MultimodeHandle::locale() const {
    return m_loc;
}

void MultimodeHandle::setLocale(QLocale::Country loc) {
    int old = m_loc;
    m_loc = loc;
    emit localeChanged(old);
}

void MultimodeHandle::refreshLocale() {
    emit localeChanged(m_loc);
}
