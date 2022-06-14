#ifndef FILEHOLDER_H
#define FILEHOLDER_H

#include <QObject>

class FileHolder : public QObject {
    Q_OBJECT
public:
    explicit FileHolder(QObject *parent = nullptr);
    ~FileHolder();

signals:
};

#endif // FILEHOLDER_H
