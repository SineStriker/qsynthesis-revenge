#include "QMAppExtension.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

#include "QMConsole.h"
#include "QMDecoratorV2.h"
#include "private/QMAppExtension_p.h"

QMCoreConsole *QMInitFactory::createConsole(QObject *parent) {
    return new QMConsole(parent);
}

QMCoreDecoratorV2 *QMInitFactory::createDecorator(QObject *parent) {
    return new QMDecoratorV2(parent);
}

QMAppExtensionPrivate::QMAppExtensionPrivate() {
}

QMAppExtensionPrivate::~QMAppExtensionPrivate() {
}

void QMAppExtensionPrivate::init() {
    // Add theme paths
    for (const auto &path : qAsConst(themePaths))
        qIDec->addThemePath(path);
}

QMCoreInitFactory *QMAppExtensionPrivate::createFactory() {
    return new QMInitFactory();
}

QMAppExtension::QMAppExtension(QObject *parent) : QMAppExtension(*new QMAppExtensionPrivate(), parent) {
}

QMAppExtension::~QMAppExtension() {
}

QMAppExtension::QMAppExtension(QMAppExtensionPrivate &d, QObject *parent) : QMGuiAppExtension(d, parent) {
    d.init();
}