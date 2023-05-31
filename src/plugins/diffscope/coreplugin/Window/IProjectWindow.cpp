#include "IProjectWindow.h"
#include "IProjectWindow_p.h"

#include <QDebug>
#include <QEvent>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

#include <CToolBar.h>
#include <CToolButton.h>

#include <QMDecoratorV2.h>
#include <QMView.h>

#include <CoreApi/ILoader.h>

#include "ICore.h"

namespace Core {

    static const char settingCatalogC[] = "IProjectWindow";

    static const char floatingPanelsGroupC[] = "FloatingPanels";

    static const char pianoKeyWidgetC[] = "PianoKeyWidget";

    IProjectWindowPrivate::IProjectWindowPrivate() {
        m_forceClose = false;
    }

    void IProjectWindowPrivate::init() {
        Q_Q(IProjectWindow);

        readSettings();

        m_doc = new DspxDocument(q);
        m_doc->setAutoRemoveLogDirectory(false);
    }

    void IProjectWindowPrivate::readSettings() {
        auto settings = ILoader::instance()->settings();
        auto obj = settings->value(settingCatalogC).toObject();
        auto floatingPanelsStateArr = obj.value(floatingPanelsGroupC).toArray();

        QJsonValue value;
        for (auto it = floatingPanelsStateArr.begin(); it != floatingPanelsStateArr.end(); ++it) {
            if (!it->isObject())
                continue;

            auto stateObj = it->toObject();
            value = stateObj.value("id");
            if (!value.isString())
                continue;

            QString id = value.toString();

            value = stateObj.value("isOpen");
            if (!value.isUndefined() && !value.isBool())
                continue;

            bool isOpen = value.toBool();

            floatingPanelsState.append({id, isOpen});
        }

        value = obj.value(pianoKeyWidgetC);
        currentPianoKeyWidget = value.isString() ? value.toString() : "uta.pianoKeys";
    }

    void IProjectWindowPrivate::saveSettings() const {
        auto settings = ILoader::instance()->settings();

        QJsonObject obj;

        QJsonArray floatingPanelsStateArr;
        for (const auto &item : qAsConst(floatingPanelsState)) {
            floatingPanelsStateArr.append(QJsonObject{
                {"id",     item.id    },
                {"isOpen", item.isOpen}
            });
        }
        obj.insert(floatingPanelsGroupC, floatingPanelsStateArr);
        obj.insert(pianoKeyWidgetC, currentPianoKeyWidget);

        settings->insert(settingCatalogC, obj);
    }

    void IProjectWindowPrivate::reloadStrings() {
    }

    void IProjectWindowPrivate::reloadMainToolbar() {
        Q_Q(IProjectWindow);
        auto items = q->actionItems();
        auto bar = q->mainToolbar();

        mainToolbarCtx->buildToolBarWithState(items, bar);
    }

    void IProjectWindowPrivate::setPianoKeyWidget(const QString &id) {
        if (currentPianoKeyWidget == id) {
            return;
        }

        auto fac = pianoKeyWidgets.value(id);
        if (!fac) {
            return;
        }
        currentPianoKeyWidget = id;

        auto org = m_projectWidget->pianoKeyWidget();
        if (org) {
            org->deleteLater();
        }
        m_projectWidget->setPianoKeyWidget(fac->create(nullptr));
    }

    void IProjectWindowPrivate::_q_documentChanged() {
        Q_Q(IProjectWindow);
        q->setWindowModified(m_doc->isModified());
        q->setWindowTitle(m_doc->displayName());
    }

    void IProjectWindowPrivate::_q_documentRaiseRequested() {
        Q_Q(IProjectWindow);
        QMView::bringWindowToForeground(q->window());
    }

    void IProjectWindowPrivate::_q_documentCloseRequested() {
        Q_Q(IProjectWindow);

        m_forceClose = true;
        q->window()->close();
    }

    DspxDocument *IProjectWindow::doc() const {
        Q_D(const IProjectWindow);
        return d->m_doc;
    }

    QToolBar *IProjectWindow::mainToolbar() const {
        Q_D(const IProjectWindow);
        return d->m_projectWidget->mainToolbar();
    }

    CDockFrame *Core::IProjectWindow::mainDock() const {
        Q_D(const IProjectWindow);
        return d->m_projectWidget->mainDock();
    }

    QAbstractButton *IProjectWindow::addFloatingPanel(const QString &id, QWidget *panel, QWidget *titleBar) {
        Q_D(IProjectWindow);

        // Add sign
        panel->setProperty("choruskit_checkable_id", id);

        // Set checked to false
        setGlobalAttribute(id, false, true);

        // Wrap
        auto bar = new Internal::FloatingTitleBar();
        bar->setTitleBar(titleBar);

        auto obj = new QObject(d);
        obj->setProperty("checked", false);

        connect(bar->foldButton(), &QAbstractButton::toggled, obj, [id, panel, this](bool checked) {
            panel->setVisible(checked); //
        });

        connect(bar->closeButton(), &QAbstractButton::clicked, obj, [id, this]() {
            setGlobalAttribute(id, false, true); //
        });

        connect(obj, &QObject::destroyed, obj, [panel, bar]() {
            panel->deleteLater();
            bar->deleteLater();
        });

        connect(this, &ObjectPool::globalAttributeChanged, obj,
                [id, obj, panel, bar, d](const QString &_id, const QVariant &var, const QVariant &orgVar) {
                    if (_id != id) {
                        return;
                    }

                    bool checked = var.toBool();
                    if (checked == obj->property("checked").toBool()) {
                        return;
                    }

                    if (checked) {
                        d->m_projectWidget->canvas()->addWidget(panel, bar);
                        bar->foldButton()->setChecked(true);
                    } else {
                        d->m_projectWidget->canvas()->removeWidget(panel);
                    }

                    obj->setProperty("checked", checked);
                });

        d->floatingPanels.insert(id, {id, panel, bar, obj});

        return bar->titleButton();
    }

    QAbstractButton *IProjectWindow::addToolWindowPanel(const QString &id, int preferredEdge, QWidget *panel) {
        return nullptr;
    }

    void IProjectWindow::addPianoKeyWidget(const QString &id, IPianoKeyWidgetFactory *factory) {
        Q_D(IProjectWindow);
        if (!factory) {
            qWarning() << "Core::IProjectWindow::addPianoKeyWidget(): trying to add null factory";
            return;
        }
        if (d->pianoKeyWidgets.contains(id)) {
            qWarning() << "Core::IProjectWindow::addPianoKeyWidget(): trying to add duplicated factory:" << id;
            return;
        }
        d->pianoKeyWidgets.insert(id, factory);
    }

    IProjectWindow::IProjectWindow(QObject *parent) : IProjectWindow(*new IProjectWindowPrivate(), parent) {
    }

    IProjectWindow::~IProjectWindow() {
        Q_D(IProjectWindow);
        d->saveSettings();

        // Remove piano key widget factories
        qDeleteAll(d->pianoKeyWidgets);
    }

    void IProjectWindow::setupWindow() {
        Q_D(IProjectWindow);

        ICoreWindow::setupWindow();

        auto win = window();
        win->setObjectName("project-window");
        win->setAcceptDrops(true);

        d->mainToolbarCtx = ICore::instance()->actionSystem()->context("project.MainToolbar");

        auto &centralWidget = d->m_projectWidget = new Internal::ProjectWidget();
        setCentralWidget(centralWidget);
        d->cp->setParent(centralWidget);

        // Close event
        connect(d->m_doc, &IDocument::changed, d, &IProjectWindowPrivate::_q_documentChanged);
        connect(d->m_doc, &IDocument::raiseRequested, d, &IProjectWindowPrivate::_q_documentRaiseRequested);
        connect(d->m_doc, &IDocument::closeRequested, d, &IProjectWindowPrivate::_q_documentCloseRequested);
    }

    void IProjectWindow::windowAddOnsFinished() {
        Q_D(IProjectWindow);

        ICoreWindow::windowAddOnsFinished();

        connect(d->mainToolbarCtx, &ActionContext::stateChanged, d, &IProjectWindowPrivate::reloadMainToolbar);
        d->reloadMainToolbar();

        auto win = window();

        qIDec->installLocale(this, _LOC(IProjectWindowPrivate, d));
        qIDec->installTheme(win, "core.ProjectWindow");

        // Restore floating panels
        for (const auto &item : qAsConst(d->floatingPanelsState)) {
            auto it = d->floatingPanels.find(item.id);
            if (it == d->floatingPanels.end())
                continue;
            setGlobalAttribute(item.id, true, true);
            it->titleBar->foldButton()->setChecked(item.isOpen);
        }

        // Restore piano key widget
        {
            QString id = d->currentPianoKeyWidget;
            d->currentPianoKeyWidget.clear();

            if (d->pianoKeyWidgets.isEmpty()) {
                d->pianoKeyWidgets.clear();
            } else if (!d->pianoKeyWidgets.contains(id)) {
                id = d->pianoKeyWidgets.begin().key();
            }
            d->setPianoKeyWidget(id);
        }

        ICore::instance()->windowSystem()->loadWindowGeometry(metaObject()->className(), win, {1200, 800});
    }

    void IProjectWindow::windowAboutToClose() {
        Q_D(IProjectWindow);

        // Windows should be all closed before quit signal arrives except a kill or interrupt signal
        // is emitted, only when this function is called the log should be removed expectedly.
        if (!d->m_doc->isModified() || d->m_forceClose) {
            d->m_doc->setAutoRemoveLogDirectory(true);
        }

        // Save floating panels state
        d->floatingPanelsState.clear();
        auto piano = d->m_projectWidget->canvas();
        for (int i = 0; i < piano->count(); ++i) {
            auto id = piano->widget(i)->property("choruskit_checkable_id").toString();
            auto it = d->floatingPanels.find(id);
            if (it == d->floatingPanels.end())
                continue;

            d->floatingPanelsState.append({
                id,
                it->titleBar->foldButton()->isChecked(),
            });
        }

        ICore::instance()->windowSystem()->saveWindowGeometry(metaObject()->className(), window());
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IProjectWindow::WindowTypeID(), parent) {
        d.init();
    }

}
