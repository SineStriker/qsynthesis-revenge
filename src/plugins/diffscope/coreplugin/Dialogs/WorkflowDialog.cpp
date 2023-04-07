#include "WorkflowDialog.h"
#include "WorkflowDialog_p.h"

#include <QResizeEvent>

#include <QMDecorator.h>

namespace Core {

    static int AnimationDuration = 250;

    WorkflowDialogPrivate::WorkflowDialogPrivate() {
        hasPrev = false;
        hasNext = false;
        widget = nullptr;

        m_pos1 = new PosWrapper(this);
        m_pos2 = new PosWrapper(this);
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

        container = new QFrame();
        container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        container->installEventFilter(this);

        m_label = new QLabel(container);
        m_label->hide();
        m_pos1->w = m_label;

        prevButton = new CTabButton();
        nextButton = new CTabButton();
        cancelButton = new CTabButton();

        buttonsLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
        buttonsLayout->setAlignment(Qt::AlignRight);
        buttonsLayout->addWidget(prevButton);
        buttonsLayout->addWidget(nextButton);
        buttonsLayout->addWidget(cancelButton);

        mainLayout = new QVBoxLayout();
        mainLayout->addWidget(container);
        mainLayout->addLayout(buttonsLayout);

        q->setLayout(mainLayout);

        connect(prevButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_prevClicked);
        connect(nextButton, &QPushButton::clicked, this, &WorkflowDialogPrivate::_q_nextClicked);
        connect(cancelButton, &QPushButton::clicked, q, &WorkflowDialog::reject);

        qIDec->installLocale(this, {{}}, _LOC(WorkflowDialogPrivate, this));
    }

    void WorkflowDialogPrivate::reloadStrings() {
        prevButton->setText(tr("Previous"));
        cancelButton->setText(tr("Cancel"));
        refreshNextButton();
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

        m_label->show();
        m_label->adjustSize();
        m_label->move(0, 0);

        widget->show();
        widget->setGeometry(QRect(QPoint(-container->width(), 0), container->size()));

        m_animation->setTargetObject(m_pos1);
        m_animation->setPropertyName("x");
        m_animation->setStartValue(m_label->x());
        m_animation->setEndValue(container->width());

        m_animation2->setTargetObject(m_pos2);
        m_animation2->setPropertyName("x");
        m_animation2->setStartValue(widget->x());
        m_animation2->setEndValue(0);

        m_animation->start();
        m_animation2->start();
    }

    void WorkflowDialogPrivate::doNextTransition() {
        Q_Q(WorkflowDialog);

        m_label->show();
        m_label->adjustSize();
        m_label->move(0, 0);

        widget->show();
        widget->setGeometry(QRect(QPoint(container->width(), 0), container->size()));

        m_animation->setTargetObject(m_pos1);
        m_animation->setPropertyName("x");
        m_animation->setStartValue(m_label->x());
        m_animation->setEndValue(-container->width());

        m_animation2->setTargetObject(m_pos2);
        m_animation2->setPropertyName("x");
        m_animation2->setStartValue(widget->x());
        m_animation2->setEndValue(0);

        m_animation->start();
        m_animation2->start();
    }

    void WorkflowDialogPrivate::refreshPrevButton() {
        prevButton->setEnabled(hasPrev);
    }

    void WorkflowDialogPrivate::refreshNextButton() {
        nextButton->setText(hasNext ? tr("Next") : tr("Finish"));
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

        prepareTransition();

        q->prev();
        emit q->aboutToPrev();

        refreshPrevButton();

        if (widget)
            doPrevTransition();
    }

    void WorkflowDialogPrivate::_q_nextClicked() {
        Q_Q(WorkflowDialog);

        if (m_animation->state() == QAbstractAnimation::Running) {
            return;
        }

        if (!hasNext) {
            emit q->finished();
            q->accept();
            return;
        }

        prepareTransition();

        q->next();
        emit q->aboutToNext();

        refreshNextButton();

        if (widget)
            doNextTransition();
    }

    WorkflowDialog::WorkflowDialog(QWidget *parent) : WorkflowDialog(*new WorkflowDialogPrivate(), parent) {
    }

    WorkflowDialog::~WorkflowDialog() {
    }

    bool WorkflowDialog::prevEnabled() const {
        Q_D(const WorkflowDialog);
        return d->hasPrev;
    }

    void WorkflowDialog::setPrevEnabled(bool enabled) {
        Q_D(WorkflowDialog);
        d->hasPrev = enabled;
        d->refreshPrevButton();
    }

    bool WorkflowDialog::nextEnabled() const {
        Q_D(const WorkflowDialog);
        return d->hasNext;
    }

    void WorkflowDialog::setNextEnabled(bool enabled) {
        Q_D(WorkflowDialog);
        d->hasNext = enabled;
        d->refreshNextButton();
    }

    QWidget *WorkflowDialog::widget() const {
        Q_D(const WorkflowDialog);
        return d->widget;
    }

    QWidget *WorkflowDialog::takeWidget() {
        Q_D(WorkflowDialog);
        auto org = d->widget;
        if (org) {
            d->m_animation->stop();
            d->m_animation2->stop();
            d->m_pos2->w = nullptr;

            org->setParent(nullptr);
            d->widget = nullptr;
        }
        return org;
    }

    void WorkflowDialog::setWidget(QWidget *w) {
        Q_D(WorkflowDialog);

        delete takeWidget();

        w->setParent(d->container);
        w->setGeometry(QRect(QPoint(), d->container->size()));
        d->widget = w;
        d->m_pos2->w = w;
    }

    void WorkflowDialog::prev() {
    }

    void WorkflowDialog::next() {
    }

    WorkflowDialog::WorkflowDialog(WorkflowDialogPrivate &d, QWidget *parent) : QDialog(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}