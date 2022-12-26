#ifndef ISTARTPAGE_H
#define ISTARTPAGE_H

#include <QObject>

#include "dsmedium_global.h"

class DSMEDIUM_API IStartPage : public QObject {
    Q_OBJECT
public:
    IStartPage(QObject *parent = nullptr);
    ~IStartPage();

public:
};

#define IStartPage_IID "DiffSinger.Plugin.StartPage"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IStartPage, IStartPage_IID)
QT_END_NAMESPACE

#endif // ISTARTPAGE_H
