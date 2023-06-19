#include "QMSettingFile.h"

#include <QFile>
#include <QMap>
#include <QTextStream>

#include <cstring>

const char QMSettingSection::KEY_VALUE_SEPARATOR[] = "=";

const char QMSettingFile::SECTION_BEGIN_MARK[] = "[";
const char QMSettingFile::SECTION_END_MARK[] = "]";

// Section
QMSettingSection::QMSettingSection() {
}

QMSettingSection::QMSettingSection(const QString &name) {
    setSectionName(name);
}

QMSettingSection::~QMSettingSection() {
}

void QMSettingSection::fromLines(const QStringList &list) {
    QString line;
    int eq;

    QString key, value;
    auto it = list.begin();
    while (it != list.end()) {
        line = *it;
        ++it;

        if (line.isEmpty()) {
            continue;
        }
        eq = line.indexOf(QMSettingSection::KEY_VALUE_SEPARATOR);
        if (eq > 0) {
            key = line.left(eq);
            value = line.mid(eq + 1);
            addPair(key, value);
        } else {
            unformatted.append(line);
        }
    }
}

QStringList QMSettingSection::toLines() const {
    QStringList list;
    const QList<QPair<QString, QString>> &pairs = toPairList();
    QString line;
    for (int i = 0; i < pairs.size(); ++i) {
        QPair<QString, QString> pair = pairs.at(i);
        line = pair.first + QMSettingSection::KEY_VALUE_SEPARATOR + pair.second;
        list.append(line);
    }
    for (int i = 0; i < unformatted.size(); ++i) {
        list.append(unformatted.at(i));
    }
    return list;
}

QString QMSettingSection::sectionName() const {
    return name;
}

void QMSettingSection::setSectionName(const QString &name) {
    this->name = name;
}

bool QMSettingSection::isArray(QString *prefix) const {
    if (map.isEmpty()) {
        return false;
    }

    QString curPrefix, lastPrefix;
    for (int i = 0; i < keys.size(); ++i) {
        curPrefix = keys.at(i);
        while (!curPrefix.isEmpty() && curPrefix.back().isDigit()) {
            curPrefix = curPrefix.mid(0, curPrefix.size() - 1);
        }

        if (i != 0 && curPrefix != lastPrefix) {
            return false;
        }
        lastPrefix = curPrefix;
    }
    if (prefix) {
        *prefix = curPrefix;
    }

    return true;
}

QStringList QMSettingSection::toArray() const {
    QStringList list;
    for (const auto &key : keys) {
        list.append(map[key]);
    }
    return list;
}

QList<QPair<QString, QString>> QMSettingSection::toPairList() const {
    QList<QPair<QString, QString>> list;
    for (const auto &key : keys) {
        list.append(qMakePair(key, map[key]));
    }
    return list;
}

const QString *QMSettingSection::valueOf(const QString &key) const {
    auto it = map.find(key);
    if (it == map.end()) {
        return nullptr;
    }
    return &it.value();
}

QString *QMSettingSection::valueOf(const QString &key) {
    auto it = map.find(key);
    if (it == map.end()) {
        return nullptr;
    }
    return &it.value();
}

bool QMSettingSection::containsKey(const QString &key) const {
    return map.contains(key);
}

bool QMSettingSection::isEmpty() const {
    return keys.isEmpty();
}

bool QMSettingSection::addPair(const QString &key, const QString &value) {
    if (map.contains(key)) {
        return false;
    }
    map.insert(key, value);
    keys.append(key);
    return true;
}

bool QMSettingSection::addPair(const QString &key, int value) {
    return addPair(key, QString::number(value));
}

bool QMSettingSection::addPair(const QString &key, double value) {
    return addPair(key, QString::number(value));
}

bool QMSettingSection::removePair(const QString &key) {
    auto it = map.find(key);
    if (it == map.end()) {
        return false;
    }
    map.erase(it);
    keys.removeOne(key);
    return true;
}

void QMSettingSection::clear() {
    map.clear();
    keys.clear();
}

void QMSettingSection::clearAll() {
    name.clear();
    clear();
    clearUnformattedLines();
}

QStringList QMSettingSection::unformattedLines() const {
    return unformatted;
}

void QMSettingSection::setUnformattedLines(const QStringList &unformatted) {
    this->unformatted = unformatted;
}

void QMSettingSection::clearUnformattedLines() {
    unformatted.clear();
}

