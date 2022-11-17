#include "LocalDecorator.h"

#include <QFile>

LocalDecorator::LocalDecorator(QObject *parent) : QObject(parent) {
}

LocalDecorator::~LocalDecorator() {
}

bool LocalDecorator::addTheme(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        stylesheetList.append(file.readAll());
        file.close();
        return true;
    }
    return false;
}

void LocalDecorator::removeThemes() {
    stylesheetList.clear();
}

QString LocalDecorator::stylesheets() const {
    return stylesheetList.join("\n\n");
}
