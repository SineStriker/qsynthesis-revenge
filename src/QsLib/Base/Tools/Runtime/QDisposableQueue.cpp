#include "QDisposableQueue.h"

QDisposableQueue::~QDisposableQueue() {
    dispose();
}

void QDisposableQueue::push(QDisposable *p, const std::function<void(QDisposable *)> &cleaner) {
    indexes.insert(p, queue.insert(queue.end(), {p, cleaner}));
    if (autoLoad) {
        p->load();
    }
}

QDisposable *QDisposableQueue::pop() {
    auto item = queue.back();
    queue.pop_back();
    return item.p;
}

void QDisposableQueue::remove(QDisposable *p) {
    auto it = indexes.find(p);
    if (it == indexes.end()) {
        return;
    }
    queue.erase(it.value());
    indexes.erase(it);
}

void QDisposableQueue::load() {
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        it->p->load();
    }
}

void QDisposableQueue::save() {
    for (auto it = queue.rbegin(); it != queue.rend(); ++it) {
        it->p->save();
    }
}

void QDisposableQueue::dispose() {
    if (autoSave) {
        save();
    }
    if (autoDelete) {
        for (auto it = queue.rbegin(); it != queue.rend(); ++it) {
            it->cleaner(it->p);
        }
    }

    queue.clear();
    indexes.clear();
}