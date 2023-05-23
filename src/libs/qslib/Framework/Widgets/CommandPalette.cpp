#include "CommandPalette.h"
#include "CommandPalette_p.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>

#include "QsFrameworkNamespace.h"

namespace QsApi {

    CommandPalettePrivate::CommandPalettePrivate() {
    }

    CommandPalettePrivate::~CommandPalettePrivate() {
    }

    void CommandPalettePrivate::init() {
        Q_Q(CommandPalette);

        clickHandled = true;
        paletteActive = false;

        m_lineEdit = new QLineEdit();
        m_listWidget = new QListWidget();

        m_layout = new QVBoxLayout(q);
        m_layout->addWidget(m_lineEdit);
        m_layout->addWidget(m_listWidget);
        q->setLayout(m_layout);

        q->setFocusPolicy(Qt::ClickFocus);
        m_lineEdit->setFocusPolicy(Qt::ClickFocus);
        m_listWidget->setFocusPolicy(Qt::ClickFocus);

        m_lineEdit->installEventFilter(this);
        m_listWidget->installEventFilter(this);

        connect(m_lineEdit, &QLineEdit::textChanged, this, &CommandPalettePrivate::_q_textChanged);
        connect(m_listWidget, &QListWidget::currentRowChanged, this, &CommandPalettePrivate::_q_currentRowChanged);
        connect(m_listWidget, &QListWidget::itemClicked, this, &CommandPalettePrivate::_q_itemClicked);

        q->hide();
    }

    void CommandPalettePrivate::showPalette() {
        Q_Q(CommandPalette);
        if (paletteActive) {
            return;
        }
        paletteActive = true;

        qApp->installEventFilter(this);
        auto w = q->parentWidget();
        if (w) {
            w->installEventFilter(this);
        }

        adjustPalette();
    }

    void CommandPalettePrivate::hidePalette() {
        Q_Q(CommandPalette);
        if (!paletteActive) {
            return;
        }
        paletteActive = false;

        qApp->removeEventFilter(this);
        auto w = q->parentWidget();
        if (w) {
            w->removeEventFilter(this);
        }
    }

    void CommandPalettePrivate::adjustPalette() {
        Q_Q(CommandPalette);
        if (!paletteActive) {
            return;
        }

        auto w = q->parentWidget();
        if (!w) {
            return;
        }

        q->adjustSize();
        q->resize(w->width() / 2, w->height() / 2);
        q->move((w->width() - q->width()) / 2, w->y());
    }

    bool CommandPalettePrivate::eventFilter(QObject *obj, QEvent *event) {
        Q_Q(CommandPalette);

        if (obj == qApp) {
            if (event->type() == QEvent::MouseButtonRelease) {
                if (clickHandled) {
                    clickHandled = false;

                    auto e = static_cast<QMouseEvent *>(event);
                    if (q->isVisible() && !q->rect().contains(q->mapFromGlobal(e->globalPos()))) {
                        QTimer::singleShot(0, this, [this]() {
                            // 点击了非 QWidget 的区域，执行关闭操作
                            qDebug() << "Clicked outside the widget, closing..." << QDateTime::currentDateTime();

                            hidePalette();

                            clickHandled = true;
                        });
                    }
                }
            }
        } else if (obj == q->parentWidget()) {
            if (event->type() == QEvent::Resize) {
                adjustPalette();
            }
        } else if (obj == m_listWidget || obj == m_lineEdit) {
            if (event->type() == QEvent::ShortcutOverride || event->type() == QEvent::KeyPress) {
                auto keyEvent = static_cast<QKeyEvent *>(event);
                switch (keyEvent->key()) {
                    case Qt::Key_Up:
                    case Qt::Key_Down: {
                        if (obj != m_listWidget) {
                            m_listWidget->setFocus();
                            QApplication::sendEvent(m_listWidget, keyEvent);
                            return true;
                        }
                        break;
                    }
                    case Qt::Key_Return:
                    case Qt::Key_Enter: {
                        int index = m_listWidget->currentRow();
                        if (index >= 0 && index < m_listWidget->count()) {
                            q->activate(index);
                        }
                        return true;
                        break;
                    }
                    case Qt::Key_Tab:
                        return true;
                        break;
                    case Qt::Key_Escape:
                        q->abandon();
                        return true;
                        break;
                    default: {
                        if (obj != m_lineEdit) {
                            m_lineEdit->setFocus();
                            QApplication::sendEvent(m_lineEdit, keyEvent);
                            return true;
                        }
                        break;
                    }
                }
            }
        }

        return QObject::eventFilter(obj, event);
    }

