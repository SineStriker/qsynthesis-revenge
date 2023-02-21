#include "CToolButton.h"

#include <QEvent>

CToolButton::CToolButton(QWidget *parent) : QToolButton(parent) {
#ifdef QS_NO_TAB_FOCUS
    QS_REMOVE_TAB_FOCUS(this)
#endif
    m_autoCheck = true;
}

CToolButton::~CToolButton() {
}

QSvgUri CToolButton::iconUp() const {
    return m_svgUris[0];
}

void CToolButton::setIconUp(const QSvgUri &iconUp) {
    m_svgUris[0] = iconUp;
    m_iconUp = iconUp.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconOver() const {
    return m_svgUris[1];
}

void CToolButton::setIconOver(const QSvgUri &iconOver) {
    m_svgUris[1] = iconOver;
    m_iconOver = iconOver.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconDown() const {
    return m_svgUris[2];
}

void CToolButton::setIconDown(const QSvgUri &iconDown) {
    m_svgUris[2] = iconDown;
    m_iconDown = iconDown.toIcon();
    reloadIcon();
    emit iconChanged();
}
QSvgUri CToolButton::iconUpChecked() const {
    return m_svgUris[3];
}

void CToolButton::setIconUpChecked(const QSvgUri &iconUpChecked) {
    m_svgUris[3] = iconUpChecked;
    m_iconUpChecked = iconUpChecked.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconOverChecked() const {
    return m_svgUris[4];
}

void CToolButton::setIconOverChecked(const QSvgUri &iconOverChecked) {
    m_svgUris[4] = iconOverChecked;
    m_iconOverChecked = iconOverChecked.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconDownChecked() const {
    return m_svgUris[5];
}

void CToolButton::setIconDownChecked(const QSvgUri &iconDownChecked) {
    m_svgUris[5] = iconDownChecked;
    m_iconDownChecked = iconDownChecked.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconDisabled() const {
    return m_svgUris[6];
}

void CToolButton::setIconDisabled(const QSvgUri &iconDisabled) {
    m_svgUris[6] = iconDisabled;
    m_iconDisabled = iconDisabled.toIcon(QSvgUri::Normal | QSvgUri::Disabled);
    reloadIcon();
    emit iconChanged();
}

bool CToolButton::autoCheck() const {
    return m_autoCheck;
}

void CToolButton::setAutoCheck(bool autoCheck) {
    m_autoCheck = autoCheck;
}

bool CToolButton::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::Enter:
    case QEvent::Leave:
    case QEvent::EnabledChange:
        reloadIcon();
        break;
    default:
        break;
    }
    return QToolButton::event(event);
}

void CToolButton::nextCheckState() {
    if (m_autoCheck) {
        QToolButton::nextCheckState();
        reloadIcon();
    }
}

void CToolButton::checkStateSet() {
    QToolButton::checkStateSet();
    reloadIcon();
}

void CToolButton::reloadIcon() {
    if (!isEnabled() && !m_iconDisabled.isNull()) {
        setIcon(m_iconDisabled);
        return;
    }
    if (isChecked()) {
        if (isDown() && !m_iconDownChecked.isNull()) {
            setIcon(m_iconDownChecked);
            return;
        } else if (underMouse() && !m_iconOverChecked.isNull()) {
            setIcon(m_iconOverChecked);
            return;
        } else if (!m_iconUpChecked.isNull()) {
            setIcon(m_iconUpChecked);
            return;
        }
    }
    if (isDown() && !m_iconDown.isNull()) {
        setIcon(m_iconDown);
        return;
    } else if (underMouse() && !m_iconOver.isNull()) {
        setIcon(m_iconOver);
        return;
    } else if (!m_iconUp.isNull()) {
        setIcon(m_iconUp);
        return;
    } else {
        setIcon(QIcon());
    }
}
