#include "CActionListener.h"

template <class T>
static void _set_checked(T it, bool on) {
    const auto &obj = it.key();
    const auto &c = it.value();

    if (c.checkedStatus != CActionListener::ControlMode::NoControl) {
        obj->setChecked((c.checkedStatus == CActionListener::ControlMode::Reverse) ^ on);
    }
}

template <class T>
static void _set_enabled(T it, bool on) {
    const auto &obj = it.key();
    const auto &c = it.value();

    if (c.checkedStatus != CActionListener::ControlMode::NoControl) {
        obj->setEnabled((c.checkedStatus == CActionListener::ControlMode::Reverse) ^ on);
    }
}

CActionListener::CActionListener(QObject *parent) : QObject(parent) {
}

CActionListener::~CActionListener() {
}

void CActionListener::setChecked(bool checked) {
    for (auto it = actions.begin(); it != actions.end(); ++it) {
        _set_checked(it, checked);
    }
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        _set_checked(it, checked);
    }
    for (auto it = listeners.begin(); it != listeners.end(); ++it) {
        _set_checked(it, checked);
    }
}

void CActionListener::setEnabled(bool enabled) {
    for (auto it = actions.begin(); it != actions.end(); ++it) {
        _set_enabled(it, enabled);
    }
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        _set_enabled(it, enabled);
    }
    for (auto it = listeners.begin(); it != listeners.end(); ++it) {
        _set_enabled(it, enabled);
    }
}

void CActionListener::addAction(QAction *action, const CActionListener::Controller &c) {
    actions.insert(action, c);
}

void CActionListener::removeAction(QAction *action) {
    actions.remove(action);
}

void CActionListener::addButton(QAbstractButton *button, const CActionListener::Controller &c) {
    buttons.insert(button, c);
}

void CActionListener::removeButton(QAbstractButton *button) {
    buttons.remove(button);
}

void CActionListener::addListener(CActionListener *listener, const CActionListener::Controller &c) {
    listeners.insert(listener, c);
}

void CActionListener::removeListener(CActionListener *listener) {
    listeners.remove(listener);
}
