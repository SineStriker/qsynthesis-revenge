#include "QsCss.h"

#include <QtGui/private/qcssparser_p.h>

QStringList QsCss::extractFunctionToStringList(const QString &str, bool *ok) {
    int leftParen = str.indexOf('(');
    int rightParen = str.lastIndexOf(')');
    QStringList res;
    if (leftParen > 0 && rightParen > leftParen) {
        if (ok) {
            *ok = true;
        }
        res = QStringList(
            {str.mid(0, leftParen), str.mid(leftParen + 1, rightParen - leftParen - 1)});
    } else {
        if (ok) {
            *ok = false;
        }
    }
    return res;
}

QColor QsCss::CssStringToColor(const QString &str) {
    if (str.isEmpty()) {
        return QColor();
    }

    QCss::Declaration dec;
    QCss::Value val;

    bool ok;
    QStringList list = extractFunctionToStringList(str, &ok);
    if (ok) {
        val.type = QCss::Value::Function;
        val.variant = list;
    } else {
        val.type = QCss::Value::String;
        val.variant = str;
    }

    dec.d->values.push_back(val);
    return dec.colorValue();
}
