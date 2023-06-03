#include "FloatingTitleBar.h"

#include <Controllers/MouseEventGuard.h>
#include <Widgets/SynthVSplitter.h>

#include <QMouseEvent>
#include <QStyle>

namespace Core::Internal {

    FloatingTitleBarButton::FloatingTitleBarButton(QWidget *parent) : CPushButton(parent) {
        m_pressed = false;
        m_hintWidget = nullptr;
        m_lastIndex = -1;

        m_curHandle = nullptr;
        m_splitter = nullptr;

        m_hintHeight = -1;
        m_hintColor = Qt::blue;

        new QsApi::MouseEventGuard(Qt::LeftButton, "mouseHandler", this);
    }

    FloatingTitleBarButton::~FloatingTitleBarButton() {
    }

    void FloatingTitleBarButton::mouseHandler(QMouseEvent *event) {
        switch (event->type()) {
            case QEvent::MouseButtonPress: {
                m_pressed = true;
                if (m_hintWidget) {
                    delete m_hintWidget;
                    m_hintWidget = nullptr;
                }
                m_curHandle = qobject_cast<QsApi::SynthVSplitterHandle *>(parentWidget()->parentWidget());
                m_splitter = m_curHandle->splitter();
                break;
            }
            case QEvent::MouseMove: {
                if (!m_pressed)
                    break;

                auto pos = m_splitter->mapFromGlobal(event->globalPos());
                int index = m_splitter->indexAt(pos);

                auto handle = m_splitter->handle(index);
                int handleHeight = (m_hintHeight.value() > 0) ? m_hintHeight : m_curHandle->handleHeight();
                if (handle == m_curHandle) {
                    if (m_hintWidget) {
                        m_hintWidget->hide();
                    }

                    m_lastIndex = -1;
                } else {
                    if (!m_hintWidget) {
                        m_hintWidget = new QWidget(m_splitter->parentWidget());
                        m_hintWidget->setAttribute(Qt::WA_StyledBackground);
                        m_hintWidget->setAutoFillBackground(true);
                        m_hintWidget->setFixedSize(handle->width(), handleHeight);

                        QPalette palette;
                        palette.setColor(QPalette::Window, m_hintColor);
                        m_hintWidget->setPalette(palette);
                    }

                    QPoint p;
                    if (handle->y() > m_curHandle->y()) {
                        if (index > 0) {
                            p = m_splitter->handle(index - 1)->pos();
                        } else {
                            p = QPoint(0, m_splitter->height() - handleHeight);
                        }
                    } else {
                        p = handle->pos();
                    }
                    p = m_splitter->pos() + p;

                    m_hintWidget->move(p);
                    m_hintWidget->show();

                    m_lastIndex = index;
                }
                break;
            }
            case QEvent::MouseButtonRelease: {
                if (!m_pressed) {
                    break;
                }
                m_pressed = false;
                if (m_hintWidget) {
                    delete m_hintWidget;
                    m_hintWidget = nullptr;
                }

                if (m_lastIndex >= 0) {
                    int curIndex = m_splitter->indexOfHandle(m_curHandle);
                    if (m_lastIndex > curIndex) {
                        m_lastIndex++;
                    }
                    m_splitter->moveWidget(curIndex, m_lastIndex);
                }

                m_curHandle = nullptr;
                m_splitter = nullptr;
                m_lastIndex = -1;
                break;
            }
            default:
                break;
        }
    }

    FloatingTitleBar::FloatingTitleBar(QWidget *parent) : QFrame(parent) {
        m_titleBar = nullptr;

        m_foldButton = new CToolButton();
        m_foldButton->setObjectName("fold-button");
        m_foldButton->setCheckable(true);
        m_foldButton->setChecked(true);

        m_titleButton = new FloatingTitleBarButton();
        m_titleButton->setObjectName("title-button");

        m_closeButton = new CToolButton();
        m_closeButton->setObjectName("close-button");

        m_layout = new QHBoxLayout();
        m_layout->setMargin(0);
        m_layout->setSpacing(0);

        m_layout->addWidget(m_foldButton);
        m_layout->addWidget(m_titleButton);
        m_layout->addStretch();
        m_layout->addWidget(m_closeButton);

        setLayout(m_layout);
    }

    FloatingTitleBar::~FloatingTitleBar() {
    }

    QAbstractButton *FloatingTitleBar::foldButton() const {
        return m_foldButton;
    }

    QAbstractButton *FloatingTitleBar::titleButton() const {
        return m_titleButton;
    }

    QAbstractButton *FloatingTitleBar::closeButton() const {
        return m_closeButton;
    }

    QWidget *FloatingTitleBar::titleBar() const {
        return m_titleBar;
    }

    QWidget *FloatingTitleBar::takeTitleBar() {
        if (!m_titleBar)
            return nullptr;

        m_layout->removeWidget(m_titleBar);
        return m_titleBar;
    }

    void FloatingTitleBar::setTitleBar(QWidget *w) {
        if (!w) {
            return;
        }
        delete takeTitleBar();
        m_layout->insertWidget(2, w);
        m_titleBar = w;
    }

}