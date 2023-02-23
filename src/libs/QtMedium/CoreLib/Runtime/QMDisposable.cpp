#include "QMDisposable.h"

#include <QDebug>

QMDisposable::QMDisposable(QObject *parent) : QObject(parent), m_loaded(false) {
}

QMDisposable::~QMDisposable() {
}

void QMDisposable::load() {
    if (m_loaded) {
        return;
    }
    loadImpl();
    m_loaded = true;
}

void QMDisposable::save() {
    if (!m_loaded) {
        return;
    }
    saveImpl();
    m_loaded = false;
}

bool QMDisposable::isLoaded() const {
    return m_loaded;
}

void QMDisposable::loadImpl() {
}

void QMDisposable::saveImpl() {
}
