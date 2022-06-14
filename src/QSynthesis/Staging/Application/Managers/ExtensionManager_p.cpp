#include "ExtensionManager_p.h"

#include <QApplication>
#include <QStyle>
#include <QWidget>

ExtensionManagerPrivate::ExtensionManagerPrivate() {
}

ExtensionManagerPrivate::~ExtensionManagerPrivate() {
}

void ExtensionManagerPrivate::init() {
}

void ExtensionManagerPrivate::reloadAppFont() {
    QWidget w;
    qApp->style()->polish(&w);
    qApp->setFont(w.font());
}
