#include "BaseTab.h"
#include "BaseTab_p.h"

BaseTab::BaseTab(QWidget *parent) : BaseTab(*new BaseTabPrivate(), parent) {
}

BaseTab::~BaseTab() {
}

void BaseTab::enter() {
    Q_D(BaseTab);
    d->active = true;
}

void BaseTab::leave() {
    Q_D(BaseTab);
    d->active = false;
}

QString BaseTab::title() const {
    Q_D(const BaseTab);
    return d->tabName;
}

QIcon BaseTab::icon() const {
    Q_D(const BaseTab);
    return d->icon;
}

void BaseTab::setIcon(const QIcon &icon) {
    Q_D(BaseTab);

    d->icon = icon;
    emit iconChanged(icon);
}

void BaseTab::setTabName(const QString &value) {
    Q_D(BaseTab);
    d->tabName = value;
    emit titleChanged(value);
}

void BaseTab::updateTabName() {
}

BaseTab::BaseTab(BaseTabPrivate &d, QWidget *parent) : QWidget(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}

bool BaseTab::active() const {
    Q_D(const BaseTab);
    return d->active;
}
