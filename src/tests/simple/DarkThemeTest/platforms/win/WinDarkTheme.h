#ifndef CHORUSKIT_WINDARKTHEME_H
#define CHORUSKIT_WINDARKTHEME_H

#include "IDarkTheme.h"
#include <QObject>

class WinDarkTheme : public IDarkTheme {
    Q_OBJECT

public:
    explicit WinDarkTheme(QObject *parent = nullptr);
    ~WinDarkTheme() override;

    bool isDarkTheme() override;
    const char *getOSName() override;

public:
    void emitThemeChanged();
};

#endif // CHORUSKIT_WINDARKTHEME_H
