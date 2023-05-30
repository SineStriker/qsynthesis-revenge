#include "FloatingTitleBar.h"

namespace Core::Internal {

    FloatingTitleBarButton::FloatingTitleBarButton(QWidget *parent) : CPushButton(parent) {
    }

    FloatingTitleBarButton::~FloatingTitleBarButton() {
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
        // qDebug() << "Floating title bar destroyed";
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