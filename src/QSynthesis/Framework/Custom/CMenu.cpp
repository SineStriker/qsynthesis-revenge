#include "CMenu.h"

#include <QApplication>
#include <QDebug>

CMenu::CMenu(QWidget *parent) : QMenu(parent) {
    init();
}

CMenu::CMenu(const QString &title, QWidget *parent) : QMenu(title, parent) {
    init();
}

CMenu::~CMenu() {
}

void CMenu::init() {
#ifdef QS_NO_TAB_FOCUS
    QS_REMOVE_TAB_FOCUS(this)
#endif
    setFont(qApp->font());
}
