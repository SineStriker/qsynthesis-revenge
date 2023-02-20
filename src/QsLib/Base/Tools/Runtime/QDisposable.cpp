#include "QDisposable.h"

#include <QDebug>

QDisposable::QDisposable(QObject *parent) : QObject(parent), m_loaded(false) {
}

QDisposable::~QDisposable() {
}

void QDisposable::load() {
    if (m_loaded) {
        return;
    }
    qDebug() << "qdisposable: instance" << metaObject()->className() << "loading";
    loadImpl();
    m_loaded = true;
}

void QDisposable::save() {
    if (!m_loaded) {
        return;
    }
    saveImpl();
    qDebug() << "qdisposable: instance" << metaObject()->className() << "saved";
    m_loaded = false;
}

bool QDisposable::isLoaded() const {
    return m_loaded;
}

void QDisposable::loadImpl() {
}

void QDisposable::saveImpl() {
}
