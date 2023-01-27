#include "ThemeTemplate.h"

#include "QsCodec.h"

#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QTextStream>

static const char First_Section_Name[] = "Config";
static const char Default_RegExp_Pattern[] = "\\{\\{.*\\}\\}";
static const char Default_RegExp_Separator[] = "|";
static const int Default_RegExp_Pattern_Left_Length = 3;
static const int Default_RegExp_Pattern_Right_Length = 3;

static const char Theme_Variable_Hint_Size[] = "size";
static const char Theme_Variable_Hint_Color[] = "color";

static QString removeSideQuote(QString token) {
    if (token.front() == '\"') {
        token.remove(0, 1);
    } else if (token.back() == '\"') {
        token.remove(token.size() - 1, 1);
    }
    return token;
}

static int toRealPixelSize(int size) {
    return size;
}

ThemeTemplate::ThemeTemplate() : left(0), right(0) {
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

    // Find "Config" token
    int leftIdx = data.indexOf('{');
    if (leftIdx < 0 ||
        data.left(leftIdx).simplified().compare(First_Section_Name, Qt::CaseSensitive) != 0) {
        // Not found
        qDebug().noquote() << QString::asprintf("ThemeTemplate: The first section name is not %s",
                                                First_Section_Name);
        return false;
    }

    int rightIdx = data.indexOf('}');
    if (rightIdx <= leftIdx) {
        qDebug().noquote() << "ThemeTemplate: Missing end bracket";
        return false;
    }

    // Get config content
    QString configContent = data.mid(leftIdx + 1, rightIdx - leftIdx - 1);
    QStringList configList = configContent.replace('\n', ';').split(';');
    for (const auto &item : qAsConst(configList)) {
        int idx = item.indexOf(':');
        if (idx < 0) {
            continue;
        }

        QString left = item.left(idx).simplified();
        QString right = item.mid(idx + 1).simplified();
        if (left == "pattern") {
            pattern = QsCodec::unescape(removeSideQuote(right));
        } else if (left == "pattern-left") {
            left = removeSideQuote(right).toInt();
        } else if (left == "pattern-right") {
            right = removeSideQuote(right).toInt();
        } else if (left == "separator") {
            separator = removeSideQuote(right);
        }
    }

    // Use default if not specified
    if (pattern.isEmpty()) {
        pattern = Default_RegExp_Pattern;
        left = Default_RegExp_Pattern_Left_Length;
        right = Default_RegExp_Pattern_Right_Length;
    }

    if (separator.isEmpty()) {
        separator = Default_RegExp_Separator;
    }

    QString content = data.mid(rightIdx + 1);

    // Remove white spaces
    while (content.front().isSpace()) {
        content.remove(0, 1);
    }

    this->content = content;

    return true;
}

QString ThemeTemplate::parse(const ThemeConfig &conf) const {
    QString stylesheet;

    QRegularExpression re(pattern);
    QRegularExpressionMatch match;
    int index = 0;

    QString Content = content;
    while ((index = Content.indexOf(re, index, &match)) != -1) {
        QString ValueString;
        QString MatchString = match.captured();
        // Use only the value inside of the brackets {{ }} without the brackets
        auto TemplateVariable = MatchString.midRef(left, MatchString.size() - left - right);

        int idx = TemplateVariable.indexOf(separator);
        if (idx >= 0) {
            QString l = TemplateVariable.left(idx).toString().simplified();
            QString r = TemplateVariable.mid(idx + 1).toString().simplified();

            if (r == Theme_Variable_Hint_Color) {
                auto it = conf.colors.find(l);
                if (it != conf.colors.end()) {
                    ValueString = '#' + it->name();
                }
            } else if (r == Theme_Variable_Hint_Size) {
                auto it = conf.sizes.find(l);
                if (it != conf.sizes.end()) {
                    ValueString = QString::number(toRealPixelSize(it.value()));
                }
            }
        } else {
            while (true) {
                auto l = TemplateVariable.toString();
                // Find color
                {
                    auto it = conf.colors.find(l);
                    if (it != conf.colors.end()) {
                        ValueString = '#' + it->name();
                        break;
                    }
                }
                // Find size
                {
                    auto it = conf.sizes.find(l);
                    if (it != conf.sizes.end()) {
                        ValueString = QString::number(toRealPixelSize(it.value()));
                        break;
                    }
                }
                break;
            }
        }
        Content.replace(index, MatchString.size(), ValueString);
        index += ValueString.size();
    }
    return Content;
}
