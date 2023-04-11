#include "IWizardFactory.h"
#include "IWizardFactory_p.h"

#include <QMSystem.h>

namespace IEMgr {

    IWizardFactoryPrivate::IWizardFactoryPrivate() {
    }

    IWizardFactoryPrivate::~IWizardFactoryPrivate() {
    }

    void IWizardFactoryPrivate::init() {
    }

    IWizardFactory::IWizardFactory(const QString &id, QObject *parent)
        : IWizardFactory(*new IWizardFactoryPrivate(), id, parent) {
    }

    IWizardFactory::~IWizardFactory() {
    }

    IWizardFactory::IWizardFactory(IWizardFactoryPrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

    QString IWizardFactory::id() const {
        Q_D(const IWizardFactory);
        return d->id;
    }

    QString IWizardFactory::filter(Feature feature) const {
        Q_UNUSED(feature);
        return QString("%1(%2)").arg(tr("All Files"), QMOs::allFilesFilter());
    }

    QString IWizardFactory::iconPath() const {
        Q_D(const IWizardFactory);
        return d->iconPath;
    }

    void IWizardFactory::setIconPath(const QString &path) {
        Q_D(IWizardFactory);
        d->iconPath = path;
    }

    QString IWizardFactory::description() const {
        Q_D(const IWizardFactory);
        return d->description;
    }

    void IWizardFactory::setDescription(const QString &description) {
        Q_D(IWizardFactory);
        d->description = description;
    }

    QString IWizardFactory::displayName() const {
        Q_D(const IWizardFactory);
        return d->displayName;
    }

    void IWizardFactory::setDisplayName(const QString &displayName) {
        Q_D(IWizardFactory);
        d->displayName = displayName;
    }

    QString IWizardFactory::category() const {
        Q_D(const IWizardFactory);
        return d->category;
    }

    void IWizardFactory::setCategory(const QString &category) {
        Q_D(IWizardFactory);
        d->category = category;
    }

    QString IWizardFactory::displayCategory() const {
        Q_D(const IWizardFactory);
        return d->displayCategory;
    }

    void IWizardFactory::setDisplayCategory(const QString &displayCategory) {
        Q_D(IWizardFactory);
        d->displayCategory = displayCategory;
    }

}
