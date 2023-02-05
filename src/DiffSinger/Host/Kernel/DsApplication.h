#ifndef DSAPPLICATION_H
#define DSAPPLICATION_H

#include <QApplication>

class DsApplicationPrivate;

class DsApplication : public QApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(DsApplication)
public:
    DsApplication(int &argc, char **argv);
    ~DsApplication();

protected:
    DsApplication(DsApplicationPrivate &d, int &argc, char **argv);

    QScopedPointer<DsApplicationPrivate> d_ptr;
};

#endif // DSAPPLICATION_H
