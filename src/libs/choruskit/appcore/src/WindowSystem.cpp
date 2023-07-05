#include "WindowSystem.h"
#include "WindowSystem_p.h"

#include "ILoader.h"
#include "IWindowAddOn_p.h"
#include "IWindow_p.h"

#include <QMBatch.h>
#include <QMLinq.h>
#include <QMSystem.h>
#include <QMView.h>

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopWidget>
#include <QPointer>
#include <QScreen>
#include <QSplitter>

namespace Core {

#define myWarning(func) (qWarning().nospace() << "Core::WindowSystem::" << (func) << "():").space()

    static const char settingCatalogC[] = "WindowSystem";

    static const char winGeometryGroupC[] = "WindowGeometry";

    static const char splitterSizesGroupC[] = "SplitterSizes";

    WindowGeometry WindowGeometry::fromObject(const QJsonObject &obj) {
        QRect winRect;
        winRect.setX(obj.value("x").toInt());
        winRect.setY(obj.value("y").toInt());
        winRect.setWidth(obj.value("width").toInt());
        winRect.setHeight(obj.value("height").toInt());

        bool isMax = obj.value("isMaximized").toBool();

        return {winRect, isMax};
    }

    QJsonObject WindowGeometry::toObject() const {
        QJsonObject obj;
        obj.insert("x", geometry.x());
        obj.insert("y", geometry.y());
        obj.insert("width", geometry.width());
        obj.insert("height", geometry.height());
        obj.insert("isMaximized", maximized);
        return obj;
    }

    SplitterSizes SplitterSizes::fromObject(const QJsonObject &obj) {
        return QMBatch::toIntList(QMBatch::arrayToStringList(obj.value("sizes").toArray()));
    }

    QJsonObject SplitterSizes::toObject() const {
        return {{"sizes", QJsonArray::fromStringList(QMLinq::Select<QString>(
                              sizes, [](int num) -> QString { return QString::number(num); }))}};
    }

    WindowSystemPrivate::WindowSystemPrivate() : q_ptr(nullptr) {
    }

    WindowSystemPrivate::~WindowSystemPrivate() {
    }

    void WindowSystemPrivate::init() {
        readSettings();
    }

    void WindowSystemPrivate::readSettings() {
        winGeometries.clear();

        auto settings = ILoader::instance()->settings();
        auto obj = settings->value(settingCatalogC).toObject();

        auto winPropsObj = obj.value(winGeometryGroupC).toObject();
        for (auto it = winPropsObj.begin(); it != winPropsObj.end(); ++it) {
            if (!it->isObject()) {
                continue;
            }
            winGeometries.insert(it.key(), WindowGeometry::fromObject(it->toObject()));
        }

        auto spPropsObj = obj.value(splitterSizesGroupC).toObject();
        for (auto it = spPropsObj.begin(); it != spPropsObj.end(); ++it) {
            if (!it->isObject()) {
                continue;
            }
            splitterSizes.insert(it.key(), SplitterSizes::fromObject(it->toObject()));
        }
    }

    void WindowSystemPrivate::saveSettings() const {
        auto settings = ILoader::instance()->settings();

        QJsonObject winPropsObj;
        for (auto it = winGeometries.begin(); it != winGeometries.end(); ++it) {
            winPropsObj.insert(it.key(), it->toObject());
        }

        QJsonObject spPropsObj;
        for (auto it = splitterSizes.begin(); it != splitterSizes.end(); ++it) {
            spPropsObj.insert(it.key(), it->toObject());
        }

        QJsonObject obj;
        obj.insert(winGeometryGroupC, winPropsObj);
        obj.insert(splitterSizesGroupC, spPropsObj);

        settings->insert(settingCatalogC, obj);
    }

    void WindowSystemPrivate::windowClosed(IWindow *iWin) {
        Q_Q(WindowSystem);

        emit q->windowDestroyed(iWin);

        iWindows.remove(iWin);
        windowMap.remove(iWin->window());

        if (!iWindows.isEmpty()) {
            return;
        }

        {
            bool lastWindowClosed = true;
            for (const auto &w : QApplication::topLevelWidgets()) {
                if (!w->isVisible() || w->parentWidget() || !w->testAttribute(Qt::WA_QuitOnClose) ||
                    w->property("choruskit_managed_window").toBool())
                    continue;

                lastWindowClosed = false;
                break;
            }

            if (!lastWindowClosed) {
                // Release quit control
                qApp->setQuitOnLastWindowClosed(true);
                return;
            }
        }

        QCloseEvent e;
        qApp->sendEvent(q, &e);
        if (e.isAccepted() && iWindows.isEmpty()) {
            // auto e2 = new QEvent(QEvent::Quit);
            // qApp->postEvent(qApp, e2);
            qApp->quit();
        }
    }

    static WindowSystem *m_instance = nullptr;

    WindowSystem::WindowSystem(QObject *parent) : WindowSystem(*new WindowSystemPrivate(), parent) {
    }

    WindowSystem::~WindowSystem() {
        Q_D(WindowSystem);

        d->saveSettings();

        m_instance = nullptr;
    }

