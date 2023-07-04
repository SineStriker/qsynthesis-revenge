#include "QUtaPluginTxt.h"

#include "QMCodec.h"
#include "QMSettingFile.h"

#include "Config/UtaPluginText.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

using namespace Utau;

Q_CHARSET_DECLARE(QUtaPluginTxt)

QUtaPluginTxt::QUtaPluginTxt() {
    QUtaPluginTxt::reset();
}

QUtaPluginTxt::~QUtaPluginTxt() {
}

bool QUtaPluginTxt::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data;
    data = file.readAll();
    file.close();

    QTextStream in(&data);
    // Detect Code
    QTextCodec *codec = QMCodec::GetUtfCodec(data, &m_charsetDetermined);
    if (codec) {
        m_codec = codec;
    } else if (!m_codec) {
        m_codec = s_codeForDefault;
    }
    in.setCodec(m_codec);

    QStringList lines;
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }

    QMSettingSection section;
    section.fromLines(lines);

    QString *nameValue = section.valueOf(KEY_NAME_PLUGIN_NAME);
    if (nameValue) {
        name = *nameValue;
    }
    QString *executeValue = section.valueOf(KEY_NAME_PLUGIN_EXECUTABLE);
    if (nameValue) {
        execute = QDir::fromNativeSeparators(*executeValue);
    }
    QString *versionValue = section.valueOf(KEY_NAME_PLUGIN_UST_VERSION);
    if (versionValue) {
        ustVersion = *versionValue;
    }
    QString *shellValue = section.valueOf(KEY_NAME_PLUGIN_SHELL);
    if (shellValue && *shellValue == VALUE_NAME_PLUGIN_SHELL) {
        useShell = true;
    }
    QString *noteValue = section.valueOf(VALUE_NAME_PLUGIN_NOTE);
    if (noteValue && *noteValue == VALUE_NAME_PLUGIN_NOTE) {
        allNote = true;
    }
    QString *charsetValue = section.valueOf(KEY_NAME_PLUGIN_CHARSET);
    if (charsetValue && !charsetValue->isEmpty()) {
        charset = *charsetValue;
    }
    custom = section.unformattedLines();

    return true;
}

bool QUtaPluginTxt::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QMSettingSection section;
    section.addPair(KEY_NAME_PLUGIN_NAME, name);
    section.addPair(KEY_NAME_PLUGIN_EXECUTABLE, QDir::toNativeSeparators(execute));
    if (useShell) {
        section.addPair(KEY_NAME_PLUGIN_SHELL, VALUE_NAME_PLUGIN_SHELL);
    }
    if (allNote) {
        section.addPair(KEY_NAME_PLUGIN_NOTE, VALUE_NAME_PLUGIN_NOTE);
    }
    if (!ustVersion.isEmpty()) {
        section.addPair(KEY_NAME_PLUGIN_UST_VERSION, ustVersion);
    }
    if (!charset.isEmpty()) {
        section.addPair(KEY_NAME_PLUGIN_CHARSET, charset);
    }
    section.setUnformattedLines(custom);

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

void QUtaPluginTxt::reset() {
    useShell = false;
    allNote = false;

    charset.clear();
    name.clear();
    ustVersion.clear();
    execute.clear();
}

bool QUtaPluginTxt::isEmpty() const {
    return execute.isEmpty();
}
