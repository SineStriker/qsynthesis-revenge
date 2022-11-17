#ifndef LOCALDECORATOR_H
#define LOCALDECORATOR_H

#include <QHash>
#include <QObject>

#include "qsframework_global.h"

class QSFRAMEWORK_API LocalDecorator : public QObject {
    Q_OBJECT
public:
    explicit LocalDecorator(QObject *parent = nullptr);
    ~LocalDecorator();

    bool addTheme(const QString &filename);
    void removeThemes();

    QString stylesheets() const;

    QStringList stylesheetList;

signals:
};

#endif // LOCALDECORATOR_H
