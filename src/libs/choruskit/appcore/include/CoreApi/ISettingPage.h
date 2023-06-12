#ifndef ISETTINGPAGE_H
#define ISETTINGPAGE_H

#include <QObject>

#include "QMDisplayString.h"

#include "CkAppCoreGlobal.h"

namespace Core {

    class ISettingPagePrivate;

    class CKAPPCORE_API ISettingPage : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ISettingPage)
    public:
        ISettingPage(const QString &id, QObject *parent = nullptr);
        ~ISettingPage();

    public:
        QString id() const;

        QString title() const;
        void setTitle(const QMDisplayString &title);

        QString description() const;
        void setDescription(const QMDisplayString &description);

        bool addPage(ISettingPage *page);
        bool removePage(ISettingPage *page);
        bool removePage(const QString &id);

        ISettingPage *page(const QString &id) const;
        QList<ISettingPage *> pages() const;
        QList<ISettingPage *> allPages() const;

        inline ISettingPage *parentPage() const;

    public:
        virtual QString sortKeyword() const;

        virtual bool matches(const QString &word) const;
        virtual QWidget *widget() = 0;

        virtual bool accept() = 0;
        virtual void finish() = 0;

    signals:
        void titleChanged(const QString &title);
        void descriptionChanged(const QString &description);

        void pageAdded(ISettingPage *page);
        void pageRemoved(ISettingPage *page);

    protected:
        ISettingPage(ISettingPagePrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<ISettingPagePrivate> d_ptr;
    };

    ISettingPage *ISettingPage::parentPage() const {
        return qobject_cast<ISettingPage *>(parent());
    }

}

#endif // ISETTINGPAGE_H