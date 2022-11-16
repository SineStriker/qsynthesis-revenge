#include "MultistyleHandle.h"

MultistyleHandle::MultistyleHandle(QObject *parent) : MultimodeHandle(parent) {
    m_theme = Light;
}

MultistyleHandle::~MultistyleHandle() {
}

MultistyleHandle::Theme MultistyleHandle::theme() const {
    return m_theme;
}

void MultistyleHandle::setTheme(Theme theme) {
    int org = m_theme;
    m_theme = theme;
    emit themeChanged(org);
}

void MultistyleHandle::refreshTheme() {
    emit themeChanged(m_theme);
}
