#ifndef CHORUSKIT_JSUTILS_H
#define CHORUSKIT_JSUTILS_H

#include <QWidget>

namespace ScriptMgr::Internal {
    QString jsMsgBox(QWidget *parent, const QString &title, const QString &message, const QString &icon, const QStringList &buttons, const QString &defaultButton);
}

#endif // CHORUSKIT_JSUTILS_H