    void CommandPalettePrivate::_q_textChanged(const QString &text) {
        Q_Q(CommandPalette);

        QListWidgetItem *firstVisibleItem = nullptr;
        for (int i = 0; i < m_listWidget->count(); i++) {
            auto item = m_listWidget->item(i);
            if (text.isEmpty() || item->text().contains(text, Qt::CaseInsensitive) ||
                item->data(QsApi::DescriptionRole).toString().contains(text, Qt::CaseInsensitive)) {
                if (!firstVisibleItem)
                    firstVisibleItem = item;
                m_listWidget->item(i)->setHidden(false);
            } else {
                m_listWidget->item(i)->setHidden(true);
            }
        }

        if (firstVisibleItem && m_listWidget->currentItem() && m_listWidget->currentItem()->isHidden()) {
            m_listWidget->setCurrentItem(firstVisibleItem);
        }

        emit q->filterKeywordChanged(text);
    }

    void CommandPalettePrivate::_q_currentRowChanged(int row) {
        Q_Q(CommandPalette);
        emit q->currentRowChanged(row);
    }

    void CommandPalettePrivate::_q_itemClicked(QListWidgetItem *item) {
    }

    CommandPalette::CommandPalette(QWidget *parent) : CommandPalette(*new CommandPalettePrivate(), parent) {
    }

    CommandPalette::~CommandPalette() {
    }

    void CommandPalette::insertItem(int index, QListWidgetItem *item) {
        Q_D(CommandPalette);
        d->m_listWidget->insertItem(index, item);
    }

    int CommandPalette::count() const {
        Q_D(const CommandPalette);
        return d->m_listWidget->count();
    }

    void CommandPalette::clear() {
        Q_D(CommandPalette);
        d->m_listWidget->clear();
    }

    QListWidgetItem *CommandPalette::currentItem() const {
        Q_D(const CommandPalette);
        return d->m_listWidget->currentItem();
    }

    void CommandPalette::setCurrentItem(QListWidgetItem *item) {
        Q_D(CommandPalette);
        d->m_listWidget->setCurrentItem(item);
    }

    int CommandPalette::currentRow() const {
        Q_D(const CommandPalette);
        return d->m_listWidget->currentRow();
    }

    void CommandPalette::setCurrentRow(int row) {
        Q_D(CommandPalette);
        d->m_listWidget->setCurrentRow(row);
    }

    QString CommandPalette::filterKeyword() const {
        Q_D(const CommandPalette);
        return d->m_lineEdit->text();
    }

    void CommandPalette::setFilterKeyword(const QString &keyword) {
        Q_D(CommandPalette);
        d->m_lineEdit->setText(keyword);
    }

    void CommandPalette::activate(int index) {
        emit activated(index);
    }

    void CommandPalette::abandon() {
        Q_D(CommandPalette);
        d->m_lineEdit->clear();
        d->m_listWidget->clear();
        emit abandoned();
    }

    bool CommandPalette::event(QEvent *event) {
        Q_D(CommandPalette);

        switch (event->type()) {
            case QEvent::Show: {
                d->showPalette();
                break;
            }
            case QEvent::ParentAboutToChange:
            case QEvent::Hide: {
                d->hidePalette();
                break;
            }
            case QEvent::FocusIn: {
                d->m_lineEdit->setFocus();
                break;
            }
            default:
                break;
        }
        return BaseContainer::event(event);
    }

    CommandPalette::CommandPalette(CommandPalettePrivate &d, QWidget *parent) : BaseContainer(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
}
