#include "QMGuiDecoratorV2.h"
#include "private/QMGuiDecoratorV2_p.h"

QMGuiDecoratorV2Private::QMGuiDecoratorV2Private() {
}

QMGuiDecoratorV2Private::~QMGuiDecoratorV2Private() {
}

void QMGuiDecoratorV2Private::init() {
}

QMGuiDecoratorV2::QMGuiDecoratorV2(QObject *parent) : QMGuiDecoratorV2(*new QMGuiDecoratorV2Private(), parent) {
}

QMGuiDecoratorV2::~QMGuiDecoratorV2() {
}

QMGuiDecoratorV2::QMGuiDecoratorV2(QMGuiDecoratorV2Private &d, QObject *parent) : QMCoreDecoratorV2(d, parent) {
    d.init();
}