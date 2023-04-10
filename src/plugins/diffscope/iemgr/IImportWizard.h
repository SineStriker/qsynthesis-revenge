#ifndef IIMPORTWIZARD_H
#define IIMPORTWIZARD_H

#include <QDateTime>
#include <QObject>

#include "IEMgrGlobal.h"

#include <CoreApi/IWindow.h>

namespace IEMgr {

    class IImportWizardPrivate;

    class IEMGR_EXPORT IImportWizard : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IImportWizard)
    public:
        explicit IImportWizard(const QString &id, QObject *parent = nullptr);
        ~IImportWizard();

        virtual QString filter() const;

        virtual bool runWizard(Core::IWindow *windowHandle) = 0;

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