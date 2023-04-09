#include "IImportWizard.h"
#include "IImportWizard_p.h"

#include <QMSystem.h>

namespace IEMgr {

    QDateTime WizardContext::dateTime() const {
        return d->time;
    }
    Core::IWindow *WizardContext::windowHandle() const {
        return d->winHandle;
    }

    WizardContext::WizardContext(IEMgr::WizardContextPrivate *d) : d(d) {
    }

    IImportWizardPrivate::IImportWizardPrivate() {
    }

    IImportWizardPrivate::~IImportWizardPrivate() {
    }

    void IImportWizardPrivate::init() {
    }

    IImportWizard::IImportWizard(const QString &id, QObject *parent)
        : IImportWizard(*new IImportWizardPrivate(), id, parent) {
    }

    IImportWizard::~IImportWizard() {
    }

    IImportWizard::IImportWizard(IImportWizardPrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
        d.id = id;
    }

    QString IImportWizard::id() const {
        Q_D(const IImportWizard);
        return d->id;
    }

    QString IImportWizard::filter() const {
        return QString("%1(%2)").arg(tr("All Files"), QMOs::allFilesFilter());
    }

    QString IImportWizard::iconPath() const {
        Q_D(const IImportWizard);
        return d->iconPath;
    }

    void IImportWizard::setIconPath(const QString &path) {
        Q_D(IImportWizard);
        d->iconPath = path;
    }

    QString IImportWizard::description() const {
        Q_D(const IImportWizard);
        return d->description;
    }

    void IImportWizard::setDescription(const QString &description) {
        Q_D(IImportWizard);
        d->description = description;
    }

    QString IImportWizard::displayName() const {
        Q_D(const IImportWizard);
        return d->displayName;
    }

    void IImportWizard::setDisplayName(const QString &displayName) {
        Q_D(IImportWizard);
        d->displayName = displayName;
    }

    QString IImportWizard::category() const {
        Q_D(const IImportWizard);
        return d->category;
    }

    void IImportWizard::setCategory(const QString &category) {
        Q_D(IImportWizard);
        d->category = category;
    }

    QString IImportWizard::displayCategory() const {
        Q_D(const IImportWizard);
        return d->displayCategory;
    }

    void IImportWizard::setDisplayCategory(const QString &displayCategory) {
        Q_D(IImportWizard);
        d->displayCategory = displayCategory;
    }

}