// File
QMSettingFile::QMSettingFile() {
    m_codec = QTextCodec::codecForLocale();
    m_breakAfterSection = false;
}

QMSettingFile::~QMSettingFile() {
}

bool QMSettingFile::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 0;
    }

    QStringList list;

    QTextStream in(&file);
    in.setCodec(m_codec);
    while (!in.atEnd()) {
        list.append(in.readLine());
    }

    file.close();

    fromLines(list);
    return true;
}

bool QMSettingFile::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
        return 0;
    }

    QStringList list = toLines();

    QTextStream out(&file);
    out.setCodec(m_codec);
    for (auto it = list.begin(); it != list.end(); ++it) {
        out << *it << Qt::endl;
    }

    file.close();
    return true;
}

void QMSettingFile::fromLines(const QStringList &list) {
    QString line;

    QString name;
    Section section;
    QStringList lines;

    bool toAdd = false;
    auto addSec = [&]() {
        if (toAdd) {
            section.fromLines(lines);
            addSection(section);
            section.clearAll();
            lines.clear();
        }
    };

    auto it = list.begin();
    while (it != list.end()) {
        line = *it;
        ++it;

        if (line.isEmpty()) {
            continue;
        }

        if (parseSectionName(line, name)) {
            addSec();
            section.setSectionName(name);
            toAdd = true;
        } else {
            lines.append(line);
        }
    }

    // At end
    addSec();
}

QStringList QMSettingFile::toLines() const {
    QStringList list;
    QList<Section> sections = this->sections();

    for (int i = 0; i < sections.size(); ++i) {
        list.append(SECTION_BEGIN_MARK + sections.at(i).sectionName() + SECTION_END_MARK);
        list.append(sections.at(i).toLines());
        if (m_breakAfterSection) {
            list.append("");
        }
    }

    return list;
}

QMSettingFile::Section *QMSettingFile::sectionOf(const QString &name) {
    auto it = map.find(name);
    if (it == map.end()) {
        return nullptr;
    }
    return &it.value();
}

QList<QMSettingFile::Section> QMSettingFile::sections() const {
    QList<Section> list;
    for (int i = 0; i < names.size(); ++i) {
        list.append(map[names.at(i)]);
    }
    return list;
}

bool QMSettingFile::contains(const QString &name) const {
    return map.contains(name);
}

bool QMSettingFile::isEmpty() const {
    return names.isEmpty();
}

bool QMSettingFile::addPairsSection(const QString &name, const QList<QPair<QString, QString>> &pairs) {
    if (contains(name)) {
        return false;
    }
    Section section(name);
    for (int i = 0; i < pairs.size(); ++i) {
        section.addPair(pairs.at(i).first, pairs.at(i).second);
    }
    addSection(section);
    return true;
}

bool QMSettingFile::addArraySection(const QString &name, const QString &prefix, const QStringList &values, int begin) {
    if (contains(name)) {
        return false;
    }
    Section section(name);
    for (int i = 0; i < values.size(); ++i) {
        section.addPair(prefix + QString::number(begin + i), values.at(i));
    }
    addSection(section);
    return true;
}

void QMSettingFile::addSection(const QMSettingFile::Section &section) {
    map.insert(section.sectionName(), section);
    names.append(section.sectionName());
}

bool QMSettingFile::removeSection(const QString &name) {
    auto it = map.find(name);
    if (it == map.end()) {
        return false;
    }

    map.erase(it);
    names.removeOne(name);
    return true;
}

void QMSettingFile::clear() {
    map.clear();
    names.clear();
}

QTextCodec *QMSettingFile::codec() const {
    return m_codec;
}

void QMSettingFile::setCodec(QTextCodec *codec) {
    m_codec = codec;
}

bool QMSettingFile::breakAfterSection() const {
    return m_breakAfterSection;
}

void QMSettingFile::setBreakAfterSection(bool breakAfterSection) {
    m_breakAfterSection = breakAfterSection;
}

bool QMSettingFile::parseSectionName(const QString &oName, QString &oResult) {
    if (oName.startsWith(SECTION_BEGIN_MARK) && oName.endsWith(SECTION_END_MARK)) {
        oResult = oName.mid(int(strlen(SECTION_BEGIN_MARK)),
                            oName.size() - int(strlen(SECTION_BEGIN_MARK)) - int(strlen(SECTION_END_MARK)));
        return true;
    } else {
        return false;
    }
}
