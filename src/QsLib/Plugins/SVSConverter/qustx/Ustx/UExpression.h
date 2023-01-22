#ifndef UEXPRESSION_H
#define UEXPRESSION_H

#include <QSharedPointer>
#include <QStringList>

/**
 *  Interpret from OpenUtau.Core.Ustx
 */

class UExpressionDescriptor {
public:
    enum Type {
        Numerical = 0,
        Options = 1,
        Curve = 2,
    };

    QString name;
    QString abbr;
    Type type;
    double min;
    double max;
    double defaultValue;
    bool isFlag;
    QString flag;
    QStringList options;

    UExpressionDescriptor();
    UExpressionDescriptor(const QString &name, const QString &abbr, double min, double max,
                          double defaultValue, const QString &flag = QString());
    UExpressionDescriptor(const QString &name, const QString &abbr, bool isFlag,
                          const QStringList &options);
    ~UExpressionDescriptor();

    static QString TypeToString(Type type);
    static Type StringtoType(const QString &str);
};

class UExpression {
public:
    inline UExpressionDescriptor *descriptor() const {
        return _descriptor.data();
    }

    inline double value() const {
        return _value;
    }

    void setValue(double value);

    UExpression();
    UExpression(UExpressionDescriptor *descriptor);
    UExpression(const QString &abbr);
    UExpression(const UExpression &another);
    UExpression(UExpression &&another);
    ~UExpression();

private:
    double _value;
    QSharedPointer<UExpressionDescriptor> _descriptor;
    QString abbr;
};

#endif // UEXPRESSION_H
