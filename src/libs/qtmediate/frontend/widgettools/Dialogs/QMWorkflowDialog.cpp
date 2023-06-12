#include "QMWorkflowDialog.h"
#include "QMWorkflowDialog_p.h"

#include <QResizeEvent>

#include <QMView.h>

static int AnimationDuration = 250;

class QMWorkflowDialogContainer : public QWidget {
public:
    explicit QMWorkflowDialogContainer(QMWorkflowDialogPrivate *d, QWidget *parent = nullptr) : QWidget(parent), d(d) {
    }

    ~QMWorkflowDialogContainer() {
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

    QMWorkflowDialogPrivate *d;
};

QMWorkflowDialogPrivate::QMWorkflowDialogPrivate() {
    widget = nullptr;
}

QMWorkflowDialogPrivate::~QMWorkflowDialogPrivate() {
}

void QMWorkflowDialogPrivate::init() {
    Q_Q(QMWorkflowDialog);

    q->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    q->setWindowFlag(Qt::WindowCloseButtonHint, true);

    m_animation = new QPropertyAnimation(this);
    m_animation2 = new QPropertyAnimation(this);

    m_animation->setDuration(AnimationDuration);
    m_animation2->setDuration(AnimationDuration);

    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    m_animation2->setEasingCurve(QEasingCurve::OutCubic);

    container = new QMWorkflowDialogContainer(this);
    container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    container->installEventFilter(this);

    m_label = new QLabel(container);
    m_label->hide();

    prevButton = new CTabButton();
    nextButton = new CTabButton();
    finishButton = new CTabButton();
    okButton = new CTabButton();
    cancelButton = new CTabButton();
    helpButton = new CTabButton();

    buttons[QMWorkflowPage::PreviousButton] = prevButton;
    buttons[QMWorkflowPage::NextButton] = nextButton;
    buttons[QMWorkflowPage::FinishButton] = finishButton;
    buttons[QMWorkflowPage::OkButton] = okButton;
    buttons[QMWorkflowPage::CancelButton] = cancelButton;
    buttons[QMWorkflowPage::HelpButton] = helpButton;

    buttonsLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
    buttonsLayout->setAlignment(Qt::AlignRight);
    buttonsLayout->addWidgetE(prevButton);
    buttonsLayout->addWidgetE(nextButton);
    buttonsLayout->addWidgetE(finishButton);
    buttonsLayout->addWidgetE(okButton);
    buttonsLayout->addWidgetE(cancelButton);
    buttonsLayout->addWidgetE(helpButton);

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(container);
    mainLayout->addLayout(buttonsLayout);

    q->setLayout(mainLayout);

    connect(prevButton, &QPushButton::clicked, this, &QMWorkflowDialogPrivate::_q_prevClicked);
    connect(nextButton, &QPushButton::clicked, this, &QMWorkflowDialogPrivate::_q_nextClicked);
    connect(okButton, &QPushButton::clicked, this, &QMWorkflowDialogPrivate::_q_finishClicked);
    connect(finishButton, &QPushButton::clicked, this, &QMWorkflowDialogPrivate::_q_finishClicked);
    connect(cancelButton, &QPushButton::clicked, q, &QMWorkflowDialog::reject);
    connect(helpButton, &QPushButton::clicked, this, &QMWorkflowDialogPrivate::_q_helpClicked);

    q->setPage(nullptr);
}

void QMWorkflowDialogPrivate::prepareTransition() {
    Q_Q(QMWorkflowDialog);

    if (!widget) {
        m_label->setPixmap(QPixmap());
        return;
    }

    auto pixmap = QMView::createDeviceRenderPixmap(q->window()->windowHandle(), widget->size());
    pixmap.fill(Qt::transparent);
    widget->render(&pixmap);

    m_label->setPixmap(pixmap);
}

void QMWorkflowDialogPrivate::doPrevTransition() {
    Q_Q(QMWorkflowDialog);

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

void QMWorkflowDialogPrivate::doNextTransition() {
    Q_Q(QMWorkflowDialog);

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

bool QMWorkflowDialogPrivate::eventFilter(QObject *obj, QEvent *event) {
    Q_Q(QMWorkflowDialog);

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

void QMWorkflowDialogPrivate::_q_prevClicked() {
    Q_Q(QMWorkflowDialog);

    if (m_animation->state() == QAbstractAnimation::Running) {
        return;
    }

    widget->clearPage();

    prepareTransition();

    if (!q->prev()) {
        return;
    }
    emit q->aboutToPrev();

    if (widget)
        doPrevTransition();
}

void QMWorkflowDialogPrivate::_q_nextClicked() {
    Q_Q(QMWorkflowDialog);

    if (m_animation->state() == QAbstractAnimation::Running) {
        return;
    }

    if (!widget->validatePage()) {
        return;
    }

    prepareTransition();

    if (!q->next()) {
        return;
    }
    emit q->aboutToNext();

    if (widget)
        doNextTransition();
}

void QMWorkflowDialogPrivate::_q_finishClicked() {
    Q_Q(QMWorkflowDialog);

    if (m_animation->state() == QAbstractAnimation::Running) {
        return;
    }

    if (!widget->validatePage()) {
        return;
    }

    if (!q->finish()) {
        return;
    }
    emit q->finished();

    q->accept();
}

void QMWorkflowDialogPrivate::_q_helpClicked() {
    Q_Q(QMWorkflowDialog);

    if (widget)
        emit widget->helpRequested();

    emit q->helpRequested();
}

void QMWorkflowDialogPrivate::_q_buttonsChanged(QMWorkflowPage::Buttons buttons) {
    for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it) {
        auto key = it.key();
        auto btn = it.value();
        btn->setVisible(buttons & key);
        btn->setText(widget->buttonText(key));
        btn->setEnabled(widget->buttonEnabled(key));
    }

    if (cancelButton->isDefault() || helpButton->isDefault()) {
        if (buttons & QMWorkflowPage::FinishButton && finishButton->isEnabled()) {
            finishButton->setDefault(true);
        } else if (buttons & QMWorkflowPage::OkButton && okButton->isEnabled()) {
            okButton->setDefault(true);
        } else if (buttons & QMWorkflowPage::NextButton && nextButton->isEnabled()) {
            nextButton->setDefault(true);
        } else if (buttons & QMWorkflowPage::PreviousButton && prevButton->isEnabled()) {
            prevButton->setDefault(true);
        }
    }
}

void QMWorkflowDialogPrivate::_q_buttonTextChanged(QMWorkflowPage::Button which, const QString &text) {
    buttons[which]->setText(text);
}

void QMWorkflowDialogPrivate::_q_buttonEnabledChanged(QMWorkflowPage::Button which, bool enabled) {
    buttons[which]->setEnabled(enabled);
}

QMWorkflowDialog::QMWorkflowDialog(QWidget *parent) : QMWorkflowDialog(*new QMWorkflowDialogPrivate(), parent) {
}

QMWorkflowDialog::~QMWorkflowDialog() {
}

QMWorkflowPage *QMWorkflowDialog::page() const {
    Q_D(const QMWorkflowDialog);
    return d->widget;
}

QMWorkflowPage *QMWorkflowDialog::takePage() {
    Q_D(QMWorkflowDialog);
    auto org = d->widget;
    if (org) {
        d->m_animation->stop();
        d->m_animation2->stop();

        disconnect(org, &QMWorkflowPage::buttonsChanged, d, &QMWorkflowDialogPrivate::_q_buttonsChanged);
        disconnect(org, &QMWorkflowPage::buttonTextChanged, d, &QMWorkflowDialogPrivate::_q_buttonTextChanged);
        disconnect(org, &QMWorkflowPage::buttonEnabledChanged, d, &QMWorkflowDialogPrivate::_q_buttonEnabledChanged);

        org->setParent(nullptr);

        d->widget = nullptr;
        d->container->updateGeometry();
    }
    return org;
}

void QMWorkflowDialog::setPage(QMWorkflowPage *w) {
    Q_D(QMWorkflowDialog);

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

    connect(w, &QMWorkflowPage::buttonsChanged, d, &QMWorkflowDialogPrivate::_q_buttonsChanged);
    connect(w, &QMWorkflowPage::buttonTextChanged, d, &QMWorkflowDialogPrivate::_q_buttonTextChanged);
    connect(w, &QMWorkflowPage::buttonEnabledChanged, d, &QMWorkflowDialogPrivate::_q_buttonEnabledChanged);
}

bool QMWorkflowDialog::prev() {
    return true;
}

bool QMWorkflowDialog::next() {
    return true;
}

bool QMWorkflowDialog::finish() {
    return true;
}

QMWorkflowDialog::QMWorkflowDialog(QMWorkflowDialogPrivate &d, QWidget *parent) : QDialog(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
