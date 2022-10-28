#include "PianoWindow_p.h"

PianoWindowPrivate::PianoWindowPrivate() {
}

PianoWindowPrivate::~PianoWindowPrivate() {
}

void PianoWindowPrivate::init() {
    Q_Q(PianoWindow);

    // Don't show icon
    if (winHandle) {
        winHandle->setTitleBarFlags(winHandle->titleBarFlags() & ~IWindowHandle::WindowIcon);
    }

    actions = new PianoActions(q);
    actions->setup();

    q->connect(actions, &PianoActions::triggered, q, &PianoWindow::_q_actionTriggered);

    docMgr = new CommandManager(q);

    frame = new CCoupleTabFrame();
    frame->setObjectName("piano-main-frame");

    tabs = new QScrollableTabWidget();
    tabs->setObjectName("main-tabs");

    spkPanel = new QWidget();
    spkPanel->setObjectName("speakers-panel");

    expPanel = new VExplorerPanel();
    expPanel->setObjectName("explorer-panel");

    langPanel = new QWidget();
    langPanel->setObjectName("languages-panel");

    expCard = new CCoupleTabBarCard();
    expCard->setProperty("parent", frame->objectName());
    expCard->setObjectName("explorer-card");
    expCard->setWidget(expPanel);

    spkCard = new CCoupleTabBarCard();
    spkCard->setProperty("parent", frame->objectName());
    spkCard->setObjectName("speakers-card");
    spkCard->setWidget(spkPanel);

    langCard = new CCoupleTabBarCard();
    langCard->setProperty("parent", frame->objectName());
    langCard->setObjectName("languages-card");
    langCard->setWidget(langPanel);

    frame->leftBar()->firstBar()->addCard(expCard);
    frame->leftBar()->firstBar()->addCard(spkCard);
    frame->leftBar()->firstBar()->addCard(langCard);

    frame->setWidget(tabs);

    q->setCentralWidget(frame);

    docMgr->addSubscriber(expPanel);
}

void PianoWindowPrivate::reloadStrings_helper() {
    actions->reloadStrings();

    expCard->setText(PianoWindow::tr("Explorer"));
    spkCard->setText(PianoWindow::tr("Speakers"));
    langCard->setText(PianoWindow::tr("Languages"));

    expPanel->reloadStrings();
}
