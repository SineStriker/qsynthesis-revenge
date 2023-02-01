#include "ThemeTemplate.h"

#include "QsCodec.h"

#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QTextStream>

static const char First_Section_Name[] = "Config";
static const char Default_RegExp_Pattern[] = "\\{\\{(.*?)\\}\\}";
static const char Default_RegExp_Separator[] = "|";

static const char Theme_Variable_Hint_Size[] = "size";
static const char Theme_Variable_Hint_Color[] = "color";

static QString removeSideQuote(QString token) {
    if (token.front() == '\"') {
        token.remove(0, 1);
    }
    if (token.back() == '\"') {
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

    // Remove all comments
    {
        QRegularExpression reg("/\\*(.*?)\\*/", QRegularExpression::MultilineOption |
                                                    QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match;
        int index = 0;
        while ((index = data.indexOf(reg, index, &match)) != -1) {
            data.remove(index, match.captured(0).size());
        }
    }

    // Remove all multi-lines
    {
        QRegularExpression reg("\\n(\\s*)\\n", QRegularExpression::MultilineOption |
                                                   QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match;
        int index = 0;
        while ((index = data.indexOf(reg, index, &match)) != -1) {
            data.replace(index, match.captured(0).size(), "\n");
            index++;
        }
    }

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
    QRegularExpression lineReg("(\\w*)\\s*:\\s*(.*);\n");
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

QString ThemeTemplate::parse(const QMap<QString, QString> &colors,
                             const QMap<QString, int> &sizes) const {
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

        int idx = TemplateVariable.indexOf(separator);
        if (idx >= 0) {
            QString l = TemplateVariable.left(idx).simplified();
            QString r = TemplateVariable.mid(idx + 1).simplified();

            if (r == Theme_Variable_Hint_Color) {
                auto it = colors.find(l);
                if (it != colors.end()) {
                    ValueString = it.value();
                }
            } else if (r == Theme_Variable_Hint_Size) {
                auto it = sizes.find(l);
                if (it != sizes.end()) {
                    ValueString = QString::number(toRealPixelSize(it.value()));
                }
            }
        } else {
            while (true) {
                auto &l = TemplateVariable;
                // Find color
                {
                    auto it = colors.find(l);
                    if (it != colors.end()) {
                        ValueString = it.value();
                        break;
                    }
                }
                // Find size
                {
                    auto it = sizes.find(l);
                    if (it != sizes.end()) {
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
