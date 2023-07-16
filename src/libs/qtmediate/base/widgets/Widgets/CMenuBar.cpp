#include "CMenuBar.h"
#include "CMenu.h"

#include "private/qmenubar_p.h"

#include <QDebug>
#include <QToolButton>

#define EXTENSION_NAME "qt_menubar_ext_button"

class CMenuBarPrivate {
public:
    QToolButton *m_extension;
};

CMenuBar::CMenuBar(QWidget *parent) : QMenuBar(parent), d(new CMenuBarPrivate()) {
    setFont(qApp->font());

    d->m_extension = findChild<QToolButton *>(QLatin1String(EXTENSION_NAME));
    Q_ASSERT(d->m_extension);
    d->m_extension->setMenu(new CMenu(this));
}

CMenuBar::~CMenuBar() {
    delete d;
}

QIcon CMenuBar::extensionIcon() const {
    return d->m_extension->icon();
}

void CMenuBar::setExtensionIcon(const QIcon &extensionIcon) {
    d->m_extension->setIcon(extensionIcon);
}

QMenu *CMenuBar::extensionMenu() const {
    return d->m_extension->menu();
}
