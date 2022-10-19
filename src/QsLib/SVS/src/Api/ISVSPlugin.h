#ifndef ISVSPLUGIN_H
#define ISVSPLUGIN_H

#include <QLocale>
#include <QObject>

#include "qssvs_global.h"

class QSSVS_API ISVSPlugin : public QObject {
    Q_OBJECT
public:
    ISVSPlugin(QObject *parent = nullptr);
    ~ISVSPlugin();

    /**
     * @brief path: Plugin library absolute path
     */
    QString path;

    /**
     * @brief locale: Current locale when executing plugin
     *
     * Install translator at entry and remove before exit, using embedded
     * qm files is recommended.
     *
     */
    QLocale locale;
};

#endif // ISVSPLUGIN_H
