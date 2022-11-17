#include "HexGenerator.h"

#include <QRandomGenerator>
#include <QRegularExpression>

static const char _chars[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void HexGenerator::Reset() {
    _registered.clear();
}

bool HexGenerator::Register(const QString &code) {
    if (_registered.contains(code)) {
        return false;
    }
    _registered.insert(code);
    return true;
}

void HexGenerator::Unregister(const QString &code) {
    _registered.remove(code);
}

QString HexGenerator::Generate(int length) {
    uint maxAttempts = qMin(UINT_MAX, (uint) (1 << (length * 4 - (length > 2 ? 1 : 0))));

    for (uint attempt = 0; attempt < maxAttempts; ++attempt) {
        QString code = GenerateOnce(length);
        if (Register(code)) {
            return code;
        }
    }

    // Failed to generate a unique code with this length
    //  ({length}) after {maxAttempts} attempts.

    return QString();
}

QString HexGenerator::GenerateOnce(int length) {
    size_t hexLen = sizeof(_chars);
    QByteArray codeArr;
    for (int i = 0; i < length; ++i) {
        codeArr.append(_chars[QRandomGenerator::global()->bounded(1, hexLen)]);
    }
    return QString::fromLatin1(codeArr);
}

bool HexGenerator::IsValidFormat(const QString &code, int length) {
    if (code.isEmpty() || code.size() != length) {
        return false;
    }
    return QRegExp(R"(^[0-9a-f].*$)").exactMatch(code);
}
