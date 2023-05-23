#include "IManager.h"
#include "IManager_p.h"

#include <coreplugin/ICore.h>

#include "IWizardFactory_p.h"

#include "Internal/Dialogs/ImportInitDialog.h"

namespace IEMgr {

    IManagerPrivate::IManagerPrivate() {
        running = false;
    }

    IManagerPrivate::~IManagerPrivate() {
    }

    void IManagerPrivate::init() {
    }

    static IManager *m_instance = nullptr;

    IManager *IManager::instance() {
        return m_instance;
    }

    bool IManager::addWizard(IWizardFactory *factory) {
        Q_D(IManager);
        if (!factory) {
            qWarning() << "IEMgr::IManager::addWizard(): trying to add null factory";
            return false;
        }
        if (d->importWizards.contains(factory->id())) {
            qWarning() << "IEMgr::IManager::addWizard(): trying to add duplicated factory:" << factory->id();
            return false;
        }
        factory->setParent(this);
        d->importWizards.append(factory->id(), factory);
        return true;
    }

    bool IManager::removeWizard(IWizardFactory *factory) {
        if (factory == nullptr) {
            qWarning() << "IEMgr::IManager::removeWizard(): trying to remove null factory";
            return false;
        }
        return removeWizard(factory->id());
    }

    bool IManager::removeWizard(const QString &id) {
        Q_D(IManager);
        auto it = d->importWizards.find(id);
        if (it == d->importWizards.end()) {
            qWarning() << "IEMgr::IManager::removeWizard(): factory does not exist:" << id;
            return false;
        }
        it.value()->setParent(nullptr);
        d->importWizards.erase(it);
        return true;
    }

    QList<IWizardFactory *> IManager::wizards() const {
        Q_D(const IManager);
        return d->importWizards.values();
    }

    void IManager::clearWizards() {
        Q_D(IManager);
        d->importWizards.clear();
    }

    void IManager::runImport(QWidget *parent, const QString &wizardId, const QString &path) {
        Q_D(IManager);

        auto iWin = Core::ICore::instance()->windowSystem()->findWindow(parent);
        if (!iWin || d->running) {
            return;
        }
        d->running = true;

        Internal::ImportInitDialog dlg(iWin->window());
        if (!wizardId.isEmpty()) {
            auto wizard = d->importWizards.value(wizardId, nullptr);
            if (wizard)
                dlg.selectWizard(wizard);
        }

        if (!path.isEmpty()) {
            dlg.setCurrentPath(path);
        }

        int code;
        do {
            code = dlg.exec();
        } while (code == QDialog::Accepted &&
                 !dlg.currentWizard()->runWizard(IWizardFactory::ImportProject, dlg.currentPath(), parent));

        d->running = false;
    }

    void IManager::runExport(QWidget *parent, const QString &wizardId, const QString &path) {
        Q_D(IManager);

        auto iWin = Core::ICore::instance()->windowSystem()->findWindow(parent);
        if (!iWin || d->running) {
            return;
        }
        d->running = true;

        d->running = false;
    }

    bool IManager::isRunning() const {
        Q_D(const IManager);
        return d->running;
    }

    IManager::IManager(QObject *parent) : IManager(*new IManagerPrivate(), parent) {
    }

    IManager::~IManager() {
        m_instance = nullptr;
    }

    IManager::IManager(IManagerPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        m_instance = this;
        d.q_ptr = this;

        d.init();
    }

}
