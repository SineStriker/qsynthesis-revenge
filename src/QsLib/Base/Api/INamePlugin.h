#ifndef INAMEPLUGIN_H
#define INAMEPLUGIN_H

#include <QObject>

#include "QsGlobal.h"

class QSBASE_API INamePlugin : public QObject {
    Q_OBJECT
public:
    explicit INamePlugin(QObject *parent = nullptr);
    ~INamePlugin();

    /**
     * @brief path: Plugin library absolute path
     */
    QString path;

    virtual void setupPath(const QString &path);
};

#endif // INAMEPLUGIN_H