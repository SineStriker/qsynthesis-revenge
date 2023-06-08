#include "QMDisplayString.h"

#include <QCoreApplication>

namespace QMPrivate {
    class BaseString;
}

class QMDisplayStringData {
public:
    QMDisplayString *q;
    QMPrivate::BaseString *str;
    QVariantHash *properties;

    explicit QMDisplayStringData(const QString &s, QMDisplayString *q);
    explicit QMDisplayStringData(QMDisplayString::GetTextEx func, void *userdata, QMDisplayString *q);
    explicit QMDisplayStringData(QMPrivate::BaseString *str, QVariantHash *properties, QMDisplayString *q);
    ~QMDisplayStringData();
};

namespace QMPrivate {

    class BaseString {
    public:
        explicit BaseString(QMDisplayString::TranslatePolicy policy, QMDisplayStringData *q) : p(policy), q(q){};
        virtual ~BaseString() = default;

        virtual QString text() const = 0;
        virtual BaseString *clone(QMDisplayStringData *q) const = 0;

        QMDisplayString::TranslatePolicy p;
        QMDisplayStringData *q;
    };

    class PlainString : public BaseString {
    public:
        explicit PlainString(QString s, QMDisplayStringData *q)
            : BaseString(QMDisplayString::TranslateIgnored, q), s(std::move(s)){};

        QString text() const override {
            return s;
        }
        BaseString *clone(QMDisplayStringData *q) const override {
            return new PlainString(s, q);
        }

        QString s;
    };

    class CallbackString : public BaseString {
    public:
        explicit CallbackString(QMDisplayString::GetTextEx func, void *userdata, QMDisplayStringData *q)
            : BaseString(QMDisplayString::TranslateAlways, q), func(func), userdata(userdata){};

        QString text() const override {
            return func(*(q->q), userdata);
        }
        BaseString *clone(QMDisplayStringData *q) const override {
            return new CallbackString(func, userdata, q);
        }

        QMDisplayString::GetTextEx func;
        void *userdata;
    };

}

static QString qm_get_text_default(const QMDisplayString &, void *userdata) {
    return reinterpret_cast<QMDisplayString::GetText>(userdata)();
}

QMDisplayStringData::QMDisplayStringData(const QString &s, QMDisplayString *q)
    : q(q), str(new QMPrivate::PlainString(s, this)), properties(nullptr) {
}

QMDisplayStringData::QMDisplayStringData(QMDisplayString::GetTextEx func, void *userdata, QMDisplayString *q)
    : q(q), str(new QMPrivate::CallbackString(func, userdata, this)), properties(nullptr) {
}

QMDisplayStringData::QMDisplayStringData(QMPrivate::BaseString *str, QVariantHash *properties, QMDisplayString *q)
    : q(q), str(str->clone(this)), properties(properties ? new QVariantHash(*properties) : nullptr) {
}

QMDisplayStringData::~QMDisplayStringData() {
    delete str;
    delete properties;
}

QMDisplayString::QMDisplayString(const QString &s) : d(new QMDisplayStringData(s, this)) {
}

QMDisplayString::QMDisplayString(QMDisplayString::GetText func)
    : d(new QMDisplayStringData(qm_get_text_default, reinterpret_cast<void *>(func), this)) {
}

QMDisplayString::QMDisplayString(GetTextEx func, void *userdata) : d(new QMDisplayStringData(func, userdata, this)) {
}

QMDisplayString::QMDisplayString(const QMDisplayString &other)
    : d(new QMDisplayStringData(other.d->str, other.d->properties, this)) {
}

QMDisplayString::QMDisplayString(QMDisplayString &&other) noexcept : d(other.d) {
    d->q = this;
}

QMDisplayString::~QMDisplayString() {
    if (d->q == this)
        delete d;
}

QMDisplayString &QMDisplayString::operator=(const QString &s) {
    setPlainString(s);
    return *this;
}

QMDisplayString &QMDisplayString::operator=(const QMDisplayString &other) {
    if (&other == this) {
        return *this;
    }

    d = new QMDisplayStringData(other.d->str, other.d->properties, this);
    return *this;
}

QMDisplayString &QMDisplayString::operator=(QMDisplayString &&other) noexcept {
    if (&other == this) {
        return *this;
    }

    d = other.d;
    d->q = this;
    return *this;
}

QString QMDisplayString::text() const {
    return d->str->text();
}

QMDisplayString::TranslatePolicy QMDisplayString::translatePolicy() const {
    return d->str->p;
}
void QMDisplayString::setTranslateCallback(QMDisplayString::GetText func) {
    setTranslateCallback(qm_get_text_default, reinterpret_cast<void *>(func));
}

void QMDisplayString::setTranslateCallback(QMDisplayString::GetTextEx func, void *userdata) {
    if (d->str->p != TranslateAlways) {
        delete d->str;
        d->str = new QMPrivate::CallbackString(func, userdata, d);
    } else {
        auto str = static_cast<QMPrivate::CallbackString *>(d->str);
        str->func = func;
        str->userdata = userdata;
    }
}

void QMDisplayString::setPlainString(const QString &s) {
    if (d->str->p != TranslateIgnored) {
        delete d->str;
        d->str = new QMPrivate::PlainString(s, d);
    } else {
        auto str = static_cast<QMPrivate::PlainString *>(d->str);
        str->s = s;
    }
}

QVariant QMDisplayString::property(const QString &key) const {
    return d->properties ? d->properties->value(key) : QVariant();
}

void QMDisplayString::setProperty(const QString &key, const QVariant &value) {
    if (!d->properties) {
        d->properties = new QVariantHash();
    }

    auto &properties = *d->properties;
    auto it = properties.find(key);
    if (it == properties.end()) {
        if (value.isNull() || !value.isValid())
            return;
        properties.insert(key, value);
    } else {
        if (value.isNull() || !value.isValid())
            properties.erase(it);
        else
            it.value() = value;
    }
}

QVariantHash QMDisplayString::propertyMap() const {
    return d->properties ? *d->properties : QVariantHash();
}
