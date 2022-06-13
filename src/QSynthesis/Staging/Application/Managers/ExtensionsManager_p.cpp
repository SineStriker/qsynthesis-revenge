#include "ExtensionsManager_p.h"

#include <QApplication>
#include <QStyle>
#include <QWidget>

ExtensionsManagerPrivate::ExtensionsManagerPrivate() {
}

ExtensionsManagerPrivate::~ExtensionsManagerPrivate() {
}

void ExtensionsManagerPrivate::init() {
}

void ExtensionsManagerPrivate::reloadAppFont() {
    QWidget w;
    qApp->style()->polish(&w);
    qApp->setFont(w.font());
}
