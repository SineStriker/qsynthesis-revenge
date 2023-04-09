#ifndef IIMPORTWIZARD_H
#define IIMPORTWIZARD_H

#include <QDateTime>
#include <QObject>

#include "IEMgrGlobal.h"

#include <CoreApi/IWindow.h>

namespace IEMgr {

    class WizardContextPrivate;

    class IEMGR_EXPORT WizardContext {
    public:
        QDateTime dateTime() const;

        Core::IWindow *windowHandle() const;

    private:
        explicit WizardContext(WizardContextPrivate *d);

        WizardContextPrivate *d;

        friend class IManager;
        friend class IImportWizard;
    };

    class IImportWizardPrivate;

    class IEMGR_EXPORT IImportWizard : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IImportWizard)
    public:
        explicit IImportWizard(const QString &id, QObject *parent = nullptr);
        ~IImportWizard();

        virtual QString filter() const;

        virtual bool runWizard(WizardContext *context) = 0;

    public:
        QString id() const;

        QString iconPath() const;
        void setIconPath(const QString &path);

        QString description() const;
        void setDescription(const QString &description);

        QString displayName() const;
        void setDisplayName(const QString &displayName);

        QString category() const;
        void setCategory(const QString &category);

        QString displayCategory() const;
        void setDisplayCategory(const QString &displayCategory);

    protected:
        IImportWizard(IImportWizardPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<IImportWizardPrivate> d_ptr;

        friend class IManager;
    };

}

#endif // IIMPORTWIZARD_H