#include "CommandPalette.h"
#include "CommandPalette_p.h"

#include "QMarginsImpl.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QTimer>

#include <CLineEdit.h>

#include "QsFrameworkNamespace.h"

namespace QsApi {

    CommandPalettePrivate::CommandPalettePrivate() {
    }

    CommandPalettePrivate::~CommandPalettePrivate() {
    }

    void CommandPalettePrivate::init() {
        Q_Q(CommandPalette);

        noClickOutsideEventToHandle = true;
        paletteActive = false;

        m_lineEdit = new CLineEdit();
        m_lineEdit->setObjectName("search-box");

        m_listWidget = new QListWidget();
        m_listWidget->setObjectName("item-list");

        m_layout = new QVBoxLayout(q);
        m_layout->addWidget(m_lineEdit);
        m_layout->addWidget(m_listWidget);
        q->setLayout(m_layout);

        q->setFocusPolicy(Qt::ClickFocus);
        m_lineEdit->setFocusPolicy(Qt::ClickFocus);
        m_listWidget->setFocusPolicy(Qt::ClickFocus);

        m_lineEdit->installEventFilter(this);
        m_listWidget->installEventFilter(this);

        m_delegate = new TitleListItemDelegate(this);
        m_listWidget->setItemDelegate(m_delegate);

        connect(m_lineEdit, &QLineEdit::textChanged, this, &CommandPalettePrivate::_q_textChanged);
        connect(m_listWidget, &QListWidget::currentRowChanged, this, &CommandPalettePrivate::_q_currentRowChanged);
        connect(m_listWidget, &QListWidget::currentItemChanged, this, &CommandPalettePrivate::_q_currentItemChanged);
        connect(m_delegate, &TitleListItemDelegate::clicked, this, &CommandPalettePrivate::_q_delegateClicked);

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

        m_listWidget->setVisible(m_listWidget->count() > 0);

        adjustPalette();
        m_lineEdit->setFocus();
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

        q->raise();
        q->adjustSize();
        q->resize(w->width() / 2, w->height() / 2);
        q->move((w->width() - q->width()) / 2, 0);
    }

    void CommandPalettePrivate::clearPalette() {
        m_lineEdit->clear();
        m_lineEdit->setPlaceholderText({});
        m_listWidget->clear();
    }

    static bool isMenu(QObject *obj) {
        return (obj->inherits("QWidgetWindow") && obj->objectName() == "QMenuClassWindow") ||
               qobject_cast<QMenu *>(obj);
    }

    bool CommandPalettePrivate::eventFilter(QObject *obj, QEvent *event) {
        Q_Q(CommandPalette);

        if (event->type() == QEvent::MouseButtonPress) {
            auto e = static_cast<QMouseEvent *>(event);
            if (q->isVisible() && !q->rect().contains(q->mapFromGlobal(e->globalPos())) && !isMenu(obj)) {
                if (noClickOutsideEventToHandle) {
                    noClickOutsideEventToHandle = false;

                    // 点击了非 QWidget 的区域，执行关闭操作
                    // qDebug() << "Clicked outside the widget, closing..." << QDateTime::currentDateTime();
                    q->abandon();

                    QTimer::singleShot(0, this, [this, q]() {
                        noClickOutsideEventToHandle = true;
                    });
                }
                goto out;
            }
        } else if (event->type() == QEvent::Shortcut) {
            q->abandon();
            goto out;
        }

        if (obj == q->parentWidget()) {
            if (event->type() == QEvent::Resize) {
                adjustPalette();
            }
        } else if (obj == m_listWidget || obj == m_lineEdit) {
            if (event->type() == QEvent::ShortcutOverride || event->type() == QEvent::KeyPress) {
                auto keyEvent = static_cast<QKeyEvent *>(event);
                switch (keyEvent->key()) {
                    case Qt::Key_Up:
                    case Qt::Key_Down: {
                        if (obj != m_listWidget && m_listWidget->isVisible()) {
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
                        } else {
                            q->activate(0);
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

    out:
        return QObject::eventFilter(obj, event);
    }

    void CommandPalettePrivate::_q_textChanged(const QString &text) {
        Q_Q(CommandPalette);

        QListWidgetItem *firstVisibleItem = nullptr;

        QString trimmed = text.trimmed();
        QStringList keywords = trimmed.split(' ');
        for (int i = 0; i < m_listWidget->count(); i++) {
            auto item = m_listWidget->item(i);
            bool find = false;

            if (trimmed.isEmpty()) {
                find = true;
            } else {
                for (const auto &keyword : qAsConst(keywords)) {
                    if (keyword.isEmpty()) {
                        continue;
                    }

                    if (item->text().contains(keyword, Qt::CaseInsensitive) ||
                        item->data(QsApi::SubtitleRole).toString().contains(keyword, Qt::CaseInsensitive) ||
                        item->data(QsApi::KeywordRole).toString().contains(keyword, Qt::CaseInsensitive)) {
                        find = true;
                    }
                }
            }

            if (!firstVisibleItem && find) {
                firstVisibleItem = item;
            }

            m_listWidget->item(i)->setHidden(!find);
        }

        if (!m_listWidget->currentItem() || m_listWidget->currentItem()->isHidden()) {
            if (firstVisibleItem) {
                m_listWidget->setCurrentItem(firstVisibleItem);
            } else {
                m_listWidget->clearSelection();
            }
        } else {
            // Explicitly reselect current item
            auto item = m_listWidget->currentItem();
            m_listWidget->clearSelection();
            m_listWidget->setCurrentItem(item);
        }

        emit q->filterKeywordChanged(text);
    }

    void CommandPalettePrivate::_q_currentRowChanged(int row) {
        Q_Q(CommandPalette);
        emit q->currentRowChanged(row);
    }

    void CommandPalettePrivate::_q_currentItemChanged(QListWidgetItem *cur, QListWidgetItem *prev) {
        Q_Q(CommandPalette);
        emit q->currentItemChanged(cur);
    }

    void CommandPalettePrivate::_q_delegateClicked(const QModelIndex &index, int button) {
        Q_Q(CommandPalette);
        if (button != Qt::LeftButton) {
            return;
        }

        auto idx = index.row();
        if (idx >= 0) {
            emit q->activate(idx);
        } else {
            emit q->abandon();
        }
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

    QListWidgetItem *CommandPalette::itemAt(int index) const {
        Q_D(const CommandPalette);
        return d->m_listWidget->item(index);
    }

    QListWidgetItem *CommandPalette::currentItem() const {
        Q_D(const CommandPalette);
        return d->m_listWidget->currentItem();
    }

    void CommandPalette::setCurrentItem(QListWidgetItem *item) {
        Q_D(CommandPalette);
        if (!item) {
            return;
        }
        d->m_listWidget->setCurrentItem(item);
        d->m_listWidget->scrollToItem(item);
    }

    int CommandPalette::currentRow() const {
        Q_D(const CommandPalette);
        return d->m_listWidget->currentRow();
    }

    void CommandPalette::setCurrentRow(int row) {
        Q_D(CommandPalette);
        d->m_listWidget->setCurrentRow(row);
    }

    QString CommandPalette::filterHint() const {
        Q_D(const CommandPalette);
        return d->m_lineEdit->placeholderText();
    }

    void CommandPalette::setFilterHint(const QString &hint) {
        Q_D(CommandPalette);
        d->m_lineEdit->setPlaceholderText(hint);
    }

    QString CommandPalette::filterKeyword() const {
        Q_D(const CommandPalette);
        return d->m_lineEdit->text();
    }

    void CommandPalette::setFilterKeyword(const QString &keyword) {
        Q_D(CommandPalette);
        d->m_lineEdit->setText(keyword);
    }

    QTypeList CommandPalette::styleData() const {
        Q_D(const CommandPalette);
        return d->m_delegate->styleData();
    }

    void CommandPalette::setStyleData(const QTypeList &list) {
        Q_D(CommandPalette);

        d->m_delegate->setStyleData(list);
        update();

        emit styleDataChanged();
    }

    void CommandPalette::start() {
        show();
    }

    void CommandPalette::activate(int index) {
        Q_D(CommandPalette);
        if (!d->paletteActive) {
            return;
        }

        hide();
        emit activated(index);
        emit finished(d->m_listWidget->item(index));
        d->clearPalette();
    }

    void CommandPalette::abandon() {
        Q_D(CommandPalette);

        activate(-1);
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
