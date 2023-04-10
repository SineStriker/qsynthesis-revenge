#include "CPushButton.h"

#include <QDateTime>
#include <QPaintEvent>

class CPushButtonPrivate {
    Q_DECLARE_PUBLIC(CPushButton)
public:
    CPushButton *q_ptr;

    QSvgUri m_svgUris[7];

    QIcon m_iconUp;
    QIcon m_iconOver;
    QIcon m_iconDown;
    QIcon m_iconUpChecked;
    QIcon m_iconOverChecked;
    QIcon m_iconDownChecked;
    QIcon m_iconDisabled;

    bool m_autoCheck;

    void init() {
#ifdef QS_NO_TAB_FOCUS
        QS_REMOVE_TAB_FOCUS(q_ptr)
#endif
        m_autoCheck = true;
    }

    void reloadIcon() {
        Q_Q(CPushButton);
        if (!q->isEnabled() && !m_iconDisabled.isNull()) {
            q->setIcon(m_iconDisabled);
            return;
        }
        if (q->isChecked()) {
            if (q->isDown() && !m_iconDownChecked.isNull()) {
                q->setIcon(m_iconDownChecked);
                return;
            } else if (q->underMouse() && !m_iconOverChecked.isNull()) {
                q->setIcon(m_iconOverChecked);
                return;
            } else if (!m_iconUpChecked.isNull()) {
                q->setIcon(m_iconUpChecked);
                return;
            }
        }
        if (q->isDown() && !m_iconDown.isNull()) {
            q->setIcon(m_iconDown);
            return;
        } else if (q->underMouse() && !m_iconOver.isNull()) {
            q->setIcon(m_iconOver);
            return;
        } else if (!m_iconUp.isNull()) {
            q->setIcon(m_iconUp);
            return;
        } else {
            q->setIcon(QIcon());
        }
    }
};

CPushButton::CPushButton(QWidget *parent) : CPushButton(*new CPushButtonPrivate(), parent) {
}

CPushButton::CPushButton(const QString &text, QWidget *parent) : CPushButton(parent) {
    setText(text);
}

CPushButton::CPushButton(const QIcon &icon, const QString &text, QWidget *parent) : CPushButton(parent) {
    setIcon(icon);
    setText(text);
}

CPushButton::~CPushButton() {
}

QSvgUri CPushButton::iconUp() const {
    Q_D(const CPushButton);
    return d->m_svgUris[0];
}

void CPushButton::setIconUp(const QSvgUri &iconUp) {
    Q_D(CPushButton);

    d->m_svgUris[0] = iconUp;
    d->m_iconUp = iconUp.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconOver() const {
    Q_D(const CPushButton);
    return d->m_svgUris[1];
}

void CPushButton::setIconOver(const QSvgUri &iconOver) {
    Q_D(CPushButton);

    d->m_svgUris[1] = iconOver;
    d->m_iconOver = iconOver.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconDown() const {
    Q_D(const CPushButton);
    return d->m_svgUris[2];
}

void CPushButton::setIconDown(const QSvgUri &iconDown) {
    Q_D(CPushButton);

    d->m_svgUris[2] = iconDown;
    d->m_iconDown = iconDown.toIcon();
    d->reloadIcon();
    emit iconChanged();
}
QSvgUri CPushButton::iconUpChecked() const {
    Q_D(const CPushButton);
    return d->m_svgUris[3];
}

void CPushButton::setIconUpChecked(const QSvgUri &iconUpChecked) {
    Q_D(CPushButton);

    d->m_svgUris[3] = iconUpChecked;
    d->m_iconUpChecked = iconUpChecked.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconOverChecked() const {
    Q_D(const CPushButton);
    return d->m_svgUris[4];
}

void CPushButton::setIconOverChecked(const QSvgUri &iconOverChecked) {
    Q_D(CPushButton);

    d->m_svgUris[4] = iconOverChecked;
    d->m_iconOverChecked = iconOverChecked.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconDownChecked() const {
    Q_D(const CPushButton);
    return d->m_svgUris[5];
}

void CPushButton::setIconDownChecked(const QSvgUri &iconDownChecked) {
    Q_D(CPushButton);

    d->m_svgUris[5] = iconDownChecked;
    d->m_iconDownChecked = iconDownChecked.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CPushButton::iconDisabled() const {
    Q_D(const CPushButton);
    return d->m_svgUris[6];
}

void CPushButton::setIconDisabled(const QSvgUri &iconDisabled) {
    Q_D(CPushButton);

    d->m_svgUris[6] = iconDisabled;
    d->m_iconDisabled = iconDisabled.toIcon(QSvgUri::Normal | QSvgUri::Disabled);
    d->reloadIcon();
    emit iconChanged();
}

bool CPushButton::autoCheck() const {
    Q_D(const CPushButton);
    return d->m_autoCheck;
}

void CPushButton::setAutoCheck(bool autoCheck) {
    Q_D(CPushButton);

    d->m_autoCheck = autoCheck;
}

CPushButton::CPushButton(CPushButtonPrivate &d, QWidget *parent) : QPushButton(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}

bool CPushButton::event(QEvent *event) {
    Q_D(CPushButton);
    switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::EnabledChange:
            d->reloadIcon();
            break;
        default:
            break;
    }
    return QPushButton::event(event);
}

void CPushButton::nextCheckState() {
    Q_D(CPushButton);
    if (d->m_autoCheck) {
        QPushButton::nextCheckState();
        d->reloadIcon();
    }
}

void CPushButton::checkStateSet() {
    Q_D(CPushButton);

    QPushButton::checkStateSet();
    d->reloadIcon();
}