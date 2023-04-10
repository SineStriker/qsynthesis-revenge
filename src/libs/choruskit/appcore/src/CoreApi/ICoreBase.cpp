#include "ICoreBase.h"
#include "ICoreBase_p.h"

#include <QApplication>

namespace Core {

    ICoreBasePrivate::ICoreBasePrivate() {
    }

    ICoreBasePrivate::~ICoreBasePrivate() {
    }

    void ICoreBasePrivate::init() {
        Q_Q(ICoreBase);

        actionSystem = new ActionSystem(q);
        windowSystem = new WindowSystem(q);
        settingCatalog = new SettingCatalog(q);
    }

    ICoreBase::ICoreBase(QObject *parent) : ICoreBase(*new ICoreBasePrivate(), parent) {
    }

    ICoreBase::~ICoreBase() {
    }
    
    ActionSystem *ICoreBase::actionSystem() const {
        Q_D(const ICoreBase);
        return d->actionSystem;
    }

    WindowSystem *ICoreBase::windowSystem() const {
        Q_D(const ICoreBase);
        return d->windowSystem;
    }

    SettingCatalog *ICoreBase::settingCatalog() const {
        Q_D(const ICoreBase);
        return d->settingCatalog;
    }

    ICoreBase::ICoreBase(ICoreBasePrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}