#include "QSvgUri.h"
#include "private/QMetaTypeUtils.h"

#include "private/CSvgIconEngine.h"

#include <QFileInfo>
#include <QMimeDatabase>

QSvgUri::QSvgUri() {
}

QSvgUri::QSvgUri(const QString &filename) : m_filename(filename) {
}

QSvgUri::QSvgUri(const QString &filename, const QColor &color)
    : m_filename(filename), m_currentColor(color.name()) {
}

QSvgUri::~QSvgUri() {
}

QStringList QSvgUri::toStringList() const {
    return {MetaFunctionName(),
            QString("%1,%2%3").arg(m_filename, QCssCustomValue_Svg_CurrentColor_Prefix,
                                   m_currentColor)};
}

QIcon QSvgUri::toIcon(int modes) const {
    if (!m_filename.isEmpty()) {
        QFileInfo info(m_filename);
        QString suffix = info.suffix();
#if QT_CONFIG(mimetype)
        if (suffix.isEmpty()) {
            suffix = QMimeDatabase()
                         .mimeTypeForFile(info)
                         .preferredSuffix(); // determination from contents
#endif                                       // mimetype
        }
        if (info.isFile() && !suffix.compare("svg", Qt::CaseInsensitive)) {
            QIcon icon(new CSvgIconEngine(m_currentColor));
            icon.addFile(m_filename);
            if (modes & Disabled) {
                icon.addFile(m_filename, {}, QIcon::Disabled);
            }
            if (modes & Active) {
                icon.addFile(m_filename, {}, QIcon::Active);
            }
            if (modes & Selected) {
                icon.addFile(m_filename, {}, QIcon::Selected);
            }
            return icon;
        }
    }
    return QIcon();
}

QString QSvgUri::filename() const {
    return m_filename;
}

void QSvgUri::setFilename(const QString &filename) {
    m_filename = filename;
}

QString QSvgUri::currentColor() const {
    return m_currentColor;
}

void QSvgUri::setCurrentColor(const QString &currentColor) {
    m_currentColor = currentColor;
}

bool QSvgUri::isNull() const {
    return m_filename.isEmpty();
}

QSvgUri::operator QIcon() const {
    return toIcon();
}

QSvgUri QSvgUri::fromStringList(const QStringList &stringList) {
    if (stringList.size() == 2 &&
        !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList content = stringList.back().split(',');
        if (!content.isEmpty()) {
            QString strFile = content.front().simplified();
            strFile.replace("\"", "");

            QSvgUri uri;
            uri.setFilename(strFile);
            if (content.size() > 1) {
                QString strData = content.at(1).simplified();
                QLatin1String prefix(QCssCustomValue_Svg_CurrentColor_Prefix);
                if (strData.startsWith(prefix, Qt::CaseInsensitive)) {
                    strData.remove(0, prefix.size());
                }
                uri.setCurrentColor(strData);
            }
            return uri;
        }
    }
    return QSvgUri();
}

QLatin1String QSvgUri::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_Svg_Url);
}

QDebug operator<<(QDebug debug, const QSvgUri &uri) {
    QStringList list = uri.toStringList();
    debug.noquote() << QString("QSvgUri(%1)").arg(list.back());
    return debug;
}
