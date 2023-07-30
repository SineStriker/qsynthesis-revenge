#ifndef CHORUSKIT_DEFAULTDARKTHEME_H
#define CHORUSKIT_DEFAULTDARKTHEME_H

#include "IDarkTheme.h"
#include <QObject>

class DefaultDarkTheme : public IDarkTheme {
    Q_OBJECT

public:
    explicit DefaultDarkTheme(QObject *parent = nullptr);
    ~DefaultDarkTheme() override;

    bool isDarkTheme() override;
    const char *getOSName() override;

private:
    void emitThemeChanged();
};

#endif // CHORUSKIT_DEFAULTDARKTHEME_H
