#include "UExpression.h"

UExpressionDescriptor::UExpressionDescriptor() {
}

UExpressionDescriptor::UExpressionDescriptor(const QString &name, const QString &abbr, double min,
                                             double max, double defaultValue, const QString &flag)
    : name(name), abbr(abbr.toLower()), type(Numerical), min(min), max(max),
      defaultValue(qMin(max, qMax(min, defaultValue))), isFlag(!flag.isEmpty()), flag(flag) {
}

UExpressionDescriptor::UExpressionDescriptor(const QString &name, const QString &abbr, bool isFlag,
                                             const QStringList &options)
    : name(name), abbr(abbr.toLower()), type(Options), min(0), max(options.size() - 1),
      isFlag(isFlag), options(options) {
}

UExpressionDescriptor::~UExpressionDescriptor() {
}

void UExpression::setValue(double value) {
    _value = (_descriptor.isNull())
                 ? value
                 : qMin(_descriptor->max, qMax(_descriptor->min, _descriptor->defaultValue));
}

UExpression::UExpression() {
}

UExpression::UExpression(UExpressionDescriptor *descriptor)
    : _descriptor(descriptor), abbr(descriptor->abbr) {
}

UExpression::UExpression(const QString &abbr) : abbr(abbr) {
}

UExpression::UExpression(const UExpression &another) {
    _value = another._value;
    _descriptor = another._descriptor;
    abbr = another.abbr;
}

UExpression::UExpression(UExpression &&another) {
    _value = another._value;
    _descriptor = std::move(another._descriptor);
    abbr = another.abbr;
}

UExpression::~UExpression() {
}

UExpression &UExpression::operator=(const UExpression &another) {
    _value = another._value;
    _descriptor = another._descriptor;
    abbr = another.abbr;
    return *this;
}

UExpression &UExpression::operator=(UExpression &&another) {
    _value = another._value;
    _descriptor = std::move(another._descriptor);
    abbr = another.abbr;
    return *this;
}
