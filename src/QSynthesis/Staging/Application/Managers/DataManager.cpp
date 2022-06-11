#include "DataManager.h"
#include "QUtauStrings.h"
#include "SystemHelper.h"

#include "Logs/CRecordHolder.h"
#include "Namespace.h"

#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>

const QChar ZH_CN_UTFCODE[] = {0x7B80, 0x4F53, 0x4E2D, 0x6587};
const QChar ZH_CHT_UTFCODE[] = {0x7E41, 0x9AD4, 0x4E2D, 0x6587};
const QChar JA_JP_UTFCODE[] = {0x65E5, 0x672C, 0x8A9E};

#define Q_FROM_UTFCODE(Name)                                                                       \
    QString::fromRawData(Name##_UTFCODE, sizeof(Name##_UTFCODE) / sizeof(QChar))

Q_SINGLETON_DECLARE(DataManager)

QMap<QString, QString> DataManager::lastOpenPaths;

DataManager::DataManager(QObject *parent) : BaseManager(parent) {
    construct();
    reloadStrings();
}

DataManager::~DataManager() {
}

bool DataManager::load() {
    if (qRecordCData.translationIndex < 0 || qRecordCData.translationIndex > localeCount()) {
        QLocale ql;
        switch (ql.country()) {
        case QLocale::China: {
            qRecordData.translationIndex = 1;
            break;
        }
        case QLocale::HongKong:
        case QLocale::Macau:
        case QLocale::Taiwan: {
            qRecordData.translationIndex = 2;
            break;
        }
        case QLocale::Japan: {
            qRecordData.translationIndex = 3;
            break;
        }
        default: {
            qRecordData.translationIndex = 0;
            break;
        }
        }
    }
    loadLocale(qRecordCData.translationIndex);

    lastOpenPaths = qRecordData.lastOpenPaths;
    return true;
}

bool DataManager::save() {
    qRecordData.lastOpenPaths = lastOpenPaths;
    return true;
}

void DataManager::reloadStrings() {
    MainTitle = "QSynthesis";
    WindowTitle = MainTitle + QString(" %1").arg(Qs::Version);
    UntitledPrefix = "*";

    ErrorTitle = tr("Error");
    UntitledFileName = tr("Untitled");
    DeletedPrefix = tr("(Deleted)");

#ifdef Q_OS_WINDOWS
    fileManagerName = tr("Explorer");
#elif defined(Q_OS_MAC)
    fileManagerName = tr("Finder");
#else
    fileManagerName = tr("File Manager");
#endif

#if defined(Q_OS_WINDOWS)
    openFilter = tr("UTAU Sequence Text(*.ust);;All Files(*.*)");
    saveFilter = tr("UTAU Sequence Text(*.ust);;All Files(*.*)");
    importFilter = tr("Standard MIDI Files(*.mid);;"
                      "Synthesizer V Files(*.svp *.s5p);;VOCALOID Files(*.vsqx *.vsq);;"
                      "OpenUtau Files(*.ustx);;All Files(*.*)");
    appendFilter = tr("Standard MIDI Files(*.mid);;UTAU Sequence Text(*.ust);;"
                      "Synthesizer V Files(*.svp *.s5p);;VOCALOID Files(*.vsqx *.vsq);;"
                      "OpenUtau Files(*.ustx);;All Files(*.*)");
    exportSelectionFilter =
        tr("Standard MIDI Files(*.mid);;UTAU Sequence Text(*.ust);;"
           "Synthesizer V Files(*.svp);;VOCALOID Files(*.vsqx);;All Files(*.*)");
    exportTrackFilter = tr("Standard MIDI Files(*.mid);;"
                           "Synthesizer V Files(*.svp);;VOCALOID Files(*.vsqx);;All Files(*.*)");
    exportOtoIniFilter = tr("Voice Configurations(oto.ini);;All Files(*.*)");
    imageFilter = tr("Image Files(*.bmp *.jpg *.jpeg *.png *.gif *.webp);;All Files(*.*)");

    audioFilter = tr("Audio Files(*.wav);;All Files(*.*)");
    toolsFilter = tr("Executable(*.exe);;All Files(*.*)");
#else
    openFilter = tr("UTAU Sequence Text(*.ust);;All Files(*)");
    saveFilter = tr("UTAU Sequence Text(*.ust);;All Files(*)");
    importFilter = tr("Standard MIDI Files(*.mid);;"
                      "Synthesizer V Files(*.svp *.s5p);;VOCALOID Files(*.vsqx *.vsq);;"
                      "OpenUtau Files(*.ustx);;All Files(*)");
    appendFilter = tr("Standard MIDI Files(*.mid);;UTAU Sequence Text(*.ust);;"
                      "Synthesizer V Files(*.svp *.s5p);;VOCALOID Files(*.vsqx *.vsq);;"
                      "OpenUtau Files(*.ustx);;All Files(*)");
    exportSelectionFilter = tr("Standard MIDI Files(*.mid);;UTAU Sequence Text(*.ust);;"
                               "Synthesizer V Files(*.svp);;VOCALOID Files(*.vsqx);;All Files(*)");
    exportTrackFilter = tr("Standard MIDI Files(*.mid);;"
                           "Synthesizer V Files(*.svp);;VOCALOID Files(*.vsqx);;All Files(*)");
    exportOtoIniFilter = tr("Voice Configurations(oto.ini);;All Files(*)");
    imageFilter = tr("Image Files(*.bmp *.jpg *.jpeg *.png *.gif *.webp);;All Files(*)");

    audioFilter = tr("Audio Files(*.wav);;All Files(*)");
    toolsFilter = tr("Executable(*)");
#endif

    emit stringUpdated();
}

bool DataManager::translate(const QString &filename) {
    QTranslator *t = new QTranslator(this);
    if (t->load(filename)) {
        qApp->installTranslator(t);
        Translators.insert(t);
        reloadStrings();
        return true;
    }
    delete t;
    return false;
}

void DataManager::eliminate() {
    for (auto it = Translators.begin(); it != Translators.end(); ++it) {
        auto t = *it;
        qApp->removeTranslator(t);
        delete t;
    }
    Translators.clear();
}

void DataManager::loadLocale(int index) {
    eliminate();
//    switch (index) {
//    case 0:
//        translate(":/translations/qsynthesis_en.qm");
//        break;
//    case 1:
//        translate(":/translations/qsynthesis_cn.qm");
//        break;
//    case 2:
//        translate(":/translations/qsynthesis_cht.qm");
//        break;
//    case 3:
//        translate(":/translations/qsynthesis_jp.qm");
//        break;
//    default:
//        break;
//    }
}

int DataManager::localeCount() const {
    return 4;
}

QStringList DataManager::localeNames() const {
    QStringList list = {
        "English",              // English
        Q_FROM_UTFCODE(ZH_CN),  // 简体中文
        Q_FROM_UTFCODE(ZH_CHT), // 繁體中文
        Q_FROM_UTFCODE(JA_JP),  // 日本語
    };
    return list;
}

QString DataManager::pluginsProfile() {
    return Sys::appPath() + Slash + Utau::DIR_NAME_PLUGIN;
}

QString DataManager::voiceProfile() {
    return Sys::appPath() + Slash + Utau::DIR_NAME_VOICE;
}

QString DataManager::configProfile() {
    return Sys::appPath() + Slash + Qs::DIR_NAME_CONFIG;
}

QString DataManager::fontsProfile() {
    return configProfile() + Slash + Qs::DIR_NAME_CONFIG_FONTS;
}

QString DataManager::themesProfile() {
    return configProfile() + Slash + Qs::DIR_NAME_CONFIG_THEMES;
}

QString DataManager::toolsProfile() {
    return configProfile() + Slash + Qs::DIR_NAME_CONFIG_TOOLS;
}

QString DataManager::tempDir() {
    return QDir::tempPath();
}

QString DataManager::desktopDir() {
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

QString DataManager::settingConifgPath() {
    return configProfile() + Slash + Qs::FILE_NAME_SETTING_JSON;
}

QString DataManager::keyboardConfigPath() {
    return configProfile() + Slash + Qs::FILE_NAME_KEYBOARD_JSON;
}

QString DataManager::recordPath() {
    return Sys::appPath() + Slash + Qs::FILE_NAME_RECORD_JSON;
}

QString DataManager::openFile(const QString &title, const QString &filter, const QString &flag,
                              QWidget *parent) {
    QString path = QFileDialog::getOpenFileName(parent, title, getLastOpenPath(flag), filter);
    if (!path.isEmpty()) {
        saveLastOpenDir(flag, path);
    }
    return path;
}

QStringList DataManager::openFiles(const QString &title, const QString &filter, const QString &flag,
                                   QWidget *parent) {
    QStringList paths =
        QFileDialog::getOpenFileNames(parent, title, getLastOpenPath(flag), filter);
    if (!paths.isEmpty()) {
        saveLastOpenDir(flag, paths.back());
    }
    return paths;
}

QString DataManager::openDir(const QString &title, const QString &flag, QWidget *parent) {
    QString path = QFileDialog::getExistingDirectory(parent, title, getLastOpenPath(flag));
    if (!path.isEmpty()) {
        saveLastOpenDir(flag, path);
    }
    return path;
}

QString DataManager::saveFile(const QString &title, const QString &filename, const QString &filter,
                              const QString &flag, QWidget *parent) {
    QFileInfo info(filename);
    if (info.isRelative() || !Sys::isDirExist(info.absolutePath())) {
        info.setFile(getLastOpenPath(flag) + Slash + info.fileName());
    }
    QString path =
        QFileDialog::getSaveFileName(parent, title, info.absoluteFilePath(), filter, nullptr);
    if (!path.isEmpty()) {
        saveLastOpenDir(flag, path);
    }
    return path;
}

QString DataManager::getLastOpenPath(const QString &type) {
    auto it = lastOpenPaths.find(type);
    if (it == lastOpenPaths.end()) {
        it = lastOpenPaths.insert(type, desktopDir());
    } else if (!Sys::isDirExist(it.value())) {
        it.value() = desktopDir();
    }
    return it.value();
}

void DataManager::saveLastOpenDir(const QString &type, const QString &path) {
    lastOpenPaths.insert(type, Sys::PathFildDirPath(path));
}
