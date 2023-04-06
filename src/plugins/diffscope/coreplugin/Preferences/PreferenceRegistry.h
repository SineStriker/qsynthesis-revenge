//
// Created by Crs_1 on 2023/4/5.
//

#ifndef CHORUSKIT_PREFERENCEREGISTRY_H
#define CHORUSKIT_PREFERENCEREGISTRY_H

#include "CoreApi/IDialogPage.h"
#include "IPreferenceItem.h"
#include <QObject>
#include <set>
namespace Core {

    class PreferenceRegistryPrivate;

    class PreferenceRegistry: public QObject {
        Q_OBJECT
    public:
        explicit PreferenceRegistry(QObject *parent);
        ~PreferenceRegistry();
        void addItem(IPreferenceItem *node, const QString &parentId = "");
        IPreferenceItem *getItem(const QString &id);

        void showPreferenceDialog(QWidget *parent, const QString &entryPageId);
        void closePreferenceDialog(int result);

    protected:
        QScopedPointer<PreferenceRegistryPrivate> d_ptr;
        Q_DECLARE_PRIVATE(PreferenceRegistry)
        PreferenceRegistry(PreferenceRegistryPrivate *d, QObject *parent);

        void removeItem(const QString &id); //TODO
    };
}


#endif // CHORUSKIT_PREFERENCEREGISTRY_H
