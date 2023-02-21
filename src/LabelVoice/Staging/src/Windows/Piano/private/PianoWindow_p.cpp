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

    frame = new CDockFrame();
    frame->setObjectName("piano-main-frame");

    tabs = new QScrollableTabWidget();
    tabs->setObjectName("main-tabs");

    spkPanel = new QWidget();
    spkPanel->setObjectName("speakers-panel");

    expPanel = new VExplorerPanel();
    expPanel->setObjectName("explorer-panel");

    langPanel = new QWidget();
    langPanel->setObjectName("languages-panel");

    expCard = frame->addWidget(Qt::LeftEdge, QM::Primary, expPanel);
    spkCard = frame->addWidget(Qt::LeftEdge, QM::Primary, spkPanel);
    langCard = frame->addWidget(Qt::LeftEdge, QM::Primary, langPanel);

    expCard->setProperty("parent", frame->objectName());
    expCard->setObjectName("explorer-card");

    spkCard->setProperty("parent", frame->objectName());
    spkCard->setObjectName("speakers-card");

    langCard->setProperty("parent", frame->objectName());
    langCard->setObjectName("languages-card");

    frame->setWidget(tabs);

    q->setCentralWidget(frame);
}

void PianoWindowPrivate::reloadStrings_helper() {
    actions->reloadStrings();

    expCard->setText(PianoWindow::tr("Explorer"));
    spkCard->setText(PianoWindow::tr("Speakers"));
    langCard->setText(PianoWindow::tr("Languages"));

    expPanel->reloadStrings();
}
