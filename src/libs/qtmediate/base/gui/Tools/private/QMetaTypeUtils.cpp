#include "QMetaTypeUtils.h"

#include "QMetaTypeImpl_p.h"
#include "QPixelSize.h"

#include "QMBatch.h"
#include "QMCss.h"

#include <QMetaType>
#include <QRegularExpression>
#include <QSize>

namespace QMetaTypeUtils {

    QVector<int> SplitStringToIntList(const QString &str) {
        QString s0 = str;
        if (s0.startsWith('(') && s0.endsWith(')'))
            s0 = s0.mid(1, s0.size() - 2);

        QStringList valueList;
        if (s0.contains(',')) {
            valueList = s0.split(',');
        } else {
            valueList = s0.split(' ');
        }

        QVector<int> x;
        for (const auto &i : valueList) {
            QString s = i.simplified();
            QLatin1String px(PixelSizeUnit);
            if (s.endsWith(px, Qt::CaseInsensitive)) {
                s.chop(px.size());
            }
            bool isNum;
            auto num = s.toInt(&isNum);
            if (isNum) {
                x.push_back(num);
            } else {
                x.push_back(0);
            }
        }

        return x;
    }

    QVector<double> SplitStringToDoubleList(const QString &str) {
        QString s0 = str;
        if (s0.startsWith('(') && s0.endsWith(')'))
            s0 = s0.mid(1, s0.size() - 2);

        QStringList valueList;
        if (s0.contains(',')) {
            valueList = s0.split(',');
        } else {
            valueList = s0.split(' ');
        }

        QVector<double> x;
        for (const auto &i : valueList) {
            QString s = i.simplified();
            QLatin1String px(PixelSizeUnit);
            if (s.endsWith(px, Qt::CaseInsensitive)) {
                s.chop(px.size());
            }
            bool isNum;
            auto num = s.toDouble(&isNum);
            if (isNum) {
                x.push_back(num);
            } else {
                x.push_back(0);
            }
        }

        return x;
    }

    QStringList SplitStringByComma(const QStringRef &str) {
        QStringList res;
        int level = 0;
        QString word;
        bool isCommented = false;
        bool isQuoted = false;
        bool isSingleQuoted = false;
        for (int i = 0; i < str.size(); ++i) {
            QChar ch = str.at(i);
            if (!isCommented) {
                if (!isSingleQuoted && ch == '\"') {
                    isQuoted = !isQuoted;
                    word.append(ch);
                    continue;
                } else if (!isQuoted && ch == '\'') {
                    isSingleQuoted = !isSingleQuoted;
                    word.append(ch);
                    continue;
                }
            }
            if (!isQuoted && !isSingleQuoted && i < str.size() - 1) {
                QChar nxt = str.at(i + 1);
                if (ch == '/' && nxt == '*') {
                    isCommented = true;
                    i++;
                    continue;
                } else if (ch == '*' && nxt == '/') {
                    isCommented = false;
                    i++;
                    continue;
                }
            }
            if (!isCommented) {
                if (level == 0 && ch == ',') {
                    res.append(word);
                    word.clear();
                } else {
                    if (ch == '(') {
                        level++;
                    } else if (ch == ')') {
                        level--;
                    }
                    word.append(ch);
                }
            }
        }

        word = word.trimmed();
        if (!word.isEmpty()) {
            res.append(word);
        }

        return res;
    }

    static const char *colorFuncNames[] = {
        "rgb",
        "rgba",
        "hsv",
        "hsvl",
    };

    QVariant StringToVariant(const QString &s) {
        bool ok;
        QStringList list = QMCss::extractFunctionToStringList(s, &ok);
        if (ok) {
            const auto &func = list.front();
            int id = QMetaTypeImpl::FunctionNameToType(func);
            if (id >= 0) {
                QVariant var(list);
                if (var.convert(id)) {
                    return var;
                } else {
                    return s;
                }
            } else {
                for (const auto &item : qAsConst(colorFuncNames)) {
                    if (func == item) {
                        return QMCss::CssStringToColor(s);
                    }
                }
                return s;
            }
        } else {
            if (s.startsWith("#")) {
                return QMCss::CssStringToColor(s);
            } else if (s.endsWith(QLatin1String(PixelSizeUnit), Qt::CaseInsensitive)) {
                if (s.contains(' ')) {
                    // QSize
                    QRegularExpression regex(R"((\d+)px?\s+(\d+)px?)");
                    QRegularExpressionMatch match = regex.match(s);

                    QSize size;
                    if (match.hasMatch()) {
                        QString widthString = match.captured(1);
                        QString heightString = match.captured(2);
                        int width = widthString.toInt();
                        int height = heightString.toInt();
                        size = QSize(width, height);
                    }
                    return size;
                } else {
                    // QPixelSize
                    QVariant var;
                    var.setValue(QPixelSize::fromString(s));
                    return var;
                }
            } else {
                bool ok2;
                double val = s.toDouble(&ok2);
                if (ok2) {
                    return val;
                } else {
                    return s;
                }
            }
        }
    }

    bool StringToBool(const QString &s) {
        if (!s.compare("true", Qt::CaseInsensitive))
            return true;
        return false;
    }

