#include "QUPluginInfo.h"
#include "QSettingFile.h"
#include "QUtauStrings.h"

#include "SystemHelper.h"
#include "TextHelper.h"

#include "Namespace.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
using namespace Utau;
using namespace Qs;

Q_CHARSET_DECLARE(QUPluginInfo)

static const char Slash = '/';

QUPluginInfo::QUPluginInfo() {
    reset();
}

QUPluginInfo::QUPluginInfo(const QString &dirname) {
    setDirname(dirname);
}

QUPluginInfo::~QUPluginInfo() {
}

bool QUPluginInfo::loadCore(const QString &filename, bool *valid) {
    Q_UNUSED(valid);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data;
    data = file.readAll();
    file.close();

    QTextStream in(&data);
    if (!m_codec) {
        // Detect Code
        QTextCodec *codec = Txt::GetUtfCodec(data, &m_charsetDetermined);
        if (codec) {
            m_codec = codec;
        } else {
            m_codec = s_codeForDefault;
        }
    }
    in.setCodec(m_codec);

    QStringList lines;
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }

    QSettingSection section;
    section.fromLines(lines);

    QString *nameValue = section.valueOf(KEY_NAME_PLUGIN_NAME);
    if (nameValue) {
        m_name = *nameValue;
    }
    QString *executeValue = section.valueOf(KEY_NAME_PLUGIN_EXECUTABLE);
    if (nameValue) {
        m_execute = QDir::fromNativeSeparators(*executeValue);
    }
    QString *versionValue = section.valueOf(KEY_NAME_PLUGIN_UST_VERSION);
    if (versionValue) {
        m_ustVersion = *versionValue;
    }
    QString *shellValue = section.valueOf(KEY_NAME_PLUGIN_SHELL);
    if (shellValue && *shellValue == VALUE_NAME_PLUGIN_SHELL) {
        m_useShell = true;
    }
    QString *noteValue = section.valueOf(VALUE_NAME_PLUGIN_NOTE);
    if (noteValue && *noteValue == VALUE_NAME_PLUGIN_NOTE) {
        m_allNote = true;
    }
    QString *charsetValue = section.valueOf(KEY_NAME_PLUGIN_CHARSET);
    if (charsetValue && !charsetValue->isEmpty()) {
        m_charset = *charsetValue;
    }
    m_custom = section.unformattedLines();

    return true;
}

bool QUPluginInfo::saveCore(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QSettingSection section;
    section.addPair(KEY_NAME_PLUGIN_NAME, m_name);
    section.addPair(KEY_NAME_PLUGIN_EXECUTABLE, QDir::toNativeSeparators(m_execute));
    if (m_useShell) {
        section.addPair(KEY_NAME_PLUGIN_SHELL, VALUE_NAME_PLUGIN_SHELL);
    }
    if (m_allNote) {
        section.addPair(KEY_NAME_PLUGIN_NOTE, VALUE_NAME_PLUGIN_NOTE);
    }
    if (!m_ustVersion.isEmpty()) {
        section.addPair(KEY_NAME_PLUGIN_UST_VERSION, m_ustVersion);
    }
    if (!m_charset.isEmpty()) {
        section.addPair(KEY_NAME_PLUGIN_CHARSET, m_charset);
    }
    section.setUnformattedLines(m_custom);

    QStringList lines = section.toLines();

    QTextStream out(&file);
    if (!m_codec) {
        m_codec = s_codeForDefault;
    }
    out.setCodec(m_codec);
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        out << *it << Qt::endl;
    }

    file.close();
    return true;
}

bool QUPluginInfo::useShell() const {
    return m_useShell;
}

QString QUPluginInfo::exePath() const {
    return m_dirname + Slash + m_execute;
}

void QUPluginInfo::setExePath(const QString &path) {
    m_execute = path.mid(m_dirname.size());
}

QString QUPluginInfo::name() const {
    return m_name;
}

QString QUPluginInfo::ustVersion() const {
    return m_ustVersion;
}

void QUPluginInfo::resetCore() {
    m_useShell = false;
    m_ustVersion.clear();
    m_name.clear();
    m_execute.clear();

    m_charset.clear();

    m_allNote = false;
}

void QUPluginInfo::setUstVersion(const QString &ustVersion) {
    m_ustVersion = ustVersion;
}

void QUPluginInfo::setName(const QString &name) {
    m_name = name;
}

void QUPluginInfo::setCharset(const QString &charset) {
    m_charset = charset;
}

void QUPluginInfo::setAllNote(bool allNote) {
    m_allNote = allNote;
}

void QUPluginInfo::setUseShell(bool useShell) {
    m_useShell = useShell;
}

QStringList QUPluginInfo::custom() const {
    return m_custom;
}

void QUPluginInfo::setCustom(const QStringList &custom) {
    m_custom = custom;
}

QString QUPluginInfo::configName() const {
    return FILE_NAME_PLUGIN_TEXT;
}

QString QUPluginInfo::charset() const {
    return m_charset;
}

bool QUPluginInfo::allNote() const {
    return m_allNote;
}

bool QUPluginInfo::valid() const {
    return QFileInfo(exePath()).isFile();
}
