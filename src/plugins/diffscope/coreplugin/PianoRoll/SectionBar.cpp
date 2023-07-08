#include "SectionBar.h"
#include "SectionBar_p.h"

#include <QApplication>
#include <QFontMetrics>
#include <QPaintEvent>

#include <cmath>

namespace Core {

    SectionBarPrivate::SectionBarPrivate() {
        curWidth = 100;
        curSnap = 480;
        startPos = 0;

        sectionNumber.setColors({0x999999, 0xCCCCCC});
        sectionBackground.setRect({4, 0, 4, 0});
        sectionBackground.setColor(Qt::transparent);

        signatureNumber.setColors({0x999999, 0xCCCCCC});
        signatureBackground.setRect({4, 0, 4, 0});
        signatureBackground.setColor(Qt::transparent);

        tempoNumber.setColor(0xCCCCCC);
        tempoBackground.setRect({4, 0, 4, 0});
        tempoBackground.setColors({0x636775, 0x878B9A, 0x999999});

        sectionLine = QPen(Qt::lightGray, 0x1, Qt::SolidLine);
        beatLine = sectionLine;
        tempoLine = QPen(Qt::lightGray, 0x1, Qt::DashLine);

        timeline = nullptr;
        pressed = false;

        deltaX = 0;
        curBarNumber = -1;
        curTimeSignature = -1;
        curTempo = -1;
        targetTempo = -1;
    }

    SectionBarPrivate::~SectionBarPrivate() {
    }

    void SectionBarPrivate::init() {
        Q_Q(SectionBar);
        q->setFrameStyle(QFrame::NoFrame);
        q->setMouseTracking(true);
    }

    void SectionBarPrivate::updateLayout() {
        Q_Q(SectionBar);

        timeSignatureBlocks.clear();
        tempoBlocks.clear();

        q->update();
    }

    void SectionBarPrivate::updateMouseArea(QMouseEvent *event) {
        Q_Q(SectionBar);

        int pos = event->pos().x();
        Q_UNUSED(pos);
        q->update();
    }

    SectionBar::SectionBar(IProjectWindow *iWin, QWidget *parent) : SectionBar(*new SectionBarPrivate(), iWin, parent) {
    }

    SectionBar::~SectionBar() {
    }

    void SectionBar::initialize() {
        Q_D(SectionBar);

        auto timeMgr = iWin->timeManager();
        auto timeline = timeMgr->timeline();
        connect(timeMgr, &MusicTimeManager::changed, d, &SectionBarPrivate::updateLayout);

        d->timeline = timeline;
    }

    void SectionBar::extensionInitialized() {
    }

    QTypeMap SectionBar::styleData() const {
        return QTypeMap();
    }

    void SectionBar::setStyleData(const QTypeMap &map) {
    }

    int SectionBar::startPos() const {
        Q_D(const SectionBar);
        return d->startPos;
    }

    void SectionBar::setStartPos(int tick) {
        Q_D(SectionBar);
        d->startPos = tick;
        d->updateLayout();
    }

    int SectionBar::currentWidth() const {
        Q_D(const SectionBar);
        return d->curWidth;
    }

    void SectionBar::setCurrentWidth(int width) {
        Q_D(SectionBar);
        d->curWidth = width;
        d->updateLayout();
    }

    int SectionBar::currentSnap() const {
        Q_D(const SectionBar);
        return d->curSnap;
    }

    void SectionBar::setCurrentSnap(int snap) {
        Q_D(SectionBar);
        d->curSnap = snap;
        d->updateLayout();
    }

    void SectionBar::paintEvent(QPaintEvent *event) {
        Q_D(SectionBar);

        QFrame::paintEvent(event);

        auto orgTempo = d->curTempo;

        d->curBarNumber = -1;
        d->curTimeSignature = -1;
        d->curTempo = -1;
        d->targetTempo = -1;

        QPainter painter(this);

        double unit = double(d->curWidth) / 480;
        const auto &changedRect = event->rect();
        int h = height();

        // Previous time signature
        struct Bar {
            int bar;
            int tick;
            QsApi::MusicTimeSignature timeSignature;
            bool hasSignature;
        };

        auto drawTimeSig = [&](const QRect &r0, int tick, const QsApi::MusicTimeSignature &ts) {
            if (r0.contains(mapFromGlobal(QCursor::pos()))) {
                d->curTimeSignature = tick;
                if (d->pressed) {
                    painter.setPen(d->signatureNumber.color3());
                    painter.setBrush(d->signatureBackground.color3());
                } else {
                    painter.setPen(d->signatureNumber.color2());
                    painter.setBrush(d->signatureBackground.color2());
                }
            } else {
                painter.setPen(d->signatureNumber.color());
                painter.setBrush(d->signatureBackground.color());
            }

            if (r0.left() >= changedRect.left() || r0.right() < changedRect.right()) {
                if (painter.brush() != Qt::NoBrush) {
                    auto pen = painter.pen();
                    painter.setPen(Qt::NoPen);
                    painter.drawRoundedRect(r0, d->signatureBackground.radius(), d->signatureBackground.radius());
                    painter.setPen(pen);
                }
                painter.setFont(d->signatureNumber.font());
                painter.drawText(r0, Qt::AlignCenter, ts.toString());
            }
        };

        auto drawTempo = [&](QRect r0, int tick, double tempo) {
            bool highlight = false;
            if (orgTempo == tick && d->deltaX != 0) {
                highlight = true;

                int toX = r0.left() + d->deltaX;
                int tick = (toX + d->startPos) / unit;
                tick = qMax(0, (tick + d->curSnap / 2) / d->curSnap * d->curSnap);
                toX = tick * unit - d->startPos;
                d->targetTempo = tick;

                r0.moveLeft(toX);
            }
            if ((r0.contains(mapFromGlobal(QCursor::pos())) && !(orgTempo != tick && d->deltaX != 0)) || highlight) {
                d->curTempo = (d->deltaX != 0) ? orgTempo : tick;
                if (d->pressed) {
                    painter.setPen(d->tempoNumber.color3());
                    painter.setBrush(d->tempoBackground.color3());
                } else {
                    painter.setPen(d->tempoNumber.color2());
                    painter.setBrush(d->tempoBackground.color2());
                }
            } else {
                painter.setPen(d->tempoNumber.color());
                painter.setBrush(d->tempoBackground.color());
            }

            if (r0.left() >= changedRect.left() || r0.right() < changedRect.right()) {
                auto pen = painter.pen();

                // Draw line
                painter.setPen(d->tempoLine);
                painter.drawLine(r0.left(), 0, r0.left(), h);

                // Draw back
                if (painter.brush() != Qt::NoBrush) {
                    painter.setPen(Qt::NoPen);
                    painter.drawRoundedRect(r0, d->tempoBackground.radius(), d->tempoBackground.radius());
                }

                // Draw text
                painter.setPen(pen);
                painter.setFont(d->tempoNumber.font());
                painter.drawText(r0, Qt::AlignCenter, QString::number(tempo));
            }
        };

        {
            // Draw time signatures
            int curPos = d->startPos + changedRect.left() / unit;
            int endPos = d->startPos + changedRect.right() / unit;

            auto time = d->timeline->tickToTime(d->startPos); // start pos time
            int startPos = d->timeline->timeToTick(time.measure(), 0, 0);

            // Draw tempo
            const auto &tempoMap = d->timeline->tempoMap();
            {
                auto it1 = tempoMap.upperBound(time.measure()) - 1;
                for (auto it = it1; it != tempoMap.end(); ++it) {
                    if (it.key() >= endPos)
                        break;

                    int x2 = (it.key() - curPos) * unit;

                    QFontMetrics fm(d->tempoNumber.font());
                    QString text = QString::number(it.value());
                    auto margins = d->tempoBackground.margins();
                    int rw = margins.left() + margins.right() + fm.horizontalAdvance(text);
                    int rh = fm.height() + margins.top() + margins.bottom();
                    QRect r0(x2, h / 4 - rh / 2, rw, rh);
                    if (r0.left() > changedRect.left() || r0.right() < changedRect.right()) {
                        d->tempoBlocks.insert(x2, {r0, it.key(), it.value()});
                    }
                }

                const SectionBarPrivate::Tempo *deferredBlock = nullptr;
                for (const auto &item : qAsConst(d->tempoBlocks)) {
                    if (orgTempo == item.tick && d->deltaX != 0) {
                        deferredBlock = &item;
                    }
                    drawTempo(item.rect, item.tick, item.value);
                }

                if (deferredBlock) {
                    const auto &item = *deferredBlock;
                    drawTempo(item.rect, item.tick, item.value);
                }
            }

            // Find the first time signature in the view port (Exclude start)
            const auto &timeSigMap = d->timeline->timeSignatureMap();

            QList<Bar> barAndSigs;

            auto it0 = timeSigMap.upperBound(time.measure()) - 1;
            {
                int curBar = time.measure();
                int curTick = startPos;

                auto it = it0;
                while (curTick < endPos) {
                    barAndSigs.append({curBar, curTick, it.value(), it.key() == curBar});

                    curBar++;
                    curTick += it->numerator() * 1920 / it->denominator();

                    auto it1 = it + 1;
                    if (it1 != timeSigMap.end() && it1.key() == curBar) {
                        it = it1; // Encounter next time signature
                    }
                }
            }

            // First bar
            int curSection = time.measure();
            for (const auto &bar : qAsConst(barAndSigs)) {
                auto beatTicks = bar.timeSignature.numerator() * 480 / bar.timeSignature.denominator();

                do {
                    // Draw section line
                    int x0 = (bar.tick - curPos) * unit;
                    int x1;

                    if (x0 >= changedRect.left()) {
                        painter.setPen(d->sectionLine);
                        painter.setBrush(Qt::NoBrush);
                        painter.drawLine(x0, h / 2, x0, h);
                    }

                    // Draw text
                    QFontMetrics fm0(d->sectionNumber.font());
                    {
                        QString text = QString::number(curSection);
                        auto margins = d->sectionBackground.margins();
                        int rw = margins.left() + margins.right() + fm0.horizontalAdvance(text);
                        int rh = fm0.height() + margins.top() + margins.bottom();
                        QRect r0(x0, h * 3 / 4 - rh / 2, rw, rh);
                        if (r0.left() > changedRect.left() || r0.right() < changedRect.right()) {
                            if (r0.contains(mapFromGlobal(QCursor::pos()))) {
                                d->curBarNumber = curSection;
                                if (d->pressed) {
                                    painter.setPen(d->sectionNumber.color3());
                                    painter.setBrush(d->sectionBackground.color3());
                                } else {
                                    painter.setPen(d->sectionNumber.color2());
                                    painter.setBrush(d->sectionBackground.color2());
                                }
                            } else {
                                painter.setPen(d->sectionNumber.color());
                                painter.setBrush(d->sectionBackground.color());
                            }
                            if (painter.brush() != Qt::NoBrush) {
                                auto pen = painter.pen();
                                painter.setPen(Qt::NoPen);
                                painter.drawRect(r0);
                                painter.setPen(pen);
                            }
                            painter.setFont(d->sectionNumber.font());
                            painter.drawText(r0, Qt::AlignCenter, text);
                        }

                        x1 = r0.right() + fm0.horizontalAdvance(' ');
                    }

                    if (!bar.hasSignature) {
                        break;
                    }

                    // Draw time signature
                    QFontMetrics fm1(d->signatureNumber.font());
                    {
                        QString text = bar.timeSignature.toString();
                        auto margins = d->signatureBackground.margins();
                        int rw = margins.left() + margins.right() + fm1.horizontalAdvance(text);
                        int rh = fm1.height() + margins.top() + margins.bottom();
                        QRect r0(x1, h - rh, rw, rh);

                        int maxW = beatTicks * unit + x0 - x1;
                        if (r0.width() > maxW) {
                            r0.setWidth(maxW);
                        }

                        if (r0.left() > changedRect.left() || r0.right() < changedRect.right()) {
                            d->timeSignatureBlocks.insert(x1, {r0, curSection, bar.tick, bar.timeSignature});
                        }
                    }
                } while (false);

                // Draw beat line
                for (int i = 1; i < bar.timeSignature.numerator(); ++i) {
                    int x0 = (bar.tick + i * beatTicks - curPos) * unit;
                    if (x0 < changedRect.left()) {
                        continue;
                    }
                    painter.setPen(d->beatLine);
                    painter.setBrush(Qt::NoBrush);
                    painter.drawLine(x0, 3 * h / 4, x0, h);
                }

                curSection++;
            }

            for (const auto &item : qAsConst(d->timeSignatureBlocks)) {
                drawTimeSig(item.rect, item.tick, item.value);
            }
        }
    }

