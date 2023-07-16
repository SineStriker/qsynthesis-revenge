#include "IDockPanel.h"
#include "IDockPanel_p.h"

#include <QApplication>
#include <QContextMenuEvent>
#include <QStyle>

#include <CDockCard.h>
#include <CToolBar.h>

#include <QMDecoratorV2.h>

namespace Core {

    Q_D_LAYOUT_PROPERTY_DECLARE(caption, Caption, captionLayout, IDockPanel)

    class TitleBarStretch : public QWidget {
    public:
        TitleBarStretch(IDockPanel *panel, QWidget *parent = nullptr) : QWidget(parent), panel(panel) {
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            setContextMenuPolicy(Qt::DefaultContextMenu);
        }

        IDockPanel *panel;

    protected:
        void contextMenuEvent(QContextMenuEvent *event) override {
            auto card = panel->card();
            QContextMenuEvent e(QContextMenuEvent::Other, card->mapFromGlobal(QCursor::pos()), QCursor::pos(),
                                QApplication::keyboardModifiers());
            QApplication::sendEvent(card, &e);
        }
    };

    CaptionLabel::CaptionLabel(IDockPanel *s, QWidget *parent) : QLabel(parent), s(s) {
    }

    CaptionLabel::~CaptionLabel() {
    }

    void CaptionLabel::mouseDoubleClickEvent(QMouseEvent *event) {
        auto card = s->card();
        if (card->viewMode() == CDockCard::DockPinned) {
            s->card()->toggleMaximizeState();
        }
    }

    IDockPanelPrivate::IDockPanelPrivate() {
        captionWidget = nullptr;
    }

    IDockPanelPrivate::~IDockPanelPrivate() {
    }

    void IDockPanelPrivate::init() {
        Q_Q(IDockPanel);

        captionLayout = new QHBoxLayout();
        captionLayout->setMargin(0);
        captionLayout->setSpacing(0);

        captionLabel = new CaptionLabel(q);
        captionLabel->setObjectName("side-panel-caption-label");

        toolBar = new CToolBar();
        toolBar->setObjectName("side-panel-toolbar");
        toolBar->setLayoutDirection(Qt::RightToLeft);

        captionLayout->addWidget(captionLabel);
        captionLayout->addWidget(new TitleBarStretch(q));
        captionLayout->addWidget(toolBar);

        mainLayout = new QVBoxLayout();
        mainLayout->setMargin(0);
        mainLayout->setSpacing(0);

        mainLayout->addLayout(captionLayout);

        q->setLayout(mainLayout);

        // Init 2
        captionLabel->setText(q->windowTitle());

        // Add basic buttons
        auto closeAction = new QAction(this);
        closeAction->setObjectName("close");
        closeAction->setProperty("caption-button", true);

        auto menuAction = new QAction(this);
        menuAction->setObjectName("menu");
        menuAction->setProperty("caption-button", true);

        toolBar->addAction(closeAction);
        toolBar->addAction(menuAction);

        connect(closeAction, &QAction::triggered, this, &IDockPanelPrivate::_q_closeActionTriggered);
        connect(menuAction, &QAction::triggered, this, &IDockPanelPrivate::_q_menuActionTriggered);

        qIDec->installTheme(q, "core.IDockPanel");
    }

    void IDockPanelPrivate::_q_closeActionTriggered() {
        Q_Q(IDockPanel);
        q->card()->setChecked(false);
    }

    void IDockPanelPrivate::_q_menuActionTriggered() {
        Q_Q(IDockPanel);

        QContextMenuEvent e(QContextMenuEvent::Other, q->card()->mapFromGlobal(QCursor::pos()), QCursor::pos(),
                            QApplication::keyboardModifiers());
        QApplication::sendEvent(q->card(), &e);
    }

    IDockPanel::IDockPanel(IProjectWindow *iWin, QWidget *parent) : IDockPanel(*new IDockPanelPrivate(), iWin, parent) {
    }

    IDockPanel::~IDockPanel() {
    }

    void IDockPanel::initialize() {
    }

    void IDockPanel::extensionInitialized() {
    }

    QToolBar *IDockPanel::toolBar() const {
        Q_D(const IDockPanel);
        return d->toolBar;
    }

    QWidget *IDockPanel::captionWidget() const {
        Q_D(const IDockPanel);
        return d->captionWidget;
    }

    QWidget *IDockPanel::takeCaptionWidget() {
        Q_D(IDockPanel);
        if (d->captionWidget)
            return nullptr;

        auto org = d->captionWidget;
        d->captionWidget = nullptr;

        d->captionLayout->replaceWidget(org, d->captionLabel);
        d->captionLabel->show();

        return org;
    }

    void IDockPanel::setCaptionWidget(QWidget *w) {
        Q_D(IDockPanel);

        if (!d->captionWidget) {
            d->captionLayout->replaceWidget(d->captionLabel, w);
            w->show();
            d->captionLabel->hide();
            return;
        }

        auto &org = d->captionWidget;
        d->captionLayout->replaceWidget(org, w);
        w->show();

        delete org;
        org = nullptr;
    }

    QWidget *IDockPanel::centralWidget() const {
        Q_D(const IDockPanel);
        if (d->mainLayout->count() < 2)
            return nullptr;
        return d->mainLayout->itemAt(1)->widget();
    }

    QWidget *IDockPanel::takeCentralWidget() {
        Q_D(IDockPanel);
        if (d->mainLayout->count() < 2)
            return nullptr;

        auto org = d->mainLayout->itemAt(1)->widget();
        d->mainLayout->removeWidget(org);

        return org;
    }

    void IDockPanel::setCentralWidget(QWidget *w) {
        Q_D(IDockPanel);
        delete takeCentralWidget();
        d->mainLayout->addWidget(w);
    }

    bool IDockPanel::event(QEvent *event) {
        Q_D(IDockPanel);
        switch (event->type()) {
            case QEvent::WindowTitleChange: {
                d->captionLabel->setText(windowTitle());
                break;
            }
            default:
                break;
        }
        return CDockToolWindow::event(event);
    }

    void IDockPanel::viewModeChanged(CDockCard::ViewMode viewMode) {
        switch (viewMode) {
            case CDockCard::DockPinned:
                setProperty("view-mode", "dock-pinned");
                style()->polish(this);
                break;
            case CDockCard::Float:
                setProperty("view-mode", "float");
                style()->polish(this);
                break;
            case CDockCard::Window:
                setProperty("view-mode", "window");
                style()->polish(this);
                setWindowIcon(qApp->windowIcon());
                break;
        }
    }

    IDockPanel::IDockPanel(IDockPanelPrivate &d, IProjectWindow *iWin, QWidget *parent)
        : CDockToolWindow(parent), IPianoRollComponent(iWin), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
