#include "QSvgIconEx.h"

#include "Image/CSvgIconEngine.h"
#include "private/QMetaTypeUtils.h"

#include <QDebug>
#include <QFile>

struct DataUrl {
    QString fmt;
    QString charset;
    QString data;
};

static DataUrl fromDataUrl(const QString &s) {
    QStringList content = s.split(';');
    if (content.size() > 1) {
        QString strForm = content.front().trimmed();
        QString strData = content.at(1).trimmed();
        QStringList strListForm = strForm.split(':');
        QStringList strListData = strData.split(',');
        if (strListForm.size() == 2 &&
            !strListForm.front().trimmed().compare(QLatin1String("data"), Qt::CaseInsensitive)) {
            DataUrl uri;
            QString strBytes;
            uri.fmt = strListForm.back().trimmed(); // Format
            if (strListData.size() == 2) {
                QString strCharset = strListData.front().trimmed();
                QLatin1String prefix("charset=");
                if (strCharset.startsWith(prefix, Qt::CaseInsensitive)) {
                    strCharset.remove(0, prefix.size());
                }
                uri.charset = strCharset;                 // Charset
                strBytes = strListData.at(1).trimmed();   // Bytes
            } else if (!strListData.isEmpty()) {
                strBytes = strListData.front().trimmed(); // Bytes
            }
            uri.data = strBytes;                          // Data
            return uri;
        }
    }

    return {};
}

static QByteArray parseDataUrl(const DataUrl &url) {
    if (url.data.isEmpty())
        return {};

    if (!url.charset.compare("utf-8", Qt::CaseInsensitive) || !url.charset.compare("utf8", Qt::CaseInsensitive)) {
        return url.data.toUtf8();
    }

    if (url.charset == "base64") {
        return QByteArray::fromBase64(url.data.toUtf8());
    }

    return {};
}

QSvgIconEx::QSvgIconEx() noexcept = default;

QSvgIconEx::QSvgIconEx(const QPixmap &pixmap) : QIcon(pixmap) {
}

QSvgIconEx::QSvgIconEx(const QIcon &other) : QIcon(other) {
}

QSvgIconEx::QSvgIconEx(const QString &fileName) : QIcon(fileName) {
}

QSvgIconEx::QSvgIconEx(QIconEngine *engine) : QIcon(engine) {
}

QSvgIconEx QSvgIconEx::fromStringList(const QStringList &stringList) {
    QMETATYPE_CHECK_FUNC(stringList, strData);

    QSvgIconEx res;
    QStringList valueList = QMetaTypeUtils::SplitStringByComma(strData);
    if (valueList.isEmpty())
        return {};

    // Avoid copy file data because it's a waste of memory
    QString files[8];
    if (!QMetaTypeUtils::ParseClickStateArgList(valueList.front().trimmed(), files, false)) {
        return {};
    }

    // Handle file contents
    QByteArray svgContents[8];
    for (int i = 0; i < 8; ++i) {
        const auto &item = files[i];
        if (item.isEmpty())
            continue;

        if (item.startsWith("data:")) {
            svgContents[i] = parseDataUrl(fromDataUrl(item));
        } else {
            QFile file(item);
            if (file.open(QIODevice::ReadOnly)) {
                svgContents[i] = file.readAll();
            }
        }
    }

    // Handle colors
    QString colors[8];
    if (valueList.size() > 1) {
        QMetaTypeUtils::ParseClickStateArgList(valueList.at(1).trimmed(), colors);
    } else {
        for (auto &c : colors)
            c = "transparent";
    }

    auto engine = new CSvgIconEngine();
    engine->setValues(svgContents, colors);

    return QSvgIconEx(engine);
}

const char *QSvgIconEx::metaFunctionName() {
    return "svg";
}

QDebug operator<<(QDebug debug, const QSvgIconEx &i) {
    QDebugStateSaver saver(debug);
    debug.resetFormat();
    debug.nospace();
    debug << "QSvgIconEx(";
    if (i.isNull()) {
        debug << "null";
    } else {
        if (!i.name().isEmpty())
            debug << i.name() << ',';
        debug << "availableSizes[normal,Off]=" << i.availableSizes() << ",cacheKey=" << Qt::showbase << Qt::hex
              << i.cacheKey() << Qt::dec << Qt::noshowbase;
    }
    debug << ')';
    return debug;
}