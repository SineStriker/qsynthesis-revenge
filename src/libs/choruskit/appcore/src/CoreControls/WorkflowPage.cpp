#include "WorkflowPage.h"
#include "WorkflowPage_p.h"

#include <QMDecorator.h>

namespace Core {

    WorkflowPagePrivate::WorkflowPagePrivate() {
    }

    WorkflowPagePrivate::~WorkflowPagePrivate() {
    }

    void WorkflowPagePrivate::init() {
        Q_Q(WorkflowPage);

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

        q->addWidget(emptyWidget);
        q->addWidget(rightWidget);

        titleLabel->hide();
        descriptionLabel->hide();
        emptyWidget->hide();

        qIDec->installLocale(this, {{}}, _LOC(WorkflowPagePrivate, this));
    }

    void WorkflowPagePrivate::reloadStrings() {
        Q_Q(WorkflowPage);

        auto tryUpdateText = [&](WorkflowPage::Button button, const QString &text) {
            auto &data = buttonsData[button];
            data.text = text;
            if (data.overrideText.isEmpty()) {
                emit q->buttonTextChanged(button, text);
            }
        };

        tryUpdateText(WorkflowPage::PreviousButton, tr("Previous"));
        tryUpdateText(WorkflowPage::NextButton, tr("Next"));
        tryUpdateText(WorkflowPage::FinishButton, tr("Finish"));
        tryUpdateText(WorkflowPage::CancelButton, tr("Cancel"));
        tryUpdateText(WorkflowPage::HelpButton, tr("Help"));
    }

    QString WorkflowPagePrivate::buttonText(WorkflowPage::Button button) const {
        auto &data = buttonsData[button];
        return data.overrideText.isEmpty() ? data.text : data.overrideText;
    }

    void WorkflowPagePrivate::setButtonText(WorkflowPage::Button button, const QString &text) {
        Q_Q(WorkflowPage);
        auto &data = buttonsData[button];
        data.overrideText = text;
        emit q->buttonTextChanged(button, text.isEmpty() ? data.text : data.overrideText);
    }

    WorkflowPage::WorkflowPage(QWidget *parent) : WorkflowPage(*new WorkflowPagePrivate(), parent) {
    }

    WorkflowPage::~WorkflowPage() {
    }

    QString WorkflowPage::title() const {
        Q_D(const WorkflowPage);
        return d->titleLabel->text();
    }

    void WorkflowPage::setTitle(const QString &title) {
        Q_D(WorkflowPage);
        d->titleLabel->setText(title);
        d->titleLabel->setHidden(title.isEmpty());
    }

    QString WorkflowPage::description() const {
        Q_D(const WorkflowPage);
        return d->descriptionLabel->text();
    }

    void WorkflowPage::setDescription(const QString &description) {
        Q_D(WorkflowPage);
        d->descriptionLabel->setText(description);
        d->descriptionLabel->setHidden(description.isEmpty());
    }

    QWidget *WorkflowPage::takeWidget() {
        Q_D(WorkflowPage);
        auto w = d->stackedWidget->widget(0);
        if (!w)
            return w;
        d->stackedWidget->removeWidget(w);
        return w;
    }

    QWidget *WorkflowPage::widget() const {
        Q_D(const WorkflowPage);
        return d->stackedWidget->widget(0);
    }

    void WorkflowPage::setWidget(QWidget *w) {
        Q_D(WorkflowPage);
        delete takeWidget();
        d->stackedWidget->addWidget(w);
    }

    QWidget *WorkflowPage::takeSideWidget() {
        Q_D(WorkflowPage);
        if (d->emptyWidget)
            return nullptr;

        auto w = QSplitter::widget(0);
        QSplitter::replaceWidget(0, d->emptyWidget = new QWidget());
        d->emptyWidget->hide();
        return w;
    }

    QWidget *WorkflowPage::sideWidget() const {
        Q_D(const WorkflowPage);
        if (d->emptyWidget)
            return nullptr;

        return QSplitter::widget(0);
    }

    void WorkflowPage::setSideWidget(QWidget *w) {
        Q_D(WorkflowPage);

        auto org = QSplitter::widget(0);
        QSplitter::replaceWidget(0, w);

        delete org;
        d->emptyWidget = nullptr;

        w->show();
    }

    WorkflowPage::Buttons WorkflowPage::buttons() const {
        Q_D(const WorkflowPage);
        return d->buttons;
    }

    void WorkflowPage::setButtons(WorkflowPage::Buttons buttons) {
        Q_D(WorkflowPage);
        d->buttons = buttons;
        emit buttonsChanged(buttons);
    }

    void WorkflowPage::setButton(Core::WorkflowPage::Button button, bool visible) {
        Q_D(WorkflowPage);
        d->buttons = visible ? (d->buttons | button) : (d->buttons & ~button);
        emit buttonsChanged(d->buttons);
    }

    QString WorkflowPage::buttonText(WorkflowPage::Button which) const {
        Q_D(const WorkflowPage);
        return d->buttonText(which);
    }

    void WorkflowPage::setButtonText(WorkflowPage::Button which, const QString &text) {
        Q_D(WorkflowPage);
        d->setButtonText(which, text);
    }

    bool WorkflowPage::buttonEnabled(WorkflowPage::Button which) const {
        Q_D(const WorkflowPage);
        return d->buttonsData[which].enabled;
    }

    void WorkflowPage::setButtonEnabled(WorkflowPage::Button which, bool enabled) {
        Q_D(WorkflowPage);
        d->buttonsData[which].enabled = enabled;
        emit buttonEnabledChanged(which, enabled);
    }

    void WorkflowPage::clearPage() {
    }

    bool WorkflowPage::validatePage() {
        return true;
    }

    WorkflowPage::WorkflowPage(WorkflowPagePrivate &d, QWidget *parent) : QSplitter(Qt::Horizontal, parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();
    }

}