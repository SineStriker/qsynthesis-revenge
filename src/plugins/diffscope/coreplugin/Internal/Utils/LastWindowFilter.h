#ifndef CHORUSKIT_LASTWINDOWFILTER_H
#define CHORUSKIT_LASTWINDOWFILTER_H

#include "ICore.h"

namespace Core::Internal {

    class LastWindowFilter : public QObject {
        Q_OBJECT
    public:
        explicit LastWindowFilter(QObject *parent = nullptr);
        ~LastWindowFilter();

        bool filtered;

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
    };
}



#endif // CHORUSKIT_LASTWINDOWFILTER_H