    int FindFirstEqualSign(const QString &s) {
        for (int i = 0; i < s.size(); ++i) {
            const auto &ch = s.at(i);
            if (ch == '=')
                return i;
            if (!ch.isLetterOrNumber() && ch != '_' && ch != '-')
                return -1;
        }
        return -1;
    }

    QHash<QString, QString> ParseFuncArgList(const QString &s, const QStringList &keys,
                                             const QHash<QString, QPair<QString, FallbackOption>> &fallbacks,
                                             bool addParen) {
        if (keys.isEmpty())
            return {};

        QHash<QString, QString> res;
        bool hasParen = false;
        if (addParen || (hasParen = s.startsWith('(') && s.endsWith(')'))) {
            auto list = SplitStringByComma(hasParen ? s.midRef(1, s.size() - 2) : &s);
            bool isKeywordArg = false;
            for (int i = 0; i < list.size(); ++i) {
                auto item = list.at(i).trimmed();
                int eq = FindFirstEqualSign(item);
                if (eq < 0) {
                    // Positional argument
                    if (isKeywordArg || keys.size() <= i) {
                        // Not allowed
                        return res;
                    }

                    res.insert(keys.at(i), QMBatch::removeSideQuote(item.trimmed()));
                    continue;
                }

                // Keyword argument
                isKeywordArg = true;
                res.insert(item.left(eq).trimmed(), QMBatch::removeSideQuote(item.mid(eq + 1).trimmed()));
            }
        } else {
            res.insert(keys.front(), QMBatch::removeSideQuote(s.trimmed()));
        }

        // Handle fallbacks
        for (const auto &key : keys) {
            if (res.contains(key))
                continue;

            auto it = fallbacks.find(key);
            if (it == fallbacks.end())
                continue;

            const auto &fallback = it.value();
            if (fallback.second == FO_Value) {
                // Use value
                res.insert(key, fallback.first);
            } else {
                // Use reference
                res.insert(key, res.value(fallback.first));
            }
        }

        return res;
    }

    bool ParseClickStateArgList(const QString &s, QString arr[], bool resolveFallback) {
        QHash<QString, QPair<QString, FallbackOption>> fallbacks;
        if (resolveFallback) {
            fallbacks = {
                {"over",      {"up", FO_Reference}      },
                {"down",      {"over", FO_Reference}    },
                {"disabled",  {"up", FO_Reference}      },
                {"up2",       {"up", FO_Reference}      },
                {"over2",     {"up2", FO_Reference}    },
                {"down2",     {"over2", FO_Reference}    },
                {"disabled2", {"up2", FO_Reference}},
            };
        }

        auto res0 = ParseFuncArgList(s,
                                     {
                                         "up",
                                         "over",
                                         "down",
                                         "disabled",
                                         "up2",
                                         "over2",
                                         "down2",
                                         "disabled2",
                                     },
                                     fallbacks);
        if (res0.isEmpty())
            return false;

        arr[QM::CS_Normal] = res0.value("up");
        arr[QM::CS_Hover] = res0.value("over");
        arr[QM::CS_Pressed] = res0.value("down");
        arr[QM::CS_Disabled] = res0.value("disabled");
        arr[QM::CS_Normal_Checked] = res0.value("up2");
        arr[QM::CS_Hover_Checked] = res0.value("over2");
        arr[QM::CS_Pressed_Checked] = res0.value("down2");
        arr[QM::CS_Disabled_Checked] = res0.value("disabled2");

        return true;
    }

    void InitializeStateIndexes(int arr[]) {
        arr[QM::CS_Normal] = QM::CS_Normal;
        arr[QM::CS_Hover] = QM::CS_Normal;
        arr[QM::CS_Pressed] = QM::CS_Hover;
        arr[QM::CS_Disabled] = QM::CS_Normal;
        arr[QM::CS_Normal_Checked] = QM::CS_Normal;
        arr[QM::CS_Hover_Checked] = QM::CS_Normal_Checked;
        arr[QM::CS_Pressed_Checked] = QM::CS_Hover_Checked;
        arr[QM::CS_Disabled_Checked] = QM::CS_Normal_Checked;
    }

    void UpdateStateIndex(int i, int arr[]) {
        switch (static_cast<QM::ClickState>(i)) {
            case QM::CS_Hover:
            case QM::CS_Disabled: {
                arr[i] = QM::CS_Normal;
                break;
            }
            case QM::CS_Pressed: {
                arr[i] = arr[QM::CS_Hover];
                break;
            }
            case QM::CS_Normal_Checked: {
                arr[i] = arr[QM::CS_Normal];
                break;
            }
            case QM::CS_Hover_Checked:
            case QM::CS_Disabled_Checked: {
                arr[i] = arr[QM::CS_Normal_Checked];
                break;
            }
            case QM::CS_Pressed_Checked: {
                arr[i] = arr[QM::CS_Hover_Checked];
                break;
            }
            default:
                break;
        }
    }

    void UpdateStateIndexes(int arr[]) {
        for (int i = 0; i < 8; ++i) {
            if (arr[i] == i)
                continue;
            UpdateStateIndex(i, arr);
        }
    }

}