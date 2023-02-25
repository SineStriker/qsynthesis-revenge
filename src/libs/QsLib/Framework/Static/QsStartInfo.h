#ifndef QSSTARTINFO_H
#define QSSTARTINFO_H

#include <QObject>

#include "QsFrameworkGlobal.h"
#include "QsNamespace.h"

class QsStartInfoPrivate;

class QSFRAMEWORK_API QsStartInfo : public QObject {
    Q_OBJECT
    Q_SINGLETON(QsStartInfo)
public:
    explicit QsStartInfo(QObject *parent = nullptr);
    ~QsStartInfo();

    bool AllowSecondary;

public:
    QList<Qs::CommandLineOptions> commandLineOptions() const;

    bool checkLoadInfo();

protected:
    QScopedPointer<QsStartInfoPrivate> d_ptr;
};

#endif // QSSTARTINFO_H
