#ifndef QDISPOSABLEQUEUE_H
#define QDISPOSABLEQUEUE_H

#include "QDisposable.h"

#include <QHash>

#include <functional>
#include <list>

class QSBASE_API QDisposableQueue {
public:
    QDisposableQueue() : autoLoad(false), autoSave(false), autoDelete(false){};
    ~QDisposableQueue();

    void push(QDisposable *p, const std::function<void(QDisposable *)> &cleaner = defaultDelete);
    QDisposable *pop();
    void remove(QDisposable *p);

    void load();
    void save();

    void dispose();

    bool autoLoad;
    bool autoSave;
    bool autoDelete;

protected:
    struct Item {
        QDisposable *p;
        std::function<void(QDisposable *)> cleaner;
    };

    std::list<Item> queue;
    QHash<QDisposable *, decltype(queue)::iterator> indexes;

    inline static void defaultDelete(QDisposable *p) {
        delete p;
    }
};

#endif // QDISPOSABLEQUEUE_H
