#include "QMWorkflowPage.h"
#include "QMWorkflowPage_p.h"

#include <QMDecoratorV2.h>

QMWorkflowPagePrivate::QMWorkflowPagePrivate() {
}

QMWorkflowPagePrivate::~QMWorkflowPagePrivate() {
}

void QMWorkflowPagePrivate::init() {
    Q_Q(QMWorkflowPage);

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

    qIDec->installLocale(this, _LOC(QMWorkflowPagePrivate, this));
}

void QMWorkflowPagePrivate::reloadStrings() {
    Q_Q(QMWorkflowPage);

    auto tryUpdateText = [&](QMWorkflowPage::Button button, const QString &text) {
        auto &data = buttonsData[button];
        data.text = text;
        if (data.overrideText.isEmpty()) {
            emit q->buttonTextChanged(button, text);
        }
    };

    tryUpdateText(QMWorkflowPage::PreviousButton, tr("Previous"));
    tryUpdateText(QMWorkflowPage::NextButton, tr("Next"));
    tryUpdateText(QMWorkflowPage::FinishButton, tr("Finish"));
    tryUpdateText(QMWorkflowPage::OkButton, tr("Confirm"));
    tryUpdateText(QMWorkflowPage::CancelButton, tr("Cancel"));
    tryUpdateText(QMWorkflowPage::HelpButton, tr("Help"));
}

QString QMWorkflowPagePrivate::buttonText(QMWorkflowPage::Button button) const {
    auto &data = buttonsData[button];
    return data.overrideText.isEmpty() ? data.text : data.overrideText;
}

void QMWorkflowPagePrivate::setButtonText(QMWorkflowPage::Button button, const QString &text) {
    Q_Q(QMWorkflowPage);
    auto &data = buttonsData[button];
    data.overrideText = text;
    emit q->buttonTextChanged(button, text.isEmpty() ? data.text : data.overrideText);
}

QMWorkflowPage::QMWorkflowPage(QWidget *parent) : QMWorkflowPage(*new QMWorkflowPagePrivate(), parent) {
}

QMWorkflowPage::~QMWorkflowPage() {
}

QSplitter *QMWorkflowPage::splitter() const {
    Q_D(const QMWorkflowPage);
    return d->splitter;
}

QString QMWorkflowPage::title() const {
    Q_D(const QMWorkflowPage);
    return d->titleLabel->text();
}

void QMWorkflowPage::setTitle(const QString &title) {
    Q_D(QMWorkflowPage);
    d->titleLabel->setText(title);
    d->titleLabel->setHidden(title.isEmpty());
}

QString QMWorkflowPage::description() const {
    Q_D(const QMWorkflowPage);
    return d->descriptionLabel->text();
}

void QMWorkflowPage::setDescription(const QString &description) {
    Q_D(QMWorkflowPage);
    d->descriptionLabel->setText(description);
    d->descriptionLabel->setHidden(description.isEmpty());
}

QWidget *QMWorkflowPage::takeWidget() {
    Q_D(QMWorkflowPage);
    auto w = d->stackedWidget->widget(0);
    if (!w)
        return w;
    d->stackedWidget->removeWidget(w);
    return w;
}

QWidget *QMWorkflowPage::widget() const {
    Q_D(const QMWorkflowPage);
    return d->stackedWidget->widget(0);
}

void QMWorkflowPage::setWidget(QWidget *w) {
    Q_D(QMWorkflowPage);
    delete takeWidget();
    d->stackedWidget->addWidget(w);
}

QWidget *QMWorkflowPage::takeSideWidget() {
    Q_D(QMWorkflowPage);
    if (d->emptyWidget)
        return nullptr;

    auto w = d->splitter->widget(0);
    d->splitter->replaceWidget(0, d->emptyWidget = new QWidget());
    d->emptyWidget->hide();
    return w;
}

QWidget *QMWorkflowPage::sideWidget() const {
    Q_D(const QMWorkflowPage);
    if (d->emptyWidget)
        return nullptr;

    return d->splitter->widget(0);
}

void QMWorkflowPage::setSideWidget(QWidget *w) {
    Q_D(QMWorkflowPage);

    auto org = d->splitter->widget(0);
    delete org;
    d->emptyWidget = nullptr;
    d->splitter->insertWidget(0, w);

    d->splitter->setStretchFactor(0, 0);
    d->splitter->setStretchFactor(1, 1);

    w->show();
}

QMWorkflowPage::Buttons QMWorkflowPage::buttons() const {
    Q_D(const QMWorkflowPage);
    return d->buttons;
}

void QMWorkflowPage::setButtons(QMWorkflowPage::Buttons buttons) {
    Q_D(QMWorkflowPage);
    d->buttons = buttons;
    emit buttonsChanged(buttons);
}

void QMWorkflowPage::setButton(QMWorkflowPage::Button button, bool visible) {
    Q_D(QMWorkflowPage);
    d->buttons = visible ? (d->buttons | button) : (d->buttons & ~button);
    emit buttonsChanged(d->buttons);
}

QString QMWorkflowPage::buttonText(QMWorkflowPage::Button which) const {
    Q_D(const QMWorkflowPage);
    return d->buttonText(which);
}

void QMWorkflowPage::setButtonText(QMWorkflowPage::Button which, const QString &text) {
    Q_D(QMWorkflowPage);
    d->setButtonText(which, text);
}

bool QMWorkflowPage::buttonEnabled(QMWorkflowPage::Button which) const {
    Q_D(const QMWorkflowPage);
    return d->buttonsData[which].enabled;
}

void QMWorkflowPage::setButtonEnabled(QMWorkflowPage::Button which, bool enabled) {
    Q_D(QMWorkflowPage);
    d->buttonsData[which].enabled = enabled;
    emit buttonEnabledChanged(which, enabled);
}

void QMWorkflowPage::clearPage() {
}

bool QMWorkflowPage::validatePage() {
    return true;
}

QMWorkflowPage::QMWorkflowPage(QMWorkflowPagePrivate &d, QWidget *parent) : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}