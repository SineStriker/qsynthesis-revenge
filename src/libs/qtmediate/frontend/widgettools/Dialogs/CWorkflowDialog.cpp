#include "CWorkflowDialog.h"
#include "CWorkflowDialog_p.h"

#include <QResizeEvent>

#include <QMView.h>

static int AnimationDuration = 250;

class QMWorkflowDialogContainer : public QWidget {
public:
    explicit QMWorkflowDialogContainer(CWorkflowDialogPrivate *d, QWidget *parent = nullptr) : QWidget(parent), d(d) {
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

    CWorkflowDialogPrivate *d;
};

CWorkflowDialogPrivate::CWorkflowDialogPrivate() {
    widget = nullptr;
}

CWorkflowDialogPrivate::~CWorkflowDialogPrivate() {
}

void CWorkflowDialogPrivate::init() {
    Q_Q(CWorkflowDialog);

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

    buttons[CWorkflowPage::PreviousButton] = prevButton;
    buttons[CWorkflowPage::NextButton] = nextButton;
    buttons[CWorkflowPage::FinishButton] = finishButton;
    buttons[CWorkflowPage::OkButton] = okButton;
    buttons[CWorkflowPage::CancelButton] = cancelButton;
    buttons[CWorkflowPage::HelpButton] = helpButton;

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

    connect(prevButton, &QPushButton::clicked, this, &CWorkflowDialogPrivate::_q_prevClicked);
    connect(nextButton, &QPushButton::clicked, this, &CWorkflowDialogPrivate::_q_nextClicked);
    connect(okButton, &QPushButton::clicked, this, &CWorkflowDialogPrivate::_q_finishClicked);
    connect(finishButton, &QPushButton::clicked, this, &CWorkflowDialogPrivate::_q_finishClicked);
    connect(cancelButton, &QPushButton::clicked, q, &CWorkflowDialog::reject);
    connect(helpButton, &QPushButton::clicked, this, &CWorkflowDialogPrivate::_q_helpClicked);

    q->setPage(nullptr);
}

void CWorkflowDialogPrivate::prepareTransition() {
    Q_Q(CWorkflowDialog);

    if (!widget) {
        m_label->setPixmap(QPixmap());
        return;
    }

    auto pixmap = QMView::createDeviceRenderPixmap(q->window()->windowHandle(), widget->size());
    pixmap.fill(Qt::transparent);
    widget->render(&pixmap);

    m_label->setPixmap(pixmap);
}

void CWorkflowDialogPrivate::doPrevTransition() {
    Q_Q(CWorkflowDialog);

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

void CWorkflowDialogPrivate::doNextTransition() {
    Q_Q(CWorkflowDialog);

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

bool CWorkflowDialogPrivate::eventFilter(QObject *obj, QEvent *event) {
    Q_Q(CWorkflowDialog);

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

void CWorkflowDialogPrivate::_q_prevClicked() {
    Q_Q(CWorkflowDialog);

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

void CWorkflowDialogPrivate::_q_nextClicked() {
    Q_Q(CWorkflowDialog);

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

void CWorkflowDialogPrivate::_q_finishClicked() {
    Q_Q(CWorkflowDialog);

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

void CWorkflowDialogPrivate::_q_helpClicked() {
    Q_Q(CWorkflowDialog);

    if (widget)
        emit widget->helpRequested();

    emit q->helpRequested();
}

void CWorkflowDialogPrivate::_q_buttonsChanged(CWorkflowPage::Buttons buttons) {
    for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it) {
        auto key = it.key();
        auto btn = it.value();
        btn->setVisible(buttons & key);
        btn->setText(widget->buttonText(key));
        btn->setEnabled(widget->buttonEnabled(key));
    }

    if (cancelButton->isDefault() || helpButton->isDefault()) {
        if (buttons & CWorkflowPage::FinishButton && finishButton->isEnabled()) {
            finishButton->setDefault(true);
        } else if (buttons & CWorkflowPage::OkButton && okButton->isEnabled()) {
            okButton->setDefault(true);
        } else if (buttons & CWorkflowPage::NextButton && nextButton->isEnabled()) {
            nextButton->setDefault(true);
        } else if (buttons & CWorkflowPage::PreviousButton && prevButton->isEnabled()) {
            prevButton->setDefault(true);
        }
    }
}

void CWorkflowDialogPrivate::_q_buttonTextChanged(CWorkflowPage::Button which, const QString &text) {
    buttons[which]->setText(text);
}

void CWorkflowDialogPrivate::_q_buttonEnabledChanged(CWorkflowPage::Button which, bool enabled) {
    buttons[which]->setEnabled(enabled);
}

CWorkflowDialog::CWorkflowDialog(QWidget *parent) : CWorkflowDialog(*new CWorkflowDialogPrivate(), parent) {
}

CWorkflowDialog::~CWorkflowDialog() {
}

CWorkflowPage *CWorkflowDialog::page() const {
    Q_D(const CWorkflowDialog);
    return d->widget;
}

CWorkflowPage *CWorkflowDialog::takePage() {
    Q_D(CWorkflowDialog);
    auto org = d->widget;
    if (org) {
        d->m_animation->stop();
        d->m_animation2->stop();

        disconnect(org, &CWorkflowPage::buttonsChanged, d, &CWorkflowDialogPrivate::_q_buttonsChanged);
        disconnect(org, &CWorkflowPage::buttonTextChanged, d, &CWorkflowDialogPrivate::_q_buttonTextChanged);
        disconnect(org, &CWorkflowPage::buttonEnabledChanged, d, &CWorkflowDialogPrivate::_q_buttonEnabledChanged);

        org->setParent(nullptr);

        d->widget = nullptr;
        d->container->updateGeometry();
    }
    return org;
}

void CWorkflowDialog::setPage(CWorkflowPage *w) {
    Q_D(CWorkflowDialog);

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

    connect(w, &CWorkflowPage::buttonsChanged, d, &CWorkflowDialogPrivate::_q_buttonsChanged);
    connect(w, &CWorkflowPage::buttonTextChanged, d, &CWorkflowDialogPrivate::_q_buttonTextChanged);
    connect(w, &CWorkflowPage::buttonEnabledChanged, d, &CWorkflowDialogPrivate::_q_buttonEnabledChanged);
}

bool CWorkflowDialog::prev() {
    return true;
}

bool CWorkflowDialog::next() {
    return true;
}

bool CWorkflowDialog::finish() {
    return true;
}

CWorkflowDialog::CWorkflowDialog(CWorkflowDialogPrivate &d, QWidget *parent) : QDialog(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
