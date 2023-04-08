#include "WorkflowDialog.h"
#include "WorkflowDialog_p.h"

#include <QResizeEvent>

#include <QMDecorator.h>

namespace Core {

    static int AnimationDuration = 250;

    class WorkflowDialogContainer : public QWidget {
    public:
        explicit WorkflowDialogContainer(WorkflowDialogPrivate *d, QWidget *parent = nullptr) : QWidget(parent), d(d) {
        }

        ~WorkflowDialogContainer() {
        }

        QSize sizeHint() const override {
            return d->widget ? d->widget->sizeHint() : QWidget::sizeHint();
        }

        QSize minimumSizeHint() const override {
            return d->widget ? d->widget->minimumSizeHint() : QWidget::minimumSizeHint();
        }

        bool event(QEvent *event) override {
            switch (event->type()) {
                case QEvent::LayoutRequest:
                    updateGeometry();
                    break;
                default:
                    break;
            }
            return QWidget::event(event);
        }

        WorkflowDialogPrivate *d;
    };

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

    WorkflowDialogPrivate::WorkflowDialogPrivate() {
        widget = nullptr;
    }

    WorkflowDialogPrivate::~WorkflowDialogPrivate() {
    }

    void WorkflowDialogPrivate::init() {
        Q_Q(WorkflowDialog);

        q->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        q->setWindowFlag(Qt::WindowCloseButtonHint, true);

        m_animation = new QPropertyAnimation(this);
        m_animation2 = new QPropertyAnimation(this);

        m_animation->setDuration(AnimationDuration);
        m_animation2->setDuration(AnimationDuration);

        m_animation->setEasingCurve(QEasingCurve::OutCubic);
        m_animation2->setEasingCurve(QEasingCurve::OutCubic);

        container = new WorkflowDialogContainer(this);
        container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        container->installEventFilter(this);

        m_label = new QLabel(container);
        m_label->hide();

        prevButton = new CTabButton();
        nextButton = new CTabButton();
        finishButton = new CTabButton();
        cancelButton = new CTabButton();
        helpButton = new CTabButton();

        buttons[WorkflowPage::PreviousButton] = prevButton;
        buttons[WorkflowPage::NextButton] = nextButton;
        buttons[WorkflowPage::FinishButton] = finishButton;
        buttons[WorkflowPage::CancelButton] = cancelButton;
        buttons[WorkflowPage::HelpButton] = helpButton;

        buttonsLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
        buttonsLayout->setAlignment(Qt::AlignRight);
        buttonsLayout->addWidget(prevButton);
        buttonsLayout->addWidget(nextButton);
        buttonsLayout->addWidget(finishButton);
        buttonsLayout->addWidget(cancelButton);
        buttonsLayout->addWidget(helpButton);

        mainLayout = new QVBoxLayout();
        mainLayout->addWidget(container);
        mainLayout->addLayout(buttonsLayout);

        q->setLayout(mainLayout);

        connect(prevButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_prevClicked);
        connect(nextButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_nextClicked);
        connect(finishButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_finishClicked);
        connect(cancelButton, &QPushButton::clicked, q, &WorkflowDialog::reject);
        connect(helpButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_helpClicked);

        q->setPage(nullptr);
    }

    void WorkflowDialogPrivate::prepareTransition() {
        Q_Q(WorkflowDialog);

        if (!widget) {
            m_label->setPixmap(QPixmap());
            return;
        }

        QPixmap pixmap(widget->size());
        pixmap.fill(Qt::transparent);
        widget->render(&pixmap);

        m_label->setPixmap(pixmap);
    }

    void WorkflowDialogPrivate::doPrevTransition() {
        Q_Q(WorkflowDialog);

        m_label->adjustSize();
        m_label->move(0, 0);

        widget->setGeometry(QRect(QPoint(-container->width(), 0), container->size()));

        QRect labelEnd(QPoint(container->width(), 0), m_label->size());
        QRect widgetEnd(QPoint(), widget->size());

        m_animation->setTargetObject(m_label);
        m_animation->setPropertyName("geometry");
        m_animation->setStartValue(m_label->geometry());
        m_animation->setEndValue(labelEnd);

        m_animation2->setTargetObject(widget);
        m_animation2->setPropertyName("geometry");
        m_animation2->setStartValue(widget->geometry());
        m_animation2->setEndValue(widgetEnd);

        m_label->show();

        m_animation->start();
        m_animation2->start();
    }

    void WorkflowDialogPrivate::doNextTransition() {
        Q_Q(WorkflowDialog);

        m_label->adjustSize();
        m_label->move(0, 0);

        widget->setGeometry(QRect(QPoint(container->width(), 0), container->size()));

        QRect labelEnd(QPoint(-container->width(), 0), m_label->size());
        QRect widgetEnd(QPoint(), widget->size());

        m_animation->setTargetObject(m_label);
        m_animation->setPropertyName("geometry");
        m_animation->setStartValue(m_label->geometry());
        m_animation->setEndValue(labelEnd);

        m_animation2->setTargetObject(widget);
        m_animation2->setPropertyName("geometry");
        m_animation2->setStartValue(widget->geometry());
        m_animation2->setEndValue(widgetEnd);

        m_label->show();

        m_animation->start();
        m_animation2->start();
    }

    bool WorkflowDialogPrivate::eventFilter(QObject *obj, QEvent *event) {
        Q_Q(WorkflowDialog);

        if (obj == container) {
            switch (event->type()) {
                case QEvent::Resize: {
                    m_animation->stop();
                    m_animation2->stop();

                    m_label->hide();

                    if (widget) {
                        widget->setGeometry(QRect(QPoint(), static_cast<QResizeEvent *>(event)->size()));
                    }
                    break;
                }
                default:
                    break;
            }
        }

        return QObject::eventFilter(obj, event);
    }

    void WorkflowDialogPrivate::_q_prevClicked() {
        Q_Q(WorkflowDialog);

        if (m_animation->state() == QAbstractAnimation::Running) {
            return;
        }

        widget->clearPage();

        prepareTransition();

        q->prev();
        emit q->aboutToPrev();

        if (widget)
            doPrevTransition();
    }

    void WorkflowDialogPrivate::_q_nextClicked() {
        Q_Q(WorkflowDialog);

        if (m_animation->state() == QAbstractAnimation::Running) {
            return;
        }

        if (!widget->validatePage()) {
            return;
        }

        prepareTransition();

        q->next();
        emit q->aboutToNext();

        if (widget)
            doNextTransition();
    }

    void WorkflowDialogPrivate::_q_finishClicked() {
        Q_Q(WorkflowDialog);

        if (m_animation->state() == QAbstractAnimation::Running) {
            return;
        }

        if (!widget->validatePage()) {
            return;
        }

        q->finish();
        emit q->finished();

        q->accept();
    }

    void WorkflowDialogPrivate::_q_helpClicked() {
        Q_Q(WorkflowDialog);

        if (widget)
            emit widget->helpRequested();

        emit q->helpRequested();
    }

    void WorkflowDialogPrivate::_q_buttonsChanged(WorkflowPage::Buttons buttons) {
        for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it) {
            auto key = it.key();
            auto btn = it.value();
            btn->setVisible(buttons & key);
            btn->setText(widget->buttonText(key));
            btn->setEnabled(widget->buttonEnabled(key));
        }

        if (cancelButton->isDefault() || helpButton->isDefault()) {
            if (buttons & WorkflowPage::FinishButton && finishButton->isEnabled()) {
                finishButton->setDefault(true);
            } else if (buttons & WorkflowPage::NextButton && nextButton->isEnabled()) {
                nextButton->setDefault(true);
            } else if (buttons & WorkflowPage::PreviousButton && prevButton->isEnabled()) {
                prevButton->setDefault(true);
            }
        }
    }

    void WorkflowDialogPrivate::_q_buttonTextChanged(WorkflowPage::Button which, const QString &text) {
        buttons[which]->setText(text);
    }

    void WorkflowDialogPrivate::_q_buttonEnabledChanged(WorkflowPage::Button which, bool enabled) {
        buttons[which]->setEnabled(enabled);
    }

    WorkflowDialog::WorkflowDialog(QWidget *parent) : WorkflowDialog(*new WorkflowDialogPrivate(), parent) {
    }

    WorkflowDialog::~WorkflowDialog() {
    }

    WorkflowPage *WorkflowDialog::page() const {
        Q_D(const WorkflowDialog);
        return d->widget;
    }

    WorkflowPage *WorkflowDialog::takePage() {
        Q_D(WorkflowDialog);
        auto org = d->widget;
        if (org) {
            d->m_animation->stop();
            d->m_animation2->stop();

            disconnect(org, &WorkflowPage::buttonsChanged, d, &WorkflowDialogPrivate::_q_buttonsChanged);
            disconnect(org, &WorkflowPage::buttonTextChanged, d, &WorkflowDialogPrivate::_q_buttonTextChanged);
            disconnect(org, &WorkflowPage::buttonEnabledChanged, d, &WorkflowDialogPrivate::_q_buttonEnabledChanged);

            org->setParent(nullptr);

            d->widget = nullptr;
            d->container->updateGeometry();
        }
        return org;
    }

    void WorkflowDialog::setPage(WorkflowPage *w) {
        Q_D(WorkflowDialog);

        delete takePage();

        if (!w) {
            for (const auto &btn : qAsConst(d->buttons)) {
                btn->hide();
            }
            return;
        }

        w->setParent(d->container);
        w->setGeometry(QRect(QPoint(), d->container->size()));
        w->show();

        d->widget = w;
        d->container->updateGeometry();

        // Update buttons first
        d->_q_buttonsChanged(w->buttons());

        connect(w, &WorkflowPage::buttonsChanged, d, &WorkflowDialogPrivate::_q_buttonsChanged);
        connect(w, &WorkflowPage::buttonTextChanged, d, &WorkflowDialogPrivate::_q_buttonTextChanged);
        connect(w, &WorkflowPage::buttonEnabledChanged, d, &WorkflowDialogPrivate::_q_buttonEnabledChanged);
    }

    void WorkflowDialog::prev() {
    }

    void WorkflowDialog::next() {
    }

    void WorkflowDialog::finish() {
    }

    WorkflowDialog::WorkflowDialog(WorkflowDialogPrivate &d, QWidget *parent) : QDialog(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
