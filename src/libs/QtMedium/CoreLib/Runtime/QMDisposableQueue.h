#ifndef QMDISPOSABLEQUEUE_H
#define QMDISPOSABLEQUEUE_H

#include "QMDisposable.h"

#include <QHash>

#include <functional>
#include <list>

class QMCORELIB_API QMDisposableQueue {
public:
    QMDisposableQueue() : autoLoad(false), autoSave(false), autoDelete(false){};
    ~QMDisposableQueue();

    void push(QMDisposable *p, const std::function<void(QMDisposable *)> &cleaner = defaultDelete);
    QMDisposable *pop();
    void remove(QMDisposable *p);

    void load();
    void save();

    void dispose();

    bool autoLoad;
    bool autoSave;
    bool autoDelete;

protected:
    struct Item {
        QMDisposable *p;
        std::function<void(QMDisposable *)> cleaner;
    };

    std::list<Item> queue;
    QHash<QMDisposable *, decltype(queue)::iterator> indexes;

    inline static void defaultDelete(QMDisposable *p) {
        delete p;
    }
};

#endif // QMDISPOSABLEQUEUE_H