    void SectionBar::mousePressEvent(QMouseEvent *event) {
        Q_D(SectionBar);
        if (event->button() == Qt::LeftButton) {
            d->pressed = true;
            d->pressedPos = event->pos();
            d->deltaX = 0;

            if (d->curBarNumber >= 0) {
                qDebug() << "bar clicked" << d->curBarNumber;
            } else if (d->curTimeSignature >= 0) {
                qDebug() << "time sig clicked" << d->curTimeSignature;
            } else if (d->curTempo >= 0) {
                qDebug() << "tempo clicked" << d->curTempo;
            }
        }
        d->updateMouseArea(event);
    }

    void SectionBar::mouseMoveEvent(QMouseEvent *event) {
        Q_D(SectionBar);

        if (d->pressed)
            d->deltaX = event->pos().x() - d->pressedPos.x();
        d->updateMouseArea(event);
    }

    void SectionBar::mouseReleaseEvent(QMouseEvent *event) {
        Q_D(SectionBar);
        if (event->button() == Qt::LeftButton) {
            d->pressed = false;
            d->deltaX = 0;

            if (d->targetTempo >= 0) {
                qDebug() << "tempo released" << d->curTempo << d->targetTempo;
            }
        }
        d->updateMouseArea(event);
    }

    void SectionBar::enterEvent(QEvent *event) {
    }

    void SectionBar::leaveEvent(QEvent *event) {
        Q_D(SectionBar);
        QMouseEvent e(QEvent::MouseMove, mapFromGlobal(QCursor::pos()), QCursor::pos(), Qt::NoButton, Qt::NoButton,
                      QApplication::keyboardModifiers());
        d->updateMouseArea(&e);
    }

    SectionBar::SectionBar(SectionBarPrivate &d, IProjectWindow *iWin, QWidget *parent)
        : QFrame(parent), IPianoRollComponent(iWin), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

} // namespace Core
