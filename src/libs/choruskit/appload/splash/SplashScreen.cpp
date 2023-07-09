#include "SplashScreen.h"

#include <QDateTime>
#include <QDebug>
#include <QPainter>

SplashScreen::SplashScreen(QScreen *screen) : QSplashScreen(screen) {
    m_showTexts = false;
    m_texts.insert("_status", {});

    connect(this, &QSplashScreen::messageChanged, this, &SplashScreen::_q_messageChanged);
}

SplashScreen::~SplashScreen() {
}

void SplashScreen::setTextAttribute(const QString &id, const SplashScreen::Attribute &attr) {
    m_texts[id] = attr;

    if (m_showTexts && isVisible())
        repaint();
}

void SplashScreen::setText(const QString &id, const QString &text) {
    auto it = m_texts.find(id);
    if (it == m_texts.end())
        return;
    it->text = text;

    if (m_showTexts && isVisible())
        repaint();
}

void SplashScreen::drawContents(QPainter *painter) {
    // QSplashScreen::drawContents(painter);

    if (!m_showTexts) {
        return;
    }

    // Draw texts
    for (const auto &item : qAsConst(m_texts)) {
        const Attribute &attr = item;

        QFont font;
#ifdef Q_OS_WINDOWS
        // font.setFamily("Microsoft YaHei");
#endif
        font.setPixelSize(attr.fontSize
                          //* screen()->logicalDotsPerInch() / QMOs::unitDpi()
        );
        QFontMetrics fm(font);

        QPoint pos(attr.pos);
        if (pos.x() < 0) {
            pos.rx() += this->width();
        }
        if (pos.y() < 0) {
            pos.ry() += this->height();
        }

        int w = this->width();
        int maxWidth = qMin(attr.anchor.first > 0 ? w - pos.x() : pos.x(), attr.maxWidth > 0 ? attr.maxWidth : w);
        QRect dst(attr.anchor.first > 0 ? pos.x() : pos.x() - maxWidth,
                  attr.anchor.second > 0 ? pos.y() : pos.y() - fm.height(), maxWidth, fm.height());

        QString text = fm.horizontalAdvance(item.text) > dst.width()
                           ? fm.elidedText(item.text, Qt::ElideRight, dst.width())
                           : item.text;
        painter->setPen(QPen(attr.fontColor));
        painter->setFont(font);
        painter->drawText(dst, (attr.anchor.first > 0 ? Qt::AlignLeft : Qt::AlignRight) | Qt::AlignVCenter, text);
    }
}

void SplashScreen::showTexts() {
    m_showTexts = true;
    if (isVisible())
        repaint();
}

void SplashScreen::mousePressEvent(QMouseEvent *event) {
    // No hide
}

void SplashScreen::_q_messageChanged(const QString &message) {
    setText("_status", message);
}

void SplashScreen::closeEvent(QCloseEvent *event) {
    emit closed();
}
