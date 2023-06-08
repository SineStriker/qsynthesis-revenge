#ifndef CHORUSKIT_QMDISPLAYSTRING_H
#define CHORUSKIT_QMDISPLAYSTRING_H

#include <QPair>
#include <QSharedDataPointer>
#include <QString>
#include <QVariant>

#include "QMGlobal.h"

class QMDisplayStringData;

class QMCORELIB_API QMDisplayString {
public:
    enum TranslatePolicy {
        TranslateAlways,
        TranslateIgnored,
    };

    typedef QString (*GetText)();
    typedef QString (*GetTextEx)(const QMDisplayString &, void *);

    QMDisplayString() : QMDisplayString(QString()){};
    QMDisplayString(const QString &s);
    QMDisplayString(GetText func);
    explicit QMDisplayString(GetTextEx func, void *userdata = nullptr);
    QMDisplayString(const QMDisplayString &other);
    QMDisplayString(QMDisplayString &&other) noexcept;
    ~QMDisplayString();

    QMDisplayString &operator=(const QString &s);
    QMDisplayString &operator=(const QMDisplayString &other);
    QMDisplayString &operator=(QMDisplayString &&other) noexcept;

    QString text() const;
    TranslatePolicy translatePolicy() const;

    void setTranslateCallback(GetText func);
    void setTranslateCallback(GetTextEx func, void *userdata = nullptr);
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
