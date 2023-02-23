#include "QMDisposableQueue.h"

QMDisposableQueue::~QMDisposableQueue() {
    dispose();
}

void QMDisposableQueue::push(QMDisposable *p, const std::function<void(QMDisposable *)> &cleaner) {
    indexes.insert(p, queue.insert(queue.end(), {p, cleaner}));
    if (autoLoad) {
        p->load();
    }
}

QMDisposable *QMDisposableQueue::pop() {
    auto item = queue.back();
    queue.pop_back();
    return item.p;
}

void QMDisposableQueue::remove(QMDisposable *p) {
    auto it = indexes.find(p);
    if (it == indexes.end()) {
        return;
    }
    queue.erase(it.value());
    indexes.erase(it);
}

void QMDisposableQueue::load() {
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        it->p->load();
    }
}

void QMDisposableQueue::save() {
    for (auto it = queue.rbegin(); it != queue.rend(); ++it) {
        it->p->save();
    }
}

void QMDisposableQueue::dispose() {
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