#include "ThemeTemplate.h"

#include "QsSystem.h"

#include "QsCodec.h"
#include "QsLinq.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>

static const char First_Section_Name[] = "Config";
static const char Default_RegExp_Pattern[] = "\\{\\{(.*?)\\}\\}";
static const char Default_RegExp_Separator[] = "|";

static const char Theme_Variable_Hint_Size[] = "size";
static const char Theme_Variable_Hint_Num[] = "num";
static const char Theme_Variable_Hint_String[] = "str";

static const char Theme_Values_Object_Delimiter[] = "_delimiter";
static const char Theme_Values_Object_Array[] = "_array";
static const char Theme_Values_Object_Object[] = "_object";

static QString removeSideQuote(QString token) {
    if (token.front() == '\"') {
        token.remove(0, 1);
    }
    if (token.back() == '\"') {
        token.remove(token.size() - 1, 1);
    }
    return token;
}

static QString removeMultiLines(QString s) {
    QRegularExpression reg("\\n(\\s*)\\n", QRegularExpression::MultilineOption |
                                               QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match;
    int index = 0;
    while ((index = s.indexOf(reg, index, &match)) != -1) {
        s.replace(index, match.captured(0).size(), "\n");
        index++;
    }
    return s;
}

static inline int toRealPixelSize(double size, double dpi) {
    return qRound(size * dpi / QsOs::unitDpi());
}

static inline QString toPixelStr(int size) {
    return QString::number(size) + QString(size == 0 ? "" : "px");
}

struct Arguments {
    QString hint;
    QString defaultValue;
    double dpi;
};

template <class Mapper>
static QString extractNumListValue(const QJsonValue &val, Mapper mapper) {
    QString res;
    QString delim = " ";

    auto extractArr = [&](const QJsonArray &arr) {
        QList<double> digits;
        for (const auto &item : qAsConst(arr)) {
            if (item.isDouble()) {
                digits.append(item.toDouble());
            }
        }
        return QsLinq::Select<double, QString>(
                   digits, //
                   [&](double digit) -> QString { return mapper(digit); })
            .join(delim);
    };

    auto extractMap = [&](const QJsonObject &obj) {
        QMap<QString, double> digitMap;
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if (it->isDouble()) {
                digitMap.insert(it.key(), it->toDouble());
            }
        }
        return QsLinq::Select<QString, double, QString>(
                   digitMap,
                   [&](const QString &key, double digit) -> QString {
                       return key + ":" + mapper(digit);
                   })
            .join(delim);
    };

    if (val.isArray()) {
        res = extractArr(val.toArray());
    } else if (val.isDouble()) {
        res = mapper(val.toDouble());
    } else if (val.isObject()) {
        QJsonObject obj = val.toObject();
        auto it = obj.find(Theme_Values_Object_Delimiter);
        if (it != obj.end() && it->isString()) {
            delim = it->toString();
        }
        while (true) {
            it = obj.find(Theme_Values_Object_Array);
            if (it != obj.end() && it->isArray()) {
                res = extractArr(it->toArray());
                break;
            }
            it = obj.find(Theme_Values_Object_Object);
            if (it != obj.end() && it->isObject()) {
                res = extractMap(it->toObject());
                break;
            }
            break;
        }
    }
    return res;
}

static QString ValueToString(const ThemeConfig::Value &val, const Arguments &args) {
    const auto &jsonVal = val.val;
    QString res;
    if (args.hint == Theme_Variable_Hint_Size) {
        res = extractNumListValue(jsonVal, //
                                  [&](double digit) {
                                      return toPixelStr(
                                          toRealPixelSize(digit * val.ratio, args.dpi)); //
                                  });
    } else if (args.hint == Theme_Variable_Hint_Num) {
        res = extractNumListValue(jsonVal, //
                                  [](double digit) {
                                      return QString::number(digit); //
                                  });
    } else if (args.hint == Theme_Variable_Hint_String ||
               (args.hint.isEmpty() && !jsonVal.isNull())) {
        res = jsonVal.toString();
    } else {
        res = args.defaultValue;
    }
    return res;
}

ThemeTemplate::ThemeTemplate() {
}

ThemeTemplate::~ThemeTemplate() {
}

