#ifndef COLORTHEME_H
#define COLORTHEME_H

#include <QColor>
#include <QFile>

#include "BaseManager.h"
#include "Macros.h"

#define qTheme ColorTheme::instance()

class ColorTheme : public BaseManager {
    Q_OBJECT
    Q_SINGLETON(ColorTheme)
public:
    explicit ColorTheme(QObject *parent = nullptr);
    ~ColorTheme();

public:
    bool load() override;
    bool save() override;

public:
    void loadTheme(int index);

    int themeCount() const;
    QStringList themeNames() const;

protected:
    static void reloadAppFont();
};

#endif // COLORTHEME_H
