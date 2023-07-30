#ifndef CHORUSKIT_MACDARKTHEME_H
#define CHORUSKIT_MACDARKTHEME_H

#include "IDarkTheme.h"
#include <QObject>

class MacDarkTheme : public IDarkTheme {
    Q_OBJECT

public:
    explicit MacDarkTheme(QObject *parent = nullptr);
    ~MacDarkTheme() override;

    bool isDarkTheme() override;
    const char *getOSName() override;

private:
    void emitThemeChanged();
};

#endif // CHORUSKIT_MACDARKTHEME_H