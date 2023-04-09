#ifndef ICOREBASE_H
#define ICOREBASE_H

#include <QObject>

#include "ActionSystem.h"
#include "DialogHelper.h"
#include "SettingCatalog.h"
#include "WindowSystem.h"

namespace Core {

    class ICoreBasePrivate;

    class CKAPPCORE_API ICoreBase : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ICoreBase)
    public:
        explicit ICoreBase(QObject *parent = nullptr);
        ~ICoreBase();

    public:
        ActionSystem *actionSystem() const;
        WindowSystem *windowSystem() const;
        DialogHelper *dialogHelper() const;
        SettingCatalog *settingCatalog() const;

    protected:
        ICoreBase(ICoreBasePrivate &d, QObject *parent = nullptr);

        QScopedPointer<ICoreBasePrivate> d_ptr;
    };

}

#endif // ICOREBASE_H