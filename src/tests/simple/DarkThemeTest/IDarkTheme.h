#ifndef CHORUSKIT_IDARKTHEME_H
#define CHORUSKIT_IDARKTHEME_H

#include <QObject>

class IDarkTheme : public QObject {
    Q_OBJECT

public:
    explicit IDarkTheme(QObject *parent = nullptr);
    ~IDarkTheme();

    virtual bool isDarkTheme() = 0;
    virtual const char *getOSName() = 0;

signals:
    void themeChanged(bool isDarkTheme);
};



#endif // CHORUSKIT_IDARKTHEME_H
