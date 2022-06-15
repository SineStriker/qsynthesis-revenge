#ifndef CENTRALTAB_H
#define CENTRALTAB_H

#include <QMap>
#include <QWidget>

#include "BaseTab.h"
#include "Macros.h"

class CentralTabPrivate;

class CentralTab : public BaseTab {
    Q_OBJECT
    Q_UNIQUE_ID(CentralTab)
    Q_DECLARE_PRIVATE(CentralTab)
public:
    explicit CentralTab(QWidget *parent = nullptr);
    ~CentralTab();

public:
    enum Type {
        NoTab = 0x0,
        Tab = 0x1,

        Config = 0x2,
        Display = 0x4,
        Document = 0x8,
        Folder = 0x10,

        Welcome = 0x200 | Display | Tab,
        Tuning = 0x400 | Document | Tab,
        Voice = 0x800 | Folder | Tab,
        Keyboard = 0x1000 | Config | Tab,
        Setting = 0x2000 | Config | Tab,
    };

    virtual Type type() const;

public:
    QString filename() const;
    void setFilename(const QString &filename);

    bool isEdited() const;

protected:
    void setTabName(const QString &tabName) override final; // Set the title of tab
    void updateTabName() override final;

    virtual void setEdited(bool edited);
    virtual void filenameSet(const QString &filename); // Called after setFilename

public:
    virtual bool load();
    virtual bool save();
    virtual bool saveAs();
    virtual bool restore();

    virtual void undo();
    virtual void redo();
    virtual void selectAll();
    virtual void deselect();

    virtual void awake(); // Window Activate
    virtual void sleep(); // Window Deactivate

    virtual void enter(); // Tab Switch In
    virtual void leave(); // Tab Switch Out

    virtual void change();

    virtual bool acceptClose();

public:
    static QString TypeToString(Type type);

protected:
    CentralTab(CentralTabPrivate &d, QWidget *parent = nullptr);
};

#endif // CENTRALTAB_H
