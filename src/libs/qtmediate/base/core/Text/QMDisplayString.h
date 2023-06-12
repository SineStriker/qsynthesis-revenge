#ifndef CHORUSKIT_QMDISPLAYSTRING_H
#define CHORUSKIT_QMDISPLAYSTRING_H

#include <functional>

#include <QPair>
#include <QSharedDataPointer>
#include <QString>
#include <QVariant>

#include "QMGlobal.h"

class QMDisplayStringData;

class QMCORE_EXPORT QMDisplayString {
public:
    enum TranslatePolicy {
        TranslateIgnored = 0,
        TranslateAlways = 1,
        TranslateAlwaysEx = 3,
    };

    using GetText = std::function<QString()>;
    using GetTextEx = std::function<QString(const QMDisplayString &, void *)>;

    QMDisplayString() : QMDisplayString(QString()){};
    QMDisplayString(const QString &s);
    QMDisplayString(const GetText &func);
    explicit QMDisplayString(const GetTextEx &func, void *userdata = nullptr);
    ~QMDisplayString();

    QMDisplayString(const QMDisplayString &other);
    QMDisplayString(QMDisplayString &&other) noexcept;

    QMDisplayString &operator=(const QString &s);
    QMDisplayString &operator=(const QMDisplayString &other);
    QMDisplayString &operator=(QMDisplayString &&other) noexcept;

    QString text() const;
    TranslatePolicy translatePolicy() const;

    void setTranslateCallback(const GetText &func);
    void setTranslateCallback(const GetTextEx &func, void *userdata = nullptr);
    void setPlainString(const QString &s);

    QVariant property(const QString &key) const;
    void setProperty(const QString &key, const QVariant &value);
    QVariantHash propertyMap() const;

    inline operator QString() const {
        return text();
    }

    template <class Func>
    inline QMDisplayString(Func func) : QMDisplayString(GetText(func)){};

private:
    QMDisplayStringData *d;

    friend class QMDisplayStringData;
};

Q_DECLARE_METATYPE(QMDisplayString)

#endif // CHORUSKIT_QMDISPLAYSTRING_H
