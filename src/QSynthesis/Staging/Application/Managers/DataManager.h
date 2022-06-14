#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QKeyEvent>
#include <QTranslator>

#include "BaseManager.h"
#include "Macros.h"

class DataManagerPrivate;

#define qData DataManager::instance()

// Always call this macro at the end of constructor
#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings();                                                                               \
    connect(qData, &DataManager::stringUpdated, this, &T::reloadStrings);

class DataManager : public BaseManager {
    Q_OBJECT
    Q_SINGLETON(DataManager)
    Q_DECLARE_PRIVATE(DataManager)
public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

public:
    bool load() override;
    bool save() override;

    void reloadStrings();

    // Global Variables
public:
    void localeLoad(int index);
    int localeCount() const;
    QStringList localeNames() const;

public:
    QString openFile(const QString &title, const QString &filter, const QString &flag,
                     QWidget *parent = nullptr);
    QStringList openFiles(const QString &title, const QString &filter, const QString &flag,
                          QWidget *parent = nullptr);
    QString openDir(const QString &title, const QString &flag, QWidget *parent = nullptr);
    QString saveFile(const QString &title, const QString &filename, const QString &filter,
                     const QString &flag, QWidget *parent = nullptr);

public:
    QString getLastOpenPath(const QString &type);
    void saveLastOpenDir(const QString &type, const QString &path);

    enum FileFilter {
        ImportFile,
        AppendFile,
        ExportSelection,
        ExportTrack,
        ExportOtoIni,
        ProjectFiles,
        ImageFiles,
        AudioFiles,
        ExecutableFiles,
    };

    QString getFileFilter(FileFilter f) const;

    enum StandardPath {
        Voice,
        Plugins,
        Extensions,
        Fonts,
        Record,
        SettingConfig,
        KeyboardConfig,
        AppData,
        AppTemp,
    };

    QString getStandardPath(StandardPath s) const;

    static QString desktopDir();

    QString mainTitle() const;
    QString windowTitle() const;
    QString errorTitle() const;
    QString untitledFileName() const;
    QString untitledPrefix() const;
    QString deletedPrefix() const;
    QString fileManagerName() const;

protected:
    DataManager(DataManagerPrivate &d, QObject *parent = nullptr);

signals:
    void stringUpdated();
};

#endif // DATAMANAGER_H
