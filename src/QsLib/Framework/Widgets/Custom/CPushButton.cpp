#include "CPushButton.h"

#include <QDateTime>
#include <QPaintEvent>

CPushButton::CPushButton(QWidget *parent) : QPushButton(parent) {
    init();
}

CPushButton::CPushButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
    init();
}

CPushButton::CPushButton(const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent) {
    init();
}

CPushButton::~CPushButton() {
}

void CPushButton::init() {
#ifdef QS_NO_TAB_FOCUS
    QS_REMOVE_TAB_FOCUS(this)
#endif
    m_autoCheck = true;
}

QSvgUri CPushButton::iconUp() const {
    return m_svgUris[0];
}

void CPushButton::setIconUp(const QSvgUri &iconUp) {
    m_svgUris[0] = iconUp;
    m_iconUp = iconUp.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconOver() const {
    return m_svgUris[1];
}

void CPushButton::setIconOver(const QSvgUri &iconOver) {
    m_svgUris[1] = iconOver;
    m_iconOver = iconOver.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconDown() const {
    return m_svgUris[2];
}

void CPushButton::setIconDown(const QSvgUri &iconDown) {
    m_svgUris[2] = iconDown;
    m_iconDown = iconDown.toIcon();
    reloadIcon();
    emit iconChanged();
}
QSvgUri CPushButton::iconUpChecked() const {
    return m_svgUris[3];
}

void CPushButton::setIconUpChecked(const QSvgUri &iconUpChecked) {
    m_svgUris[3] = iconUpChecked;
    m_iconUpChecked = iconUpChecked.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconOverChecked() const {
    return m_svgUris[4];
}

void CPushButton::setIconOverChecked(const QSvgUri &iconOverChecked) {
    m_svgUris[4] = iconOverChecked;
    m_iconOverChecked = iconOverChecked.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconDownChecked() const {
    return m_svgUris[5];
}

void CPushButton::setIconDownChecked(const QSvgUri &iconDownChecked) {
    m_svgUris[5] = iconDownChecked;
    m_iconDownChecked = iconDownChecked.toIcon();
    reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconDisabled() const {
    return m_svgUris[6];
}

void CPushButton::setIconDisabled(const QSvgUri &iconDisabled) {
    m_svgUris[6] = iconDisabled;
    m_iconDisabled = iconDisabled.toIcon(QSvgUri::Normal | QSvgUri::Disabled);
    reloadIcon();
    emit iconChanged();
}

bool CPushButton::autoCheck() const {
    return m_autoCheck;
}

void CPushButton::setAutoCheck(bool autoCheck) {
    m_autoCheck = autoCheck;
}

bool CPushButton::event(QEvent *event) {
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
    return QPushButton::event(event);
}

void CPushButton::nextCheckState() {
    if (m_autoCheck) {
        QPushButton::nextCheckState();
        reloadIcon();
    }
}

void CPushButton::checkStateSet() {
    QPushButton::checkStateSet();
    reloadIcon();
}

void CPushButton::reloadIcon() {
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
