#ifndef DATAMANAGERPRIVATE_H
#define DATAMANAGERPRIVATE_H

#include "BaseManager_p.h"
#include "DataManager.h"

class DataManagerPrivate : public BaseManagerPrivate {
    Q_DECLARE_PUBLIC(DataManager)
public:
    DataManagerPrivate();
    ~DataManagerPrivate();

    void init();

    // Standard Locations
    QString appDataPath;
    QString appTempPath;

    QMap<QString, QString> lastOpenPaths;

    // Global Used Strings
    QString MainTitle;
    QString WindowTitle;
    QString ErrorTitle;
    QString UntitledFileName;
    QString UntitledPrefix;
    QString DeletedPrefix;

    QString fileManagerName;

    QString projectFilter;         // *.ust
    QString importFilter;          // *.mid *.svp *.s5p *.vsqx *.vsq *.ustx
    QString appendFilter;          // *.mid *.ust *.svp *.s5p *.vsqx *.vsq *.ustx
    QString exportSelectionFilter; // *.mid *.ust *.svp *.vsqx
    QString exportTrackFilter;     // *.mid *.ust *.svp *.s5p *.vsqx
    QString exportOtoIniFilter;    // oto.ini
    QString imageFilter;           // *.bmp *.jpg *.jpeg *.png *.gif *.webp
    QString audioFilter;           // *.wav
    QString toolsFilter;           // *.exe

    // Translators
    bool translate(const QString &filename);
    void eliminate();

    QSet<QTranslator *> translators;
};

#endif // DATAMANAGERPRIVATE_H
