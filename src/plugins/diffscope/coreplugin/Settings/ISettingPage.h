#ifndef ISETTINGPAGE_H
#define ISETTINGPAGE_H

#include <QObject>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class ISettingPagePrivate;

    class CORE_EXPORT ISettingPage : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ISettingPage)
    public:
        ISettingPage(const QString &id, QObject *parent = nullptr);
        ~ISettingPage();

    public:
        QString id() const;

        QString title() const;
        void setTitle(const QString &title);

    public:
        virtual bool matches(const QString &word) const;
        virtual QWidget *widget() = 0;

        virtual bool accept() = 0;
        virtual void finish() = 0;

    signals:
        void titleChanged(const QString &title);

    protected:
        ISettingPage(ISettingPagePrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<ISettingPagePrivate> d_ptr;
    };

}

#endif // ISETTINGPAGE_H