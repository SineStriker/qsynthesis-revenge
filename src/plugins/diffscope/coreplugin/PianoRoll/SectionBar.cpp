#include "SectionBar.h"
#include "SectionBar_p.h"

#include <QApplication>
#include <QComboBox>
#include <QDialog>
#include <QFontMetrics>
#include <QLabel>
#include <QLineEdit>
#include <QPaintEvent>

#include <cmath>

#include "CLineEdit.h"
#include "QMSystem.h"

#include "ICore.h"

namespace Core {

    class EditDialog : public QDialog {
    public:
        EditDialog(QWidget *parent = nullptr) : QDialog(parent) {
            setObjectName("section-edit-popup");
            setWindowFlags(Qt::Popup);
        }
        bool event(QEvent *event) override {
            switch (event->type()) {
                case QEvent::KeyPress:
                case QEvent::ShortcutOverride: {
                    auto e = static_cast<QKeyEvent *>(event);
                    int key = e->key();
                    switch (key) {
                        case Qt::Key_Enter:
                        case Qt::Key_Return:
                            accept();
                            break;
                        case Qt::Key_Escape:
                            reject();
                            break;
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
            return QDialog::event(event);
        }
    };

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
        hoverDataType = TempDataType::None;
        dragDataType = TempDataType::None;
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

    QTypeMap SectionBarPrivate::styleData_helper() const {
        return {
            {"sectionNumber",       QVariant::fromValue(sectionNumber)      },
            {"signatureNumber",     QVariant::fromValue(signatureNumber)    },
            {"signatureBackground", QVariant::fromValue(signatureBackground)},
            {"tempoNumber",         QVariant::fromValue(tempoNumber)        },
            {"tempoBackground",     QVariant::fromValue(tempoBackground)    },
            {"sectionLine",         QVariant::fromValue(sectionLine)        },
            {"beatLine",            QVariant::fromValue(beatLine)           },
            {"tempoLine",           QVariant::fromValue(tempoLine)          },
        };
    }

    void SectionBarPrivate::setStyleData_helper(const QTypeMap &map) {
        auto decodeStyle = [](const QVariant &var, auto &val) {
            using Type = decltype(typename std::remove_reference<decltype(val)>::type());
            if (var.canConvert<Type>()) {
                val = var.value<Type>();
            }
        };

        decodeStyle(map["sectionNumber"], sectionNumber);
        decodeStyle(map["signatureNumber"], signatureNumber);
        decodeStyle(map["signatureBackground"], signatureBackground);
        decodeStyle(map["tempoNumber"], tempoNumber);
        decodeStyle(map["tempoBackground"], tempoBackground);
        decodeStyle(map["sectionLine"], sectionLine);
        decodeStyle(map["beatLine"], beatLine);
        decodeStyle(map["tempoLine"], tempoLine);
    }

    void SectionBarPrivate::edit_sectionClicked() {
        edit_sectionRightClicked();
    }

    void SectionBarPrivate::edit_sectionRightClicked() {
        Q_Q(SectionBar);

        auto menu = ICore::createCoreMenu(q);

        QAction addAction(tr("Add time signature here"));
        QAction editAction(tr("Edit time signature here"));
        if (timeline->timeSignatureMap().contains(hoverData.curSectionNum)) {
            menu->addAction(&editAction);
        } else {
            menu->addAction(&addAction);
        }

        auto action = menu->exec(QCursor::pos());
        if (action == &addAction || action == &editAction) {
            // Add new time signature
            q->changeTimeSignatureAt(hoverData.curSectionNum);
        }
    }

    void SectionBarPrivate::edit_sectionDoubleClicked() {
    }

    void SectionBarPrivate::edit_timeSignatureClicked() {
    }

    void SectionBarPrivate::edit_timeSignatureRightClicked() {
        Q_Q(SectionBar);
        auto menu = ICore::createCoreMenu(q);

        QAction removeAction(tr("Remove"));
        QAction editAction(tr("Edit"));

        if (hoverData.curTimeSignature > 0)
            menu->addAction(&removeAction);
        menu->addAction(&editAction);

        auto action = menu->exec(QCursor::pos());
        if (action == &removeAction) {
            // Remove
        } else if (action == &editAction) {
            // Edit
            q->changeTimeSignatureAt(timeSignatureBlocks.value(hoverData.curTimeSignature).section);
        }
    }

    void SectionBarPrivate::edit_timeSignatureDoubleClicked() {
        Q_Q(SectionBar);

        q->changeTimeSignatureAt(timeSignatureBlocks.value(hoverData.curTimeSignature).section);

        //        const auto &block = timeSignatureBlocks.value(curTimeSignature);
        //
        //        EditDialog dlg(q);
        //        auto lineEdit = new CLineEdit();
        //        lineEdit->setObjectName("time-signature-text");
        //        lineEdit->setText(QString::number(block.value.numerator()));
        //
        //        auto combo = new QComboBox();
        //        combo->setView(new QListView());
        //        ICore::autoPolishScrollArea(combo);
        //        combo->setObjectName("time-signature-combo");
        //        combo->addItems({"2", "4", "8", "16", "32"});
        //        combo->setCurrentText(QString::number(block.value.denominator()));
        //
        //        auto label = new QLabel("/");
        //        label->setObjectName("time-signature-label");
        //
        //        QIntValidator validator;
        //
        //        auto layout = new QHBoxLayout();
        //        layout->setMargin(0);
        //        layout->setSpacing(0);
        //        layout->addWidget(lineEdit);
        //        layout->addWidget(label);
        //        layout->addWidget(combo);
        //        layout->setStretch(0, 1);
        //        layout->setStretch(2, 1);
        //
        //        //        double ratio = QGuiApplication::primaryScreen()->logicalDotsPerInch() / QMOs::unitDpi();
        //
        //        dlg.setLayout(layout);
        //        dlg.adjustSize();
        //        dlg.move(q->mapToGlobal(QPoint(block.rect.left(), block.rect.center().y() - dlg.height() / 2)));
        //
        //        lineEdit->setFocus();
        //        lineEdit->setValidator(&validator);
        //        combo->setValidator(&validator);
        //        dlg.exec();
    }

    void SectionBarPrivate::edit_tempoClicked() {
    }

    void SectionBarPrivate::edit_tempoRightClicked() {
        Q_Q(SectionBar);
        auto menu = ICore::createCoreMenu(q);

        QAction removeAction(tr("Remove"));
        QAction editAction(tr("Edit"));
        if (hoverData.curTempo > 0)
            menu->addAction(&removeAction);
        menu->addAction(&editAction);

        auto action = menu->exec(QCursor::pos());
        if (action == &removeAction) {
            // Remove
        } else if (action == &editAction) {
            // Edit
            q->changeTempoAt(hoverData.curTempo);
        }
    }

    void SectionBarPrivate::edit_tempoDoubleClicked() {
        Q_Q(SectionBar);

        const auto &block = tempoBlocks.value(dragData.orgTempo);

        EditDialog dlg(q);
        auto lineEdit = new CLineEdit();
        lineEdit->setObjectName("tempo-text");
        lineEdit->setText(QString::number(block.value));

        QFontMetrics fm(lineEdit->font());
        auto offset = fm.horizontalAdvance(" ") * 2;
        auto adjust = [&](const QString &text) {
            dlg.resize(fm.horizontalAdvance(text) + offset * 4, lineEdit->height()); //
        };
        connect(lineEdit, &QLineEdit::textChanged, &dlg, adjust);

        QDoubleValidator validator(0.0, std::numeric_limits<double>::max(), 3);
        validator.setNotation(QDoubleValidator::StandardNotation);

        auto layout = new QHBoxLayout();
        layout->setMargin(0);
        layout->addWidget(lineEdit);

        dlg.setLayout(layout);
        dlg.adjustSize();
        dlg.move(q->mapToGlobal(QPoint(block.rect.left(), block.rect.center().y() - dlg.height() / 2)));

        lineEdit->setFocus();
        lineEdit->setValidator(&validator);
        adjust(lineEdit->text());
        dlg.exec();

        // Result
        qDebug() << dlg.result() << lineEdit->text();
    }

    void SectionBarPrivate::edit_tempoMoved() {
    }

    void SectionBarPrivate::edit_blankRightClicked() {
        Q_Q(SectionBar);

        auto pos = q->mapFromGlobal(QCursor::pos());
        int tick = startPos + double(pos.x()) / curWidth * 480;
        auto time = timeline->tickToTime(tick);

        auto menu = ICore::createCoreMenu(q);

        QAction addTempoAction(tr("Add tempo"));
        menu->addAction(&addTempoAction);

        QAction addTimeSigAction(tr("Add time signature"));
        QAction editTimeSigAction(tr("Edit time signature"));

        if (timeline->timeSignatureMap().contains(time.measure())) {
            menu->addAction(&editTimeSigAction);
        } else {
            menu->addAction(&addTimeSigAction);
        }

        auto action = menu->exec(QCursor::pos());
        if (action == &addTempoAction) {
            q->changeTempoAt(tick);
        } else if (action == &addTimeSigAction || action == &editTimeSigAction) {
            q->changeTimeSignatureAt(time.measure());
        }
    }

    void SectionBarPrivate::updatePlayHead() {
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
        Q_D(const SectionBar);
        return d->styleData_helper();
    }

    void SectionBar::setStyleData(const QTypeMap &map) {
        Q_D(SectionBar);
        d->setStyleData_helper(map);
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

    void SectionBar::changeTimeSignatureAt(int bar) {
        qDebug() << "change time signature at" << bar;
    }

    void SectionBar::changeTempoAt(int tick) {
        qDebug() << "change tempo at" << tick;
    }

    void SectionBar::paintEvent(QPaintEvent *event) {
        Q_D(SectionBar);

        QFrame::paintEvent(event);

        d->hoverDataType = SectionBarPrivate::TempDataType::None;

        QPainter painter(this);

        double unit = double(d->curWidth) / 480;
        const auto &changedRect = event->rect();
        // const auto &changedRect = this->rect();

        int startPos = d->startPos + changedRect.left() / unit;
        int endPos = d->startPos + changedRect.right() / unit;
        int h = height();

        // Previous time signature
        struct Bar {
            int bar;
            int tick;
            QsApi::MusicTimeSignature timeSignature;
            bool hasSignature;
        };

        auto drawSectionNum = [&](const QRect &r0, int section) {
            bool hover = false;
            bool pressed = false;
            if (d->dragDataType == SectionBarPrivate::TempDataType::SectionNum) {
                if (d->dragData.orgSectionNum == section) {
                    pressed = true;
                }
            } else if (d->dragDataType == SectionBarPrivate::TempDataType::None) {
                if (r0.contains(mapFromGlobal(QCursor::pos()))) {
                    if (d->pressed && d->deltaX == 0) {
                        pressed = true;
                        d->dragDataType = SectionBarPrivate::TempDataType::SectionNum;
                        d->dragData.orgSectionNum = section;
                    } else {
                        hover = true;
                    }

                    d->hoverDataType = SectionBarPrivate::TempDataType::SectionNum;
                    d->hoverData.curSectionNum = section;
                }
            }

            if (pressed) {
                painter.setPen(d->sectionNumber.color3());
                painter.setBrush(d->sectionBackground.color3());
            } else if (hover) {
                painter.setPen(d->sectionNumber.color2());
                painter.setBrush(d->sectionBackground.color2());
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
            painter.drawText(r0, Qt::AlignCenter, QString::number(section));
        };

        auto drawTimeSig = [&](const QRect &r0, int tick, const QsApi::MusicTimeSignature &ts) {
            bool hover = false;
            bool pressed = false;
            if (d->dragDataType == SectionBarPrivate::TempDataType::TimeSignature) {
                if (d->hoverData.curTimeSignature == tick) {
                    pressed = true;
                }
            } else if (d->dragDataType == SectionBarPrivate::TempDataType::None) {
                if (r0.contains(mapFromGlobal(QCursor::pos()))) {
                    if (d->pressed && d->deltaX == 0) {
                        pressed = true;
                        d->dragDataType = SectionBarPrivate::TempDataType::TimeSignature;
                    } else {
                        hover = true;
                    }
                    d->hoverDataType = SectionBarPrivate::TempDataType::TimeSignature;
                    d->hoverData.curTimeSignature = tick;
                }
            }

            if (pressed) {
                painter.setPen(d->signatureNumber.color3());
                painter.setBrush(d->signatureBackground.color3());
            } else if (hover) {
                painter.setPen(d->signatureNumber.color2());
                painter.setBrush(d->signatureBackground.color2());
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
            bool hover = false;
            bool pressed = false;
            if (d->dragDataType == SectionBarPrivate::TempDataType::Tempo) {
                if (d->dragData.orgTempo == tick) {
                    pressed = true;

                    int toX = r0.left() + d->deltaX;
                    int toTick = toX / unit + startPos;
                    toTick = qMax(0, (toTick + d->curSnap / 2) / d->curSnap * d->curSnap);
                    toX = (toTick - startPos) * unit;
                    r0.moveLeft(toX);

                    d->dragData.targetTempo = toTick;
                }
            } else if (d->dragDataType == SectionBarPrivate::TempDataType::None) {
                if (r0.contains(mapFromGlobal(QCursor::pos()))) {
                    if (d->pressed && d->deltaX == 0) {
                        pressed = true;
                        d->dragDataType = SectionBarPrivate::TempDataType::Tempo;
                        d->dragData.orgTempo = tick;
                        d->dragData.targetTempo = -1;
                    } else {
                        hover = true;
                    }

                    d->hoverDataType = SectionBarPrivate::TempDataType::Tempo;
                    d->hoverData.curTempo = tick;
                }
            }

            if (pressed) {
                painter.setPen(d->tempoNumber.color3());
                painter.setBrush(d->tempoBackground.color3());
            } else if (hover) {
                painter.setPen(d->tempoNumber.color2());
                painter.setBrush(d->tempoBackground.color2());
            } else {
                painter.setPen(d->tempoNumber.color());
                painter.setBrush(d->tempoBackground.color());
            }

            if (r0.left() >= changedRect.left() || r0.right() < changedRect.right()) {
                auto pen = painter.pen();

                // Draw line
                painter.setPen(d->tempoLine);
                painter.drawLine(r0.left(), r0.top(), r0.left(), h);

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
            auto time = d->timeline->tickToTime(startPos); // start pos time
            int lastTimeSigPos = d->timeline->timeToTick(time.measure(), 0, 0);

            // Draw tempo
            const auto &tempoMap = d->timeline->tempoMap();
            {
                auto it1 = tempoMap.upperBound(time.measure()) - 1;
                for (auto it = it1; it != tempoMap.end(); ++it) {
                    if (it.key() >= endPos)
                        break;

                    int x2 = (it.key() - startPos) * unit + changedRect.left();

                    QFontMetrics fm(d->tempoNumber.font());
                    QString text = QString::number(it.value());
                    auto margins = d->tempoBackground.margins();
                    int rw = margins.left() + margins.right() + fm.horizontalAdvance(text);
                    int rh = fm.height() + margins.top() + margins.bottom();
                    QRect r0(x2, h / 4 - rh / 2, rw, rh);
                    if (r0.left() > changedRect.left() || r0.right() < changedRect.right()) {
                        d->tempoBlocks.insert(it.key(), {r0, it.key(), it.value()});
                    }
                }

                const SectionBarPrivate::Tempo *deferredBlock = nullptr;
                for (const auto &item : qAsConst(d->tempoBlocks)) {
                    if (d->dragDataType == SectionBarPrivate::TempDataType::Tempo &&
                        d->dragData.orgTempo == item.tick && d->deltaX != 0) {
                        deferredBlock = &item;
                        continue;
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
                int curTick = lastTimeSigPos;

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
                auto beatTicks = 1920 / bar.timeSignature.denominator();

                do {
                    // Draw section line
                    int x0 = (bar.tick - startPos) * unit + changedRect.left();
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
                            drawSectionNum(r0, curSection);
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
                        QRect r0(x1, h * 3 / 4 - rh / 2, rw, rh);

                        int maxW = beatTicks * unit + x0 - x1;
                        if (r0.width() > maxW) {
                            r0.setWidth(maxW);
                        }

                        if (r0.left() > changedRect.left() || r0.right() < changedRect.right()) {
                           d->timeSignatureBlocks.insert(bar.tick, {r0, curSection, bar.tick, bar.timeSignature});
                        }
                    }
                } while (false);

                // Draw beat line
                for (int i = 1; i < bar.timeSignature.numerator(); ++i) {
                    int x0 = (bar.tick + i * beatTicks - startPos) * unit + changedRect.left();
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
            d->dragDataType = SectionBarPrivate::TempDataType::None;

            d->updateMouseArea(event);

            switch (d->dragDataType) {
                case SectionBarPrivate::TempDataType::None:
                    d->updatePlayHead();
                    break;
                default:
                    break;
            }
        }
    }

    void SectionBar::mouseMoveEvent(QMouseEvent *event) {
        Q_D(SectionBar);

        if (d->pressed) {
            d->deltaX = event->pos().x() - d->pressedPos.x();

            d->updateMouseArea(event);

            switch (d->dragDataType) {
                case SectionBarPrivate::TempDataType::None:
                    d->updatePlayHead();
                    break;
                default:
                    break;
            }
        } else {
            d->updateMouseArea(event);
        }
    }

    void SectionBar::mouseReleaseEvent(QMouseEvent *event) {
        Q_D(SectionBar);
        if (event->button() == Qt::LeftButton) {
            if (d->deltaX == 0) {
                // After click
                switch (d->hoverDataType) {
                    case SectionBarPrivate::TempDataType::SectionNum:
                        d->edit_sectionClicked();
                        break;
                    case SectionBarPrivate::TempDataType::TimeSignature:
                        d->edit_timeSignatureClicked();
                        break;
                    case SectionBarPrivate::TempDataType::Tempo:
                        d->edit_tempoClicked();
                        break;
                    default: {
                        // Update play head
                        break;
                    }
                }
            } else {
                // After drag
                switch (d->dragDataType) {
                    case SectionBarPrivate::TempDataType::Tempo:
                        if (d->dragData.targetTempo >= 0) {
                            d->edit_tempoMoved();
                        }
                        break;
                    default: {
                        break;
                    }
                }
            }

            d->pressed = false;
            d->deltaX = 0;
            d->dragDataType = SectionBarPrivate::TempDataType::None;
        } else if (event->button() == Qt::RightButton) {
            if (d->deltaX == 0) {
                // After click
                switch (d->hoverDataType) {
                    case SectionBarPrivate::TempDataType::SectionNum:
                        d->edit_sectionRightClicked();
                        break;
                    case SectionBarPrivate::TempDataType::TimeSignature:
                        d->edit_timeSignatureRightClicked();
                        break;
                    case SectionBarPrivate::TempDataType::Tempo:
                        d->edit_tempoRightClicked();
                        break;
                    default: {
                        d->edit_blankRightClicked();
                        break;
                    }
                }
            }
        }

        d->updateMouseArea(event);
    }

    void SectionBar::mouseDoubleClickEvent(QMouseEvent *event) {
        Q_D(SectionBar);
        if (event->button() == Qt::LeftButton) {
            if (d->deltaX == 0) {
                switch (d->hoverDataType) {
                    case SectionBarPrivate::TempDataType::SectionNum:
                        d->edit_sectionDoubleClicked();
                        break;
                    case SectionBarPrivate::TempDataType::TimeSignature:
                        d->edit_timeSignatureDoubleClicked();
                        break;
                    case SectionBarPrivate::TempDataType::Tempo:
                        d->edit_tempoDoubleClicked();
                        break;
                    default: {
                        break;
                    }
                }
            }

            d->deltaX = 0;
            d->pressed = false;
            d->dragDataType = SectionBarPrivate::TempDataType::None;
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
