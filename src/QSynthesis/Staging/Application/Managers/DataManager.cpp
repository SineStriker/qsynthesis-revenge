#include "DataManager.h"
#include "DataManager_p.h"

#include "QUtauStrings.h"
#include "SystemHelper.h"

#include "Logs/CRecordHolder.h"
#include "Namespace.h"

#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

static const char Slash = '/';

static const QChar ZH_CN_UTFCODE[] = {0x7B80, 0x4F53, 0x4E2D, 0x6587};
static const QChar ZH_CHT_UTFCODE[] = {0x7E41, 0x9AD4, 0x4E2D, 0x6587};
static const QChar JA_JP_UTFCODE[] = {0x65E5, 0x672C, 0x8A9E};

#define Q_FROM_UTFCODE(Name)                                                                       \
    QString::fromRawData(Name##_UTFCODE, sizeof(Name##_UTFCODE) / sizeof(QChar))

Q_SINGLETON_DECLARE(DataManager)

DataManager::DataManager(QObject *parent) : DataManager(*new DataManagerPrivate(), parent) {
}

DataManager::~DataManager() {
}

bool DataManager::load() {
    Q_D(DataManager);

    // Create directories
    if (!Sys::isDirExist(d->appDataPath)) {
        if (!Sys::mkDir(d->appDataPath)) {
            QMessageBox::warning(nullptr, d->MainTitle,
                                 tr("Failed to make application data path!"));
            return false;
        }
    }
    if (!Sys::isDirExist(d->appTempPath)) {
        if (!Sys::mkDir(d->appTempPath)) {
            QMessageBox::warning(nullptr, d->MainTitle, tr("Failed to make temporary path!"));
            return false;
        }
    }

    // Load Record
    qRecord->setFilename(qData->getStandardPath(DataManager::Record));
    qRecord->load();

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
    localeLoad(qRecordCData.translationIndex);

    d->lastOpenPaths = qRecordData.lastOpenPaths;
    return true;
}

bool DataManager::save() {
    Q_D(DataManager);

    qRecordData.lastOpenPaths = d->lastOpenPaths;

    // Save Record
    if (QApplication::keyboardModifiers() != Qt::ControlModifier) {
        qRecord->save(); // Do not save if ctrl is pressed
    }

    return true;
}

void DataManager::reloadStrings() {
    Q_D(DataManager);

    d->WindowTitle = d->MainTitle + QString(" %1").arg(Qs::Version);
    d->UntitledPrefix = "*";

    d->ErrorTitle = tr("Error");
    d->UntitledFileName = tr("Untitled");
    d->DeletedPrefix = tr("(Deleted)");

#ifdef Q_OS_WINDOWS
    d->fileManagerName = tr("Explorer");
#elif defined(Q_OS_MAC)
    d->fileManagerName = tr("Finder");
#else
    d->fileManagerName = tr("File Manager");
#endif

#if defined(Q_OS_WINDOWS)
    d->projectFilter = tr("UTAU Sequence Text(*.ust);;All Files(*.*)");
    d->importFilter = tr("Standard MIDI Files(*.mid);;"
                         "Synthesizer V Files(*.svp *.s5p);;VOCALOID Files(*.vsqx *.vsq);;"
                         "OpenUtau Files(*.ustx);;All Files(*.*)");
    d->appendFilter = tr("Standard MIDI Files(*.mid);;UTAU Sequence Text(*.ust);;"
                         "Synthesizer V Files(*.svp *.s5p);;VOCALOID Files(*.vsqx *.vsq);;"
                         "OpenUtau Files(*.ustx);;All Files(*.*)");
    d->exportSelectionFilter =
        tr("Standard MIDI Files(*.mid);;UTAU Sequence Text(*.ust);;"
           "Synthesizer V Files(*.svp);;VOCALOID Files(*.vsqx);;All Files(*.*)");
    d->exportTrackFilter = tr("Standard MIDI Files(*.mid);;"
                              "Synthesizer V Files(*.svp);;VOCALOID Files(*.vsqx);;All Files(*.*)");
    d->exportOtoIniFilter = tr("Voice Configurations(oto.ini);;All Files(*.*)");
    d->imageFilter = tr("Image Files(*.bmp *.jpg *.jpeg *.png *.gif *.webp);;All Files(*.*)");

    d->audioFilter = tr("Audio Files(*.wav);;All Files(*.*)");
    d->toolsFilter = tr("Executable(*.exe);;All Files(*.*)");
#else
    d->projectFilter = tr("UTAU Sequence Text(*.ust);;All Files(*)");
    d->importFilter = tr("Standard MIDI Files(*.mid);;"
                         "Synthesizer V Files(*.svp *.s5p);;VOCALOID Files(*.vsqx *.vsq);;"
                         "OpenUtau Files(*.ustx);;All Files(*)");
    d->appendFilter = tr("Standard MIDI Files(*.mid);;UTAU Sequence Text(*.ust);;"
                         "Synthesizer V Files(*.svp *.s5p);;VOCALOID Files(*.vsqx *.vsq);;"
                         "OpenUtau Files(*.ustx);;All Files(*)");
    d->exportSelectionFilter =
        tr("Standard MIDI Files(*.mid);;UTAU Sequence Text(*.ust);;"
           "Synthesizer V Files(*.svp);;VOCALOID Files(*.vsqx);;All Files(*)");
    d->exportTrackFilter = tr("Standard MIDI Files(*.mid);;"
                              "Synthesizer V Files(*.svp);;VOCALOID Files(*.vsqx);;All Files(*)");
    d->exportOtoIniFilter = tr("Voice Configurations(oto.ini);;All Files(*)");
    d->imageFilter = tr("Image Files(*.bmp *.jpg *.jpeg *.png *.gif *.webp);;All Files(*)");

    d->audioFilter = tr("Audio Files(*.wav);;All Files(*)");
    d->toolsFilter = tr("Executable(*)");
#endif

    emit stringUpdated();
}

DataManager::DataManager(DataManagerPrivate &d, QObject *parent) : BaseManager(d, parent) {
    construct();

    d.init();
    reloadStrings();
}

void DataManager::localeLoad(int index) {
    Q_D(DataManager);
    d->eliminate();
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
    QStringList paths = QFileDialog::getOpenFileNames(parent, title, getLastOpenPath(flag), filter);
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
    Q_D(DataManager);
    auto it = d->lastOpenPaths.find(type);
    if (it == d->lastOpenPaths.end()) {
        it = d->lastOpenPaths.insert(type, desktopDir());
    } else if (!Sys::isDirExist(it.value())) {
        it.value() = desktopDir();
    }
    return it.value();
}

void DataManager::saveLastOpenDir(const QString &type, const QString &path) {
    Q_D(DataManager);
    d->lastOpenPaths.insert(type, Sys::PathFildDirPath(path));
}

QString DataManager::getFileFilter(DataManager::FileFilter f) const {
    Q_D(const DataManager);
    QString res;
    switch (f) {
    case ImportFile:
        res = d->importFilter;
        break;
    case AppendFile:
        res = d->appendFilter;
        break;
    case ExportSelection:
        res = d->exportSelectionFilter;
        break;
    case ExportTrack:
        res = d->exportSelectionFilter;
        break;
    case ExportOtoIni:
        res = d->exportOtoIniFilter;
        break;
    case ProjectFiles:
        res = d->projectFilter;
        break;
    case ImageFiles:
        res = d->imageFilter;
        break;
    case AudioFiles:
        res = d->audioFilter;
        break;
    case ExecutableFiles:
        res = d->toolsFilter;
        break;
    };
    return res;
}

QString DataManager::getStandardPath(DataManager::StandardPath s) const {
    Q_D(const DataManager);
    QString res;
    switch (s) {
    case Voice:
        break;
    case Plugins:
        break;
    case Extensions:
        break;
    case Fonts:
        res = Sys::appPath() + Slash + Qs::DIR_NAME_CONFIG_FONTS;
        break;
    case Record:
        res = d->appDataPath + Slash + Qs::FILE_NAME_RECORD_JSON;
        break;
    case SettingConfig:
        break;
    case KeyboardConfig:
        break;
    case AppData:
        res = d->appDataPath;
        break;
    case AppTemp:
        res = d->appTempPath;
        break;
    }
    return res;
}

QString DataManager::desktopDir() {
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}
