#include "IStyleHelper.h"

#include <QChildEvent>
#include <QStyle>

#include <CMenu.h>

namespace Core {

    class IStyleHelperPrivate {
        Q_DECLARE_PUBLIC(IStyleHelper)
    public:
        IStyleHelperPrivate() {
        }

        void init() {
        }

        IStyleHelper *q_ptr;
    };

    class PopUpMenuWatcher : public QObject {
    public:
        explicit PopUpMenuWatcher(QWidget *parent = nullptr) : QObject(parent) {
            parent->installEventFilter(this);
        }

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override {
            if (obj == parent()) {
                switch (event->type()) {
                    case QEvent::ChildPolished: {
                        auto e = static_cast<QChildEvent *>(event);
                        auto child = e->child();
                        if (child->inherits("QMenu") && child->property("core-style").isNull()) {
                            auto menu = qobject_cast<QMenu *>(child);
                            menu->setProperty("core-style", true);
                            menu->style()->polish(menu);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            return QObject::eventFilter(obj, event);
        }
    };

    void IStyleHelper::autoPolishPopupMenu(QWidget *w) {
        new PopUpMenuWatcher(w);
    }

    QMenu *IStyleHelper::createPolishedMenu(QWidget *parent) {
        auto menu = new CMenu(parent);
        menu->setProperty("core-style", true);
        return menu;
    }

    IStyleHelper::IStyleHelper(QObject *parent) : QObject(parent) {
    }

    IStyleHelper::~IStyleHelper() {
    }

    IStyleHelper::IStyleHelper(IStyleHelperPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();
    }

} // Core