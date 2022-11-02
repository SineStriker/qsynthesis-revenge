#include "LvEmptyTemplate.h"

LvEmptyTemplate::LvEmptyTemplate(QObject *parent) : ITemplateGenerator(parent) {
}

LvEmptyTemplate::~LvEmptyTemplate() {
}

QByteArray LvEmptyTemplate::descFile() const {
    return {};
}

bool LvEmptyTemplate::save(const QString &dir) {
    return true;
}
