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
#include <QMSystem.h>
#include <QMView.h>

#include <CoreApi/ILoader.h>

#include "ICore.h"

#include "Internal/Widgets/PianoKeyWidget.h"
#include "PianoRoll/PianoRoll_p.h"

namespace Core {

    static const char mainDockSettingCatalogC[] = "IProjectWindow/MainDock";

    IProjectWindowPrivate::IProjectWindowPrivate(DspxDocument *doc) : m_doc(doc) {
        m_changeHandled = true;
        m_forceClose = false;

        m_doc = doc;
        m_timeMgr = nullptr;

        doc->setAutoRemoveLogDir(false);
        doc->setParent(this);
    }

    IProjectWindowPrivate::~IProjectWindowPrivate() {
    }

    void IProjectWindowPrivate::init() {
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

    bool IProjectWindowPrivate::eventFilter(QObject *obj, QEvent *event) {
        Q_Q(IProjectWindow);
        if (obj == q->window()) {
            switch (event->type()) {
                case QEvent::Close: {
                    if (!m_forceClose) {
                        if (m_doc->isVST()) {
                            qDebug() << "VSTMode, Hide";

                            // VST mode, we simply hide window
                            event->ignore();
                            q->window()->hide();
                            return true;
                        } else if (m_doc->isModified()) {
                            // Unexpected close
                            auto ret = QMessageBox::question(q->window(), ICore::mainTitle(),
                                                             tr("There are unsaved changes, do you want to save them?"),
                                                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                            switch (ret) {
                                case QMessageBox::Yes: {
                                    event->setAccepted([q, this]() {
                                        if (QMFs::isFileExist(m_doc->filePath())) {
                                            return m_doc->save(m_doc->filePath());
                                        }
                                        return ICore::instance()->documentSystem()->saveFileBrowse(q->window(), m_doc);
                                    }());
                                    break;
                                }
                                case QMessageBox::No:
                                    m_forceClose = true;
                                    event->accept();
                                    break;
                                case QMessageBox::Cancel:
                                    event->ignore();
                                    return true;
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
        return ICoreWindowPrivate::eventFilter(obj, event);
    }

    void IProjectWindowPrivate::_q_documentChanged() {
        Q_Q(IProjectWindow);

        if (!m_changeHandled) {
            return;
        }
        m_changeHandled = false;

        QTimer::singleShot(0, q, [q, this]() {
            q->setWindowModified(m_doc->isModified());
            q->setWindowTitle(m_doc->displayName());
            m_changeHandled = true;
        });
    }

    void IProjectWindowPrivate::_q_documentRaiseRequested() {
        Q_Q(IProjectWindow);
        QMView::bringWindowToForeground(q->window());
    }

    void IProjectWindowPrivate::_q_documentCloseRequested() {
        Q_Q(IProjectWindow);

        m_forceClose = true;
        if (!m_doc->isVST())
            q->setProperty("choruskit_show_home", true);
        q->window()->close();
    }

    IProjectWindow::IProjectWindow(DspxDocument *doc, QObject *parent)
        : IProjectWindow(*new IProjectWindowPrivate(doc), parent) {
    }

    IProjectWindow::~IProjectWindow() {
    }

    DspxDocument *IProjectWindow::doc() const {
        Q_D(const IProjectWindow);
        return d->m_doc;
    }

    MusicTimeManager *IProjectWindow::timeManager() const {
        Q_D(const IProjectWindow);
        return d->m_timeMgr;
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

    void IProjectWindow::nextLoadingState(Core::IWindow::State nextState) {
        ICoreWindow::nextLoadingState(nextState);

        Q_D(IProjectWindow);
        auto win = window();
        switch (nextState) {
            case IWindow::WindowSetup: {
                win->setObjectName("project-window");
                win->setAcceptDrops(true);

                d->mainToolbarCtx = ICore::instance()->actionSystem()->context("project.MainToolbar");
                if (!d->mainToolbarCtx) {
                    ICore::fatalError(win, tr("Failed to create main toolbar."));
                }

                auto &timeMgr = d->m_timeMgr;
                timeMgr = new MusicTimeManager(this, d);

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
                connect(frame, &CDockFrame::cardAdded, this, [](QAbstractButton *card) {
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

                auto &pianoRoll = d->m_pianoRoll;
                pianoRoll = new PianoRoll(this);
                frame->setWidget(pianoRoll);

                setCentralWidget(centralWidget);
                d->cp->setParent(centralWidget);

                win->installEventFilter(d);

                // Close event
                connect(d->m_doc, &IDocument::changed, d, &IProjectWindowPrivate::_q_documentChanged);
                connect(d->m_doc, &IDocument::raiseRequested, d, &IProjectWindowPrivate::_q_documentRaiseRequested);
                connect(d->m_doc, &IDocument::closeRequested, d, &IProjectWindowPrivate::_q_documentCloseRequested);

                // Initialize
                timeMgr->initialize();
                pianoRoll->initialize();
                break;
            }

            case IWindow::Initialized: {
                // Initialized
                d->m_pianoRoll->extensionInitialized();
                d->m_timeMgr->extensionInitialized();
                break;
            }

            case IWindow::Running: {
                connect(d->mainToolbarCtx, &ActionContext::stateChanged, d, &IProjectWindowPrivate::reloadMainToolbar);
                d->reloadMainToolbar();

                qIDec->installLocale(this, _LOC(IProjectWindowPrivate, d));
                qIDec->installTheme(win, "core.ProjectWindow");

                // Restore dock
                d->restoreMainDockState();

                // Restore piano roll state
                d->m_pianoRoll->d_func()->readSettings();

                ICore::instance()->windowSystem()->loadWindowGeometry(metaObject()->className(), win, {1200, 800});

                // Update window state
                emit d->m_doc->changed();

                break;
            }

            case IWindow::Closed: {
                // Windows should be all closed before quit signal arrives except a kill or
                // interrupt signal is emitted, only when this function is called the log should be
                // removed expectedly.
                if (!d->m_doc->isModified() || d->m_forceClose) {
                    d->m_doc->setAutoRemoveLogDir(true);
                }

                // Save piano roll state
                d->m_pianoRoll->d_func()->saveSettings();

                // Save dock state
                d->saveMainDockState();

                ICore::instance()->windowSystem()->saveWindowGeometry(metaObject()->className(), window());
                break;
            }

            default:
                break;
        }
    }

    IProjectWindow::IProjectWindow(IProjectWindowPrivate &d, QObject *parent) : ICoreWindow(d, "project", parent) {
        d.init();
    }

} // namespace Core