bool ThemeTemplate::load(const QString &filename) {
    QFile file(filename);

    // Open file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QString data = QTextStream(&file).readAll();
    file.close();

    // Remove all comments
    {
        QRegularExpression reg("\\/\\*(.*?)\\*\\/",
                               QRegularExpression::MultilineOption |
                                   QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match;
        int index = 0;
        while ((index = data.indexOf(reg, index, &match)) != -1) {
            data.remove(index, match.captured(0).size());
        }
    }

    // Remove all multilines
    data = removeMultiLines(data);

    // Find "Config" token
    int leftIdx = data.indexOf('{');
    if (leftIdx < 0 ||
        data.left(leftIdx).simplified().compare(First_Section_Name, Qt::CaseSensitive) != 0) {
        // Not found
        qDebug().noquote() << QString::asprintf("ThemeTemplate: The first section name is not %s",
                                                First_Section_Name);
        return false;
    }

    QRegularExpression rbReg(";\\s*(\\})", QRegularExpression::MultilineOption);
    QRegularExpressionMatch match;

    int rightIdx = data.indexOf(rbReg, leftIdx, &match);
    if (rightIdx <= leftIdx) {
        qDebug().noquote() << "ThemeTemplate: Missing end bracket";
        return false;
    }
    rightIdx = match.capturedStart(1);

    // Get config content
    QRegularExpression lineReg("([-\\w]+)\\s*:\\s*(.*);\n");
    QString configContent = data.mid(leftIdx + 1, rightIdx - leftIdx - 1) + "\n";
    int idx = 0;
    while ((idx = configContent.indexOf(lineReg, idx, &match)) != -1) {
        QString left = match.captured(1);
        QString right = match.captured(2);
        if (left == "pattern") {
            pattern = QsCodec::unescape(removeSideQuote(right));
        } else if (left == "separator") {
            separator = removeSideQuote(right);
        }
        idx += match.captured().size();
    }

    // Use default if not specified
    if (pattern.isEmpty()) {
        pattern = Default_RegExp_Pattern;
    }

    if (separator.isEmpty()) {
        separator = Default_RegExp_Separator;
    }

    QString content = data.mid(rightIdx + 1);

    // Remove white spaces
    this->content = content.simplified();

    return true;
}

QString ThemeTemplate::parse(const QMap<QString, ThemeConfig::Value> &vars, double dpi) const {
    QRegularExpression re(pattern);
    QRegularExpressionMatch match;
    int index = 0;

    QString Content = content;

    while ((index = Content.indexOf(re, index, &match)) != -1) {
        QString ValueString;
        QString MatchString = match.captured();

        // Use only the value inside of the brackets {{ }} without the brackets
        auto TemplateVariable = match.captured(1);
        if (TemplateVariable.isEmpty()) {
            continue;
        }

        QStringList list = TemplateVariable.split(separator);
        if (!list.isEmpty()) {
            QString key = list.front();
            QString hint = list.size() == 1 ? QString() : list.at(1);
            QString defaultValue = list.size() <= 2 ? QString() : list.at(2);
            auto it = vars.find(key);
            if (it != vars.end()) {
                ValueString = ValueToString(it.value(), Arguments{hint, defaultValue, dpi});
            } else if (!defaultValue.isEmpty()) {
                if (hint == Theme_Variable_Hint_Size) {
                    ValueString = toPixelStr(toRealPixelSize(defaultValue.toDouble(), dpi));
                } else {
                    ValueString = defaultValue;
                }
            }
        }

        if (ValueString.isEmpty()) {
            ValueString = "/**/";
        }

        Content.replace(index, MatchString.size(), ValueString);
        index += ValueString.size();
    }
    // Remove all properties containing unparsed variables
    {
        QRegularExpression reg(R"((?<=({|;))\s*[-\w]+?\s*?:[^;]*?\/\*\*\/.*?;)");
        QRegularExpressionMatch match;
        int index = 0;
        while ((index = Content.indexOf(reg, index, &match)) != -1) {
            Content.remove(index, match.captured(0).size());
        }
    }

    // Content = removeMultiLines(Content);
    Content = Content.simplified();

    return Content;
}
