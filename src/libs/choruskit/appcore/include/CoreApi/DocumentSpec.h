#ifndef DOCUMENTSPEC_H
#define DOCUMENTSPEC_H

#include <QObject>

#include "IWindowContext.h"

namespace Core {

    class DocumentSpecPrivate;

    class CKAPPCORE_API DocumentSpec : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DocumentSpec)
    public:
        explicit DocumentSpec(const QString &id, QObject *parent = nullptr);
        ~DocumentSpec();

    public:
        QString id() const;

        QString displayName() const;
        void setDisplayName(const QString &displayName);

        QIcon icon() const;
        void setIcon(const QIcon &icon);

    public:
        virtual QStringList supportedExtensions() const = 0;
        virtual QString filter() const = 0;
        virtual QString saveFilter() const;
        virtual bool open(const QString &fileName, IWindowContext *context);

        virtual bool canRecover() const;
        virtual bool recover(const QString &logDir, const QString &fileName, IWindowContext *context);

    protected:
        DocumentSpec(DocumentSpecPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<DocumentSpecPrivate> d_ptr;
    };

}

#endif // DOCUMENTSPEC_H