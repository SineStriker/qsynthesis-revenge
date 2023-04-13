#ifndef CHORUSKIT_IBUTTONBAR_H
#define CHORUSKIT_IBUTTONBAR_H

#include <QAbstractButton>

namespace Core {

    class IButtonBar : public QObject {
    public:
        virtual QAbstractButton *addButton(const QString &id) = 0;
        virtual void removeButton(const QString &id) = 0;
    };

}

#endif // CHORUSKIT_IBUTTONBAR_H
