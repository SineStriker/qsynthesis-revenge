#include "QVoiceBank.h"
#include "SystemHelper.h"

QVoiceBank::QVoiceBank() {
}

QVoiceBank::~QVoiceBank() {
}

QString QVoiceBank::dirname() const {
    return m_dirname;
}

void QVoiceBank::setDirname(const QString &dirname) {
    m_dirname = dirname;

    reset();
    CharInfo.setDirname(dirname);
    ReadmeTxt.setDirname(dirname);
    PrefixMap.setDirname(dirname);
    OtoInis.clear();
}

void QVoiceBank::reset() {
    CharInfo.reset();
    ReadmeTxt.reset();
    PrefixMap.reset();
    OtoInis.clear();
}

void QVoiceBank::clear() {
    CharInfo.clear();
    ReadmeTxt.clear();
    PrefixMap.clear();
    OtoInis.clear();
}

bool QVoiceBank::load(bool *overload) {
    if (!Sys::isDirExist(m_dirname)) {
        return false;
    }

    if (overload) {
        *overload = false;
    }

    bool charLoad = CharInfo.load();
    bool readmeLoad = ReadmeTxt.load();
    bool mapLoad = PrefixMap.load();
    bool otoLoad = loadOtos(overload);

    Q_UNUSED(charLoad)

    return readmeLoad && mapLoad && otoLoad;
}

bool QVoiceBank::save() {
    if (!Sys::isDirExist(m_dirname)) {
        return false;
    }

    bool mapSave = PrefixMap.save();
    bool readmeSave = ReadmeTxt.save();
    bool charSave = CharInfo.save();
    bool otoSave = saveOtos();

    return mapSave && readmeSave && charSave && otoSave;
}

bool QVoiceBank::loadOtos(bool *overload) {
    QStringList dirs = Sys::FindRecursiveDirs(m_dirname, SUBDIR_MAX + 1);
    dirs.prepend(m_dirname);

    int i = 0;
    for (auto it = dirs.begin(); it != dirs.end(); ++it) {
        const auto &dir = *it;
        OtoInis.insert(dir, QOtoIni(dir));
        i++;
        if (i >= SUBDIR_MAX) {
            if (overload) {
                *overload = true;
            }
            break;
        }
    }

    bool otoLoad = true;
    for (auto it = OtoInis.begin(); it != OtoInis.end(); ++it) {
        otoLoad &= it->load();
    }
    return otoLoad;
}

bool QVoiceBank::saveOtos() {
    bool otoLoad = true;
    for (auto it = OtoInis.begin(); it != OtoInis.end(); ++it) {
        otoLoad &= it->save();
    }
    return otoLoad;
}
