#include "BaseTab_p.h"

#include <QDebug>

BaseTabPrivate::BaseTabPrivate() {
}

BaseTabPrivate::~BaseTabPrivate() {
}

void BaseTabPrivate::init() {
    Q_Q(BaseTab);
    q->setAttribute(Qt::WA_StyledBackground);

    active = false;
}
