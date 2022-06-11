#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QKeyEvent>
#include <QTranslator>

#include "BaseManager.h"
#include "Macros.h"

#define qData DataManager::instance()

// Always call this macro at the end of constructor
#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings();                                                                               \
    connect(qData, &DataManager::stringUpdated, this, &T::reloadStrings);

class DataManager : public BaseManager {
    Q_OBJECT
    Q_SINGLETON(DataManager)
public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

public:
    bool load() override;
    bool save() override;

    void reloadStrings();

public:
    // Translators
    bool translate(const QString &filename);
    void eliminate();

    void loadLocale(int index);

    int localeCount() const;
    QStringList localeNames() const;

    // Inherit UTAU
    static QString pluginsProfile();
    static QString voiceProfile();

    // Qs Special
    static QString configProfile();
    static QString fontsProfile();
    static QString themesProfile();
    static QString toolsProfile();

    static QString tempDir();
    static QString desktopDir();

    static QString settingConifgPath();
    static QString keyboardConfigPath();
    static QString recordPath(); // Substitution for setting.ini

public:
    static QString openFile(const QString &title, const QString &filter, const QString &flag,
                            QWidget *parent = nullptr);
    static QStringList openFiles(const QString &title, const QString &filter, const QString &flag,
                                 QWidget *parent = nullptr);
    static QString openDir(const QString &title, const QString &flag, QWidget *parent = nullptr);
    static QString saveFile(const QString &title, const QString &filename, const QString &filter,
                            const QString &flag, QWidget *parent = nullptr);

private:
    QSet<QTranslator *> Translators;

    static QMap<QString, QString> lastOpenPaths;

    static QString getLastOpenPath(const QString &type);
    static void saveLastOpenDir(const QString &type, const QString &path);

    // Global Variables
public:
    QString FontName;

    QString MainTitle;
    QString WindowTitle;
    QString ErrorTitle;
    QString UntitledFileName;
    QString UntitledPrefix;
    QString DeletedPrefix;

    QString fileManagerName;

    QString openFilter;            // *.ust
    QString saveFilter;            // *.ust
    QString importFilter;          // *.mid *.svp *.s5p *.vsqx *.vsq *.ustx
    QString appendFilter;          // *.mid *.ust *.svp *.s5p *.vsqx *.vsq *.ustx
    QString exportSelectionFilter; // *.mid *.ust *.svp *.vsqx
    QString exportTrackFilter;     // *.mid *.ust *.svp *.s5p *.vsqx
    QString exportOtoIniFilter;    // oto.ini
    QString imageFilter;           // *.bmp *.jpg *.jpeg *.png *.gif *.webp
    QString audioFilter;           // *.wav
    QString toolsFilter;           // *.exe

signals:
    void stringUpdated();
};

#endif // DATAMANAGER_H
