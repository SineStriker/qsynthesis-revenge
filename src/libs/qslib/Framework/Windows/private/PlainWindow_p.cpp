#include "PlainWindow_p.h"

#include "factories/WindowHandleFactory.h"

#include <QCoreApplication>

namespace QsApi {

    PlainWindowPrivate::PlainWindowPrivate() {
    }

    PlainWindowPrivate::~PlainWindowPrivate() {
        if (winHandle) {
            delete winHandle;
            winHandle = nullptr;
        }
    }

    void PlainWindowPrivate::init() {
        Q_Q(PlainWindow);
        winHandle = WindowHandleFactory::create(q);
        if (winHandle) {
            winHandle->setup();
        }
        q->setWindowTitle(qAppName());
    }

}