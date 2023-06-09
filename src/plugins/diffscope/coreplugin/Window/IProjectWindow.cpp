#include "IProjectWindow.h"
#include "IProjectWindow_p.h"

#include <QDebug>
#include <QEvent>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QMetaEnum>
#include <QVBoxLayout>

#include <CToolBar.h>
#include <CToolButton.h>

#include <QMDecoratorV2.h>
#include <QMView.h>

#include <CoreApi/ILoader.h>

#include "ICore.h"

#include "Internal/Widgets/PianoKeyWidget.h"
#include "PianoRoll/PianoRoll_p.h"

namespace Core {

    static const char mainDockSettingCatalogC[] = "IProjectWindow/MainDock";

    IProjectWindowPrivate::IProjectWindowPrivate() {
        m_forceClose = false;
    }

    void IProjectWindowPrivate::init() {
        Q_Q(IProjectWindow);

        m_doc = new DspxDocument(q);
        m_doc->setAutoRemoveLogDirectory(false);
    }

    void IProjectWindowPrivate::reloadStrings() {
    }

    void IProjectWindowPrivate::reloadMainToolbar() {
        Q_Q(IProjectWindow);
        auto items = q->actionItems();
        auto bar = q->mainToolbar();

        mainToolbarCtx->buildToolBarWithState(items, bar);
    }

    static const int corners[8][2] = {
        {Qt::LeftEdge,   QM::Primary  },
        {Qt::LeftEdge,   QM::Secondary},
        {Qt::TopEdge,    QM::Primary  },
        {Qt::TopEdge,    QM::Secondary},
        {Qt::RightEdge,  QM::Primary  },
        {Qt::RightEdge,  QM::Secondary},
        {Qt::BottomEdge, QM::Primary  },
        {Qt::BottomEdge, QM::Secondary},
    };

    void IProjectWindowPrivate::saveMainDockState() const {
        auto settings = ILoader::instance()->settings();

        QJsonArray arr;
        for (const auto &corner : qAsConst(corners)) {
            QJsonArray arr0;
            for (const auto &card :
                 m_frame->widgets(static_cast<Qt::Edge>(corner[0]), static_cast<QM::Priority>(corner[1]))) {
                if (card->objectName().isEmpty())
                    continue;
                arr0.append(QJsonObject{
                    {"name",     card->objectName()                                                     },
                    {"checked",  card->isChecked()                                                      },
                    {"viewMode", QMetaEnum::fromType<CDockCard::ViewMode>().valueToKey(card->viewMode())},
                });
            }
            arr.append(arr0);
        }

        settings->insert(mainDockSettingCatalogC, arr);
    }

    void IProjectWindowPrivate::restoreMainDockState() const {
        auto settings = ILoader::instance()->settings();
        auto arr = settings->value(mainDockSettingCatalogC).toArray();

        QHash<QString, CDockCard *> cardMap;
        for (const auto &corner : qAsConst(corners)) {
            for (const auto &card :
                 m_frame->widgets(static_cast<Qt::Edge>(corner[0]), static_cast<QM::Priority>(corner[1]))) {
                if (card->objectName().isEmpty()) {
                    continue;
                }
                cardMap.insert(card->objectName(), card);
            }
        }

        QJsonValue value;
        for (int i = 0; i < qMin(arr.size(), 8); ++i) {
            const auto &val = arr.at(i);
            if (!val.isArray())
                continue;

            for (const auto &item : val.toArray()) {
                if (!item.isObject())
                    continue;

                auto obj = item.toObject();
                value = obj.value("name");
                if (value.isUndefined() || !value.isString())
                    continue;

                auto card = cardMap.value(value.toString());
                if (!card)
                    continue;

                const auto &corner = corners[i];
                m_frame->moveWidget(card, static_cast<Qt::Edge>(corner[0]), static_cast<QM::Priority>(corner[1]));

                value = obj.value("checked");
                if (value.isBool())
                    card->setChecked(value.toBool());

                value = obj.value("viewMode");
                if (value.isString()) {
                    auto viewMode = QMetaEnum::fromType<CDockCard::ViewMode>().keyToValue(value.toString().toLatin1());
                    if (viewMode >= 0) {
                        card->setViewMode(static_cast<CDockCard::ViewMode>(viewMode));
                    }
                }
            }
        }
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
        q->setProperty("choruskit_show_home", true);
        q->window()->close();
    }

    DspxDocument *IProjectWindow::doc() const {
        Q_D(const IProjectWindow);
        return d->m_doc;
    }

    QToolBar *IProjectWindow::mainToolbar() const {
        Q_D(const IProjectWindow);
        return d->m_toolbar;
    }

    CDockFrame *Core::IProjectWindow::mainDock() const {
        Q_D(const IProjectWindow);
        return d->m_frame;
    }

    PianoRoll *IProjectWindow::pianoRoll() const {
        Q_D(const IProjectWindow);
        return d->m_pianoRoll;
    }

    IProjectWindow::IProjectWindow(QObject *parent) : IProjectWindow(*new IProjectWindowPrivate(), parent) {
    }

    IProjectWindow::~IProjectWindow() {
        Q_D(IProjectWindow);
    }

    void IProjectWindow::setupWindow() {
        Q_D(IProjectWindow);

        ICoreWindow::setupWindow();

        auto win = window();
        win->setObjectName("project-window");
        win->setAcceptDrops(true);

        d->mainToolbarCtx = ICore::instance()->actionSystem()->context("project.MainToolbar");
        if (!d->mainToolbarCtx) {
            ICore::fatalError(win, tr("Failed to create main toolbar."));
        }

        auto &toolbar = d->m_toolbar;
        toolbar = new CToolBar();
        toolbar->setObjectName("main-toolbar");
        toolbar->setMovable(false);
        toolbar->setFloatable(false);
        toolbar->setOrientation(Qt::Horizontal);
        toolbar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        auto &frame = d->m_frame;
        frame = new CDockFrame();
        frame->setObjectName("main-dock");
        connect(frame, &CDockFrame::cardAdded, this, [this](QAbstractButton *card) {
            card->setProperty("type", "main-dock-card"); //
        });

        auto &layout = d->m_layout;
        layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(toolbar);
        layout->addWidget(frame, 1);

        auto &centralWidget = d->m_centralWidget;
        centralWidget = new QWidget();
        centralWidget->setAttribute(Qt::WA_StyledBackground);
        centralWidget->setLayout(layout);

        auto &m_pianoRoll = d->m_pianoRoll;
        m_pianoRoll = new PianoRoll();
        frame->setWidget(m_pianoRoll);

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

        // Restore dock
        d->restoreMainDockState();

        // Restore piano roll state
        d->m_pianoRoll->d_func()->readSettings();

        ICore::instance()->windowSystem()->loadWindowGeometry(metaObject()->className(), win, {1200, 800});
    }

    void IProjectWindow::windowAboutToClose() {
        Q_D(IProjectWindow);

        // Windows should be all closed before quit signal arrives except a kill or interrupt signal
        // is emitted, only when this function is called the log should be removed expectedly.
        if (!d->m_doc->isModified() || d->m_forceClose) {
            d->m_doc->setAutoRemoveLogDirectory(true);
        }

        // Save piano roll state
        d->m_pianoRoll->d_func()->saveSettings();

        // Save dock state
        d->saveMainDockState();

        ICore::instance()->windowSystem()->saveWindowGeometry(metaObject()->className(), window());
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent)
        : ICoreWindow(d, IProjectWindow::WindowTypeID(), parent) {
        d.init();
    }

}
