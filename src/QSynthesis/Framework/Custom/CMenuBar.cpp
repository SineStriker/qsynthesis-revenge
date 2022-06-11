#include "CMenuBar.h"
#include "CMenu.h"
#include "private/qmenubar_p.h"

#include <QApplication>
#include <QDebug>
#include <QToolButton>

#define EXTENSION_NAME "qt_menubar_ext_button"

CMenuBar::CMenuBar(QWidget *parent) : QMenuBar(parent) {
    setFont(qApp->font());

    m_extension = findChild<QToolButton *>(QLatin1String(EXTENSION_NAME));
    Q_ASSERT(m_extension);
    m_extension->setMenu(new CMenu());

#ifdef QS_NO_TAB_FOCUS
    QS_REMOVE_TAB_FOCUS(m_extension)
#endif

    m_extension->setMenu(new CMenu());
}

CMenuBar::~CMenuBar() {
}

QSvgUri CMenuBar::extensionIcon() const {
    return m_extensionIcon;
}

void CMenuBar::setExtensionIcon(const QSvgUri &extensionIcon) {
    m_extensionIcon = extensionIcon;
    m_extension->setIcon(m_extensionIcon.toIcon());
    emit extensionIconChanged();
}