    bool WindowSystem::addAddOn(const QString &id, const QMetaObject *metaObject,
                                const WindowSystem::AddOnFactory &factory) {
        Q_D(WindowSystem);
        if (!metaObject) {
            myWarning(__func__) << "trying to add null meta object";
            return false;
        }

        d->addOnFactories[id].append(metaObject, factory);
        return true;
    }

    bool WindowSystem::removeAddOn(const QString &id, const QMetaObject *metaObject) {
        Q_D(WindowSystem);
        if (!metaObject) {
            myWarning(__func__) << "trying to remove null factory";
            return false;
        }

        auto it = d->addOnFactories.find(id);
        if (it == d->addOnFactories.end() || !it->remove(metaObject)) {
            return false;
        }

        if (it->isEmpty()) {
            d->addOnFactories.erase(it);
        }
        return true;
    }

    IWindow *WindowSystem::findWindow(QWidget *window) const {
        Q_D(const WindowSystem);
        if (!window)
            return nullptr;
        window = window->window();
        return d->windowMap.value(window, nullptr);
    }

    int WindowSystem::count() const {
        Q_D(const WindowSystem);
        return d->iWindows.size();
    }

    QList<IWindow *> WindowSystem::windows() const {
        Q_D(const WindowSystem);
        return d->iWindows.values();
    }

    IWindow *WindowSystem::firstWindow() const {
        Q_D(const WindowSystem);
        return d->iWindows.isEmpty() ? nullptr : *d->iWindows.begin();
    }

    using WindowSizeTrimmers = QHash<QString, WindowSizeTrimmer *>;

    Q_GLOBAL_STATIC(WindowSizeTrimmers, winSizeTrimmers)

    class WindowSizeTrimmer : public QObject {
    public:
        WindowSizeTrimmer(const QString &id, QWidget *w) : QObject(w), id(id) {
            winSizeTrimmers->insert(id, this);

            widget = w;
            m_pos = w->pos();
            m_obsolete = false;
            w->installEventFilter(this);
        }

        ~WindowSizeTrimmer() {
            winSizeTrimmers->remove(id);
        };

        QPoint pos() const {
            return m_pos;
        }

        void makeObsolete() {
            m_obsolete = true;
            deleteLater();
        }

        bool obsolete() const {
            return m_obsolete;
        }

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override {
            switch (event->type()) {
                    // case QEvent ::WindowStateChange: {
                    //     auto e = static_cast<QWindowStateChangeEvent *>(event);
                    //     if ((e->oldState() & Qt::WindowMaximized) || !(widget->windowState() &
                    //     Qt::WindowMaximized))
                    //     {
                    //         break;
                    //     }
                    // }
                case QEvent::Move:
                    if (widget->isVisible()) {
                        makeObsolete();
                    }
                    break;
                default:
                    break;
            }
            return QObject::eventFilter(obj, event);
        }

        QPointer<QWidget> widget;
        QString id;
        QPoint m_pos;
        bool m_obsolete;
    };

    void WindowSystem::loadWindowGeometry(const QString &id, QWidget *w,
                                          const QSize &fallback) const {
        Q_D(const WindowSystem);

        auto winProp = d->winGeometries.value(id, {});

        const auto &winRect = winProp.geometry;
        const auto &isMax = winProp.maximized;

        bool isDialog = w->parentWidget() && (w->windowFlags() & Qt::Dialog);
        if (winRect.size().isEmpty() || isMax) {
            // Adjust sizes
            w->resize(fallback.isValid() ? fallback : (QApplication::desktop()->size() * 0.75));
            if (!isDialog) {
                QMView::centralizeWindow(w);
            }
            if (isMax) {
                w->showMaximized();
            }
        } else {
            if (isDialog)
                w->resize(winRect.size());
            else
                w->setGeometry(winRect);
        }


        double ratio = (w->screen()->logicalDotsPerInch() / QMOs::unitDpi());
        auto addTrimmer = [&](bool move) {
            if (!isDialog && !isMax) {
                if (move) {
                    QRect rect = w->geometry();
                    w->setGeometry(QRect(rect.topLeft() + QPoint(30, 30) * ratio, rect.size()));
                }

                new WindowSizeTrimmer(id, w);
            }
        };

        // Check trimmers
        auto trimmer = winSizeTrimmers->value(id, {});
        if (trimmer && !trimmer->obsolete()) {
            trimmer->makeObsolete();
            addTrimmer(true);
        } else {
            addTrimmer(false);
        }
    }

    void WindowSystem::saveWindowGeometry(const QString &id, QWidget *w) {
        Q_D(WindowSystem);
        d->winGeometries.insert(id, {w->geometry(), w->isMaximized()});
    }

    void WindowSystem::loadSplitterSizes(const QString &id, QSplitter *s,
                                         const QList<int> &fallback) const {
        Q_D(const WindowSystem);

        auto spProp = d->splitterSizes.value(id, {});
        if (spProp.sizes.size() != s->count()) {
            if (fallback.size() == s->count())
                s->setSizes(fallback);
        } else {
            s->setSizes(spProp.sizes);
        }
    }

    void WindowSystem::saveSplitterSizes(const QString &id, QSplitter *s) {
        Q_D(WindowSystem);
        d->splitterSizes.insert(id, {s->sizes()});
    }

    WindowSystem::WindowSystem(WindowSystemPrivate &d, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }
} // namespace Core
