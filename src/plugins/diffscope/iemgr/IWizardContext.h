#ifndef CHORUSKIT_IWIZARDCONTEXT_H
#define CHORUSKIT_IWIZARDCONTEXT_H

#include "IEMgrGlobal.h"

#include <CoreApi/IWindowContext.h>

namespace IEMgr {

    class IWizardContextPrivate;

    class IEMGR_EXPORT IWizardContext : public Core::IWindowContext {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IWizardContext)
    public:
        explicit IWizardContext(Core::IWindow *winHandle, QObject *parent = nullptr);
        ~IWizardContext();

        enum Result {
            Success,
            Canceled,
            Retry,
        };

    public:
        QString wizardId() const;
        void setWizardId(const QString &id);

        QString path() const;
        void setPath(const QString &path);

        Result result() const;
        void setResult(Result result);
    };

}

#endif // CHORUSKIT_IWIZARDCONTEXT_H
