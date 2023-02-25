#include "LvEmptyTemplate.h"

LvEmptyTemplate::LvEmptyTemplate(QObject *parent) : ILvProjTemplate(parent) {
}

LvEmptyTemplate::~LvEmptyTemplate() {
}

QByteArray LvEmptyTemplate::descFile() const {
    return {};
}

bool LvEmptyTemplate::save(const QString &dir) {
    return true;
}
