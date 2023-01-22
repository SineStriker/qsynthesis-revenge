#ifndef ISVSPLUGIN_H
#define ISVSPLUGIN_H

#include <QLocale>
#include <QObject>

#include "QsSVSGlobal.h"

class QSSVS_API ISVSPlugin : public QObject {
    Q_OBJECT
public:
    ISVSPlugin(QObject *parent = nullptr);
    ~ISVSPlugin();

    /**
     * @brief path: Plugin library absolute path
     */
    QString path;
};

#endif // ISVSPLUGIN_H
