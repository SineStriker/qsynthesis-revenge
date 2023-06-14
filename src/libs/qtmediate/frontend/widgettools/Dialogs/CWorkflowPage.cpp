#include "CWorkflowPage.h"
#include "CWorkflowPage_p.h"

#include <QMDecoratorV2.h>

CWorkflowPagePrivate::CWorkflowPagePrivate() {
}

CWorkflowPagePrivate::~CWorkflowPagePrivate() {
}

void CWorkflowPagePrivate::init() {
    Q_Q(CWorkflowPage);

    titleLabel = new QLabel();
    descriptionLabel = new QLabel();
    stackedWidget = new QStackedWidget();

    rightLayout = new QVBoxLayout();
    rightLayout->setMargin(0);
    rightLayout->setSpacing(0);

    rightLayout->addWidget(titleLabel);
    rightLayout->addWidget(descriptionLabel);
    rightLayout->addWidget(stackedWidget);

    rightWidget = new QWidget();
    rightWidget->setLayout(rightLayout);

    emptyWidget = new QWidget();

    splitter = new QSplitter(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(emptyWidget);
    splitter->addWidget(rightWidget);

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(splitter);
    q->setLayout(mainLayout);

    titleLabel->hide();
    descriptionLabel->hide();
    emptyWidget->hide();

    qIDec->installLocale(this, _LOC(CWorkflowPagePrivate, this));
}

void CWorkflowPagePrivate::reloadStrings() {
    Q_Q(CWorkflowPage);

    auto tryUpdateText = [&](CWorkflowPage::Button button, const QString &text) {
        auto &data = buttonsData[button];
        data.text = text;
        if (data.overrideText.isEmpty()) {
            emit q->buttonTextChanged(button, text);
        }
    };

    tryUpdateText(CWorkflowPage::PreviousButton, tr("Previous"));
    tryUpdateText(CWorkflowPage::NextButton, tr("Next"));
    tryUpdateText(CWorkflowPage::FinishButton, tr("Finish"));
    tryUpdateText(CWorkflowPage::OkButton, tr("Confirm"));
    tryUpdateText(CWorkflowPage::CancelButton, tr("Cancel"));
    tryUpdateText(CWorkflowPage::HelpButton, tr("Help"));
}

QString CWorkflowPagePrivate::buttonText(CWorkflowPage::Button button) const {
    auto &data = buttonsData[button];
    return data.overrideText.isEmpty() ? data.text : data.overrideText;
}

void CWorkflowPagePrivate::setButtonText(CWorkflowPage::Button button, const QString &text) {
    Q_Q(CWorkflowPage);
    auto &data = buttonsData[button];
    data.overrideText = text;
    emit q->buttonTextChanged(button, text.isEmpty() ? data.text : data.overrideText);
}

CWorkflowPage::CWorkflowPage(QWidget *parent) : CWorkflowPage(*new CWorkflowPagePrivate(), parent) {
}

CWorkflowPage::~CWorkflowPage() {
}

QSplitter *CWorkflowPage::splitter() const {
    Q_D(const CWorkflowPage);
    return d->splitter;
}

QString CWorkflowPage::title() const {
    Q_D(const CWorkflowPage);
    return d->titleLabel->text();
}

void CWorkflowPage::setTitle(const QString &title) {
    Q_D(CWorkflowPage);
    d->titleLabel->setText(title);
    d->titleLabel->setHidden(title.isEmpty());
}

QString CWorkflowPage::description() const {
    Q_D(const CWorkflowPage);
    return d->descriptionLabel->text();
}

void CWorkflowPage::setDescription(const QString &description) {
    Q_D(CWorkflowPage);
    d->descriptionLabel->setText(description);
    d->descriptionLabel->setHidden(description.isEmpty());
}

QWidget *CWorkflowPage::takeWidget() {
    Q_D(CWorkflowPage);
    auto w = d->stackedWidget->widget(0);
    if (!w)
        return w;
    d->stackedWidget->removeWidget(w);
    return w;
}

QWidget *CWorkflowPage::widget() const {
    Q_D(const CWorkflowPage);
    return d->stackedWidget->widget(0);
}

void CWorkflowPage::setWidget(QWidget *w) {
    Q_D(CWorkflowPage);
    delete takeWidget();
    d->stackedWidget->addWidget(w);
}

QWidget *CWorkflowPage::takeSideWidget() {
    Q_D(CWorkflowPage);
    if (d->emptyWidget)
        return nullptr;

    auto w = d->splitter->widget(0);
    d->splitter->replaceWidget(0, d->emptyWidget = new QWidget());
    d->emptyWidget->hide();
    return w;
}

QWidget *CWorkflowPage::sideWidget() const {
    Q_D(const CWorkflowPage);
    if (d->emptyWidget)
        return nullptr;

    return d->splitter->widget(0);
}

void CWorkflowPage::setSideWidget(QWidget *w) {
    Q_D(CWorkflowPage);

    auto org = d->splitter->widget(0);
    delete org;
    d->emptyWidget = nullptr;
    d->splitter->insertWidget(0, w);

    d->splitter->setStretchFactor(0, 0);
    d->splitter->setStretchFactor(1, 1);

    w->show();
}

CWorkflowPage::Buttons CWorkflowPage::buttons() const {
    Q_D(const CWorkflowPage);
    return d->buttons;
}

void CWorkflowPage::setButtons(CWorkflowPage::Buttons buttons) {
    Q_D(CWorkflowPage);
    d->buttons = buttons;
    emit buttonsChanged(buttons);
}

void CWorkflowPage::setButton(CWorkflowPage::Button button, bool visible) {
    Q_D(CWorkflowPage);
    d->buttons = visible ? (d->buttons | button) : (d->buttons & ~button);
    emit buttonsChanged(d->buttons);
}

QString CWorkflowPage::buttonText(CWorkflowPage::Button which) const {
    Q_D(const CWorkflowPage);
    return d->buttonText(which);
}

void CWorkflowPage::setButtonText(CWorkflowPage::Button which, const QString &text) {
    Q_D(CWorkflowPage);
    d->setButtonText(which, text);
}

bool CWorkflowPage::buttonEnabled(CWorkflowPage::Button which) const {
    Q_D(const CWorkflowPage);
    return d->buttonsData[which].enabled;
}

void CWorkflowPage::setButtonEnabled(CWorkflowPage::Button which, bool enabled) {
    Q_D(CWorkflowPage);
    d->buttonsData[which].enabled = enabled;
    emit buttonEnabledChanged(which, enabled);
}

void CWorkflowPage::clearPage() {
}

bool CWorkflowPage::validatePage() {
    return true;
}

CWorkflowPage::CWorkflowPage(CWorkflowPagePrivate &d, QWidget *parent) : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}