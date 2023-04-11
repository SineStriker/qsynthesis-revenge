#include "IWizardContext.h"

#include <CoreApi/IWindowContext_p.h>

namespace IEMgr {

    class IWizardContextPrivate : public Core::IWindowContextPrivate {
    public:
        QString wizardId;
        QString path;
        IWizardContext::Result result;

        IWizardContextPrivate() : result(IWizardContext::Success) {
        }
    };

    IWizardContext::IWizardContext(Core::IWindow *winHandle, QObject *parent)
        : Core::IWindowContext(*new IWizardContextPrivate(), winHandle, parent) {
    }

    IWizardContext::~IWizardContext() {
    }

    QString IWizardContext::wizardId() const {
        Q_D(const IWizardContext);
        return d->wizardId;
    }

    void IWizardContext::setWizardId(const QString &id) {
        Q_D(IWizardContext);
        d->wizardId = id;
    }

    QString IWizardContext::path() const {
        Q_D(const IWizardContext);
        return d->path;
    }

    void IWizardContext::setPath(const QString &path) {
        Q_D(IWizardContext);
        d->path = path;
    }

    IWizardContext::Result IWizardContext::result() const {
        Q_D(const IWizardContext);
        return d->result;
    }

    void IWizardContext::setResult(IWizardContext::Result result) {
        Q_D(IWizardContext);
        d->result = result;
    }

}