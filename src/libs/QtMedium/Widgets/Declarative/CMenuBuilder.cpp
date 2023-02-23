#include "CMenuBuilder.h"
#include "private/CMenuBuilder_p.h"

#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>

#define SCC(X, Y) static const char X[] = Y

SCC(MENUDATA_ROOT_ELEMENT_NAME, "document");

SCC(MENUDATA_CONFIG_ELEMENT_NAME, "config");
SCC(MENUDATA_CONFIG_NAMESPACE_ELEMENT_NAME, "namespace");

SCC(MENUDATA_CONTENT_ELEMENT_NAME, "content");
SCC(MENUDATA_MAINMENU_ELEMENT_NAME, "mainMenu");
SCC(MENUDATA_TOOLBAR_ELEMENT_NAME, "toolBar");
SCC(MENUDATA_EDITORACTIONS_ELEMENT_NAME, "editorActions");

SCC(MENUDATA_MENU_ELEMENT_NAME, "menu");
SCC(MENUDATA_ACTION_ELEMENT_NAME, "action");
SCC(MENUDATA_SEPARATOR_ELEMENT_NAME, "separator");

SCC(MENUDATA_ID_ATTRIBUTE_NAME, "id");

CMenuBuilder::CMenuBuilder(QObject *parent) : CMenuBuilder(*new CMenuBuilderPrivate(), parent) {
}

CMenuBuilder::~CMenuBuilder() {
}

bool CMenuBuilder::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    file.close();
    return true;
}

bool CMenuBuilder::save(const QString &filename) {
    return true;
}

CMenuBuilder::CMenuBuilder(CMenuBuilderPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
