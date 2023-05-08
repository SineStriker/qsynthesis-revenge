
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>

#include <cmath>


#include "DsSentence.h"
#include "F0Widget.h"
#include "qjsonstream.h"


F0Widget::F0Widget(QWidget *parent) : QFrame(parent) {
    hasError = false;
    assert(FrequencyToMidiNote(440.0) == 69.0);
    assert(FrequencyToMidiNote(110.0) == 45.0);

    horizontalScrollBar = new QScrollBar(Qt::Horizontal, this);
    verticalScrollBar = new QScrollBar(Qt::Vertical, this);
    verticalScrollBar->setRange(60000, 60000);
    verticalScrollBar->setInvertedControls(true);
    verticalScrollBar->setInvertedAppearance(true);

    // Connect signals
    connect(horizontalScrollBar, &QScrollBar::valueChanged, this, &F0Widget::onHorizontalScroll);
    connect(verticalScrollBar, &QScrollBar::valueChanged, this, &F0Widget::onVerticalScroll);
}

F0Widget::~F0Widget() {
}

void F0Widget::setDsSentenceContent(const QJsonObject &content) {
    hasError = false;
    clear();

    DsSentence sentence;
    QAS::JsonStream stream(content);
    stream >> sentence;

    auto f0Seq = sentence.f0_seq.split(" ", QString::SkipEmptyParts);
    foreach (auto f0, f0Seq) {
        f0Values.append(FrequencyToMidiNote(f0.toDouble()));
    }
    f0Timestep = sentence.f0_timestep.toDouble();

    auto phSeq = sentence.ph_seq.split(" ", QString::SkipEmptyParts);
    auto phDur = sentence.ph_dur.split(" ", QString::SkipEmptyParts);
    auto phNum = sentence.ph_num.split(" ", QString::SkipEmptyParts);
    auto noteSeq = sentence.note_seq.split(" ", QString::SkipEmptyParts);
    auto noteDur = sentence.note_dur.split(" ", QString::SkipEmptyParts);
    auto text = sentence.text.split(" ", QString::SkipEmptyParts);
    auto slur = sentence.note_slur.split(" ", QString::SkipEmptyParts);

    QVector<bool> isRest(noteSeq.size(), false);

#if 1
    // Give trailing rests in note_seq a valid pitch
    for (int i = noteSeq.size() - 1; i >= 0 && noteSeq[i] == "rest"; i--) {
        if (noteSeq[i] == "rest") {
            isRest[i] = true;
            for (int j = i - 1; j >= 0; j--) {
                if (noteSeq[j] != "rest") {
                    noteSeq[i] = noteSeq[j];
                    break;
                }
            }
        }
    }

    // Search from beginning of note_seq and give rests a valid pitch after it
    for (int i = 0; i < noteSeq.size(); i++) {
        if (noteSeq[i] == "rest") {
            isRest[i] = true;
            for (int j = i + 1; j < noteSeq.size(); j++) {
                if (noteSeq[j] != "rest") {
                    noteSeq[i] = noteSeq[j];
                    break;
                }
            }
        }
    }
#endif

    auto noteBegin = 0.0;
    for (int i = 0; i < noteSeq.size(); i++) {
        MiniNote note;
        note.duration = noteDur[i].toDouble();
        note.pitch = NoteNameToMidiNote(noteSeq[i]);
        note.isSlur = slur[i].toInt();
        note.isRest = isRest[i];
        midiIntervals.insert({noteBegin, noteBegin + note.duration, note});
        noteBegin += note.duration;
    }

    // Update ranges
    std::get<1>(timeRange) = noteBegin;
    std::get<0>(pitchRange) = *std::min_element(f0Values.begin(), f0Values.end());
    std::get<1>(pitchRange) = *std::max_element(f0Values.begin(), f0Values.end());
    horizontalScrollBar->setMaximum(noteBegin * 1000);
    verticalScrollBar->setMaximum(std::get<1>(pitchRange) * 100);
    verticalScrollBar->setMinimum(std::get<0>(pitchRange) * 100);

    setF0CenterAndSyncScrollbar(f0Values.first());

    update();
}

void F0Widget::setErrorStatusText(const QString &text) {
    hasError = true;
    errorStatusText = text;
    update();
}

void F0Widget::clear() {
    hasError = false;
    errorStatusText = "";
    midiIntervals.clear();
    f0Values.clear();
    // phonemeIntervals.clear();
    // textIntervals.clear();
    update();
}

void F0Widget::setPlayheadPos(double time) {
    qDebug() << time;
    playheadPos = time;
    auto w = width() - KeyWidth - ScrollBarWidth;
    double leftTime = centerTime - w / 2 / secondWidth, rightTime = centerTime + w / 2 / secondWidth;
    if (time > rightTime || time < leftTime)
        setTimeAxisCenterAndSyncScrollbar(time + (rightTime - leftTime) / 2);
    update();
}

int F0Widget::NoteNameToMidiNote(const QString &noteName) {
    if (noteName == "rest")
        return 0;
    static const QMap<QString, int> noteNameToMidiNote = {
        {"C",  1 },
        {"C#", 2 },
        {"D",  3 },
        {"D#", 4 },
        {"E",  5 },
        {"F",  6 },
        {"F#", 7 },
        {"G",  8 },
        {"G#", 9 },
        {"A",  10},
        {"A#", 11},
        {"B",  12},
    };
    int octave = noteName.right(1).toInt();
    return 11 + 12 * octave + noteNameToMidiNote[noteName.left(noteName.size() - 1)];
}

QString F0Widget::MidiNoteToNoteName(int note) {
    // A0 = MIDI 21, C8 = 108. Return empty when out of range
    static const char *NoteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    if (note > 108 || note < 21)
        return QString();
    note -= 12;
    return QString("%1%2").arg(NoteNames[note % 12]).arg(note / 12);
}

double F0Widget::FrequencyToMidiNote(double freq) {
    return 69 + 12 * log2(freq / 440);
}

std::tuple<size_t, size_t> F0Widget::refF0IndexRange(double startTime, double endTime) {
    return {std::min((size_t)::floor(std::max(0.0, startTime / f0Timestep)), (size_t) f0Values.size() - 1),
            std::min((size_t)::ceil(endTime / f0Timestep), (size_t) f0Values.size() - 1)};
}

void F0Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    do {
        // Fill dark grey background
        painter.fillRect(rect(), QColor(40, 40, 40));

        // Convenience references
        auto w = width(), h = height();
        const Qt::GlobalColor keyColor[] = {Qt::white, Qt::black, Qt::white, Qt::black, Qt::white, Qt::white,
                                            Qt::black, Qt::white, Qt::black, Qt::white, Qt::black, Qt::white};

        // Print centered red error string if the error flag is set
        if (hasError) {
            painter.setPen(Qt::red);
            painter.drawText(rect(), Qt::AlignCenter, errorStatusText);
            break;
        }

        // Draw time axis and marker axis
        QLinearGradient grad(0, 0, 0, TimeAxisHeight);
        grad.setColorAt(0, QColor(40, 40, 40));
        grad.setColorAt(1, QColor(60, 60, 60));
        painter.fillRect(0, HorizontalScrollHeight, w, TimeAxisHeight, grad);

        // Draw piano roll. Find the lowest drawn key from the center pitch
        painter.translate(KeyWidth, TimelineHeight);
        h = height() - TimelineHeight;
        w = width() - KeyWidth - ScrollBarWidth;
        painter.setClipRect(0, 0, w, h);

        painter.setPen(Qt::black);
        double keyReferenceY = h / 2 - (1 - ::fmod(centerPitch, 1)) * semitoneHeight; // Top of center pitch's key
        int lowestPitch = ::floor(centerPitch) - ::ceil((h - keyReferenceY) / semitoneHeight);
        double lowestPitchY = keyReferenceY + (int(centerPitch - lowestPitch) + 0.5) * semitoneHeight;

        if (lowestPitch < 21) {
            lowestPitchY += (21 - lowestPitch) * semitoneHeight;
            lowestPitch = 21;
        }

        // Grid
        painter.setPen(QColor(80, 80, 80));
        {
            auto y = lowestPitchY;
            while (y > 0) {
                painter.drawLine(0, y, w, y);
                y -= semitoneHeight;
            }
        }

        // Midi notes
        auto leftTime = centerTime - w / 2 / secondWidth, rightTime = centerTime + w / 2 / secondWidth;

        static constexpr QColor NoteColors[] = {QColor(106, 164, 234), QColor(60, 113, 219)};
        for (auto i : midiIntervals.findOverlappingIntervals({leftTime, rightTime}, false)) {
            if (i.value.pitch == 0)
                continue; // Skip rests (pitch 0)
            auto rec =
                QRectF((i.low - leftTime) * secondWidth, lowestPitchY - (i.value.pitch - lowestPitch) * semitoneHeight,
                       i.value.duration * secondWidth, semitoneHeight);
            if (rec.bottom() < 0 || rec.top() > h)
                continue;
            if (!i.value.isRest) {
                painter.setPen(Qt::black);
                painter.fillRect(rec, NoteColors[i.value.isSlur]);
                painter.drawRect(rec);
            } else {
                auto pen = painter.pen();
                pen.setStyle(Qt::DashLine);
                pen.setColor(NoteColors[0]);
                painter.setPen(pen);
                painter.drawRect(rec);
            }
            // rec.adjust(NotePadding, NotePadding, -NotePadding, -NotePadding);
            // painter.drawText(rec, Qt::AlignVCenter | Qt::AlignLeft, i.value.text);
        }

        // F0 Curve
        if (!f0Values.empty()) {
            // lowestPitchY is the lowest (drawn) key's Top-left y coordinate
            // But F0 curve is drawn from the center of the key, so we need to adjust it
            lowestPitchY += semitoneHeight / 2;

            auto visibleF0 = refF0IndexRange(leftTime, rightTime);
            QPainterPath path;
            double f0X = (std::get<0>(visibleF0) - leftTime / f0Timestep) * f0Timestep * secondWidth,
                   f0Ybase = lowestPitchY + lowestPitch * semitoneHeight;
            path.moveTo(f0X, lowestPitchY - (f0Values[std::get<0>(visibleF0)] - lowestPitch) * semitoneHeight);
            for (int i = std::get<0>(visibleF0) + 1; i < std::get<1>(visibleF0); i++) {
                f0X += f0Timestep * secondWidth;
                path.lineTo(f0X, lowestPitchY - (f0Values[i] - lowestPitch) * semitoneHeight);
            }
            painter.setPen(Qt::red);
            painter.drawPath(path);

            lowestPitchY -= semitoneHeight / 2;
        }

        painter.translate(-KeyWidth, 0);
        painter.setClipRect(0, 0, width(), h);
        w += KeyWidth;

        // Debug text
        painter.setPen(Qt::white);
        painter.drawText(KeyWidth, TimelineHeight,
                         QString("CenterPitch %1 (%2)  LowestPitch %3 (%4)")
                             .arg(centerPitch)
                             .arg(MidiNoteToNoteName(centerPitch))
                             .arg(lowestPitch)
                             .arg(MidiNoteToNoteName(lowestPitch)));

        // Piano keys
        do {
            lowestPitch++;
            lowestPitchY -= semitoneHeight;
            auto rec = QRectF(0, lowestPitchY, 60.0, semitoneHeight);
            auto color = keyColor[lowestPitch % 12];
            painter.fillRect(rec, color);
            painter.setPen(color == Qt::white ? Qt::black : Qt::white);
            painter.drawText(rec, Qt::AlignRight | Qt::AlignVCenter, MidiNoteToNoteName(lowestPitch));
        } while (lowestPitchY > 0 && lowestPitch <= 108);

        painter.translate(0, -TimelineHeight);
        painter.setClipRect(0, 0, w, height());
        h += TimelineHeight;

        // Playhead (playheadPos)
        painter.setPen(QColor(255, 180, 0));
        painter.drawLine((playheadPos - leftTime) * secondWidth + KeyWidth, TimelineHeight,
                         (playheadPos - leftTime) * secondWidth + KeyWidth, h);
    } while (false);

    painter.end();

    QFrame::paintEvent(event);
}

void F0Widget::resizeEvent(QResizeEvent *event) {
    // Update the scrollbars
    auto w = width(), h = height();
    horizontalScrollBar->setGeometry(0, 0, w, HorizontalScrollHeight);
    horizontalScrollBar->setPageStep(w / secondWidth * 1000);
    verticalScrollBar->setGeometry(w - ScrollBarWidth, TimelineHeight, ScrollBarWidth, h - TimelineHeight);
    verticalScrollBar->setPageStep((h - TimelineHeight) / semitoneHeight * 100);

    QFrame::resizeEvent(event);
}

void F0Widget::setTimeAxisCenterAndSyncScrollbar(double time) {
    centerTime = std::clamp(time, std::get<0>(timeRange), std::get<1>(timeRange));
    horizontalScrollBar->setValue(centerTime * 1000);
}

void F0Widget::setF0CenterAndSyncScrollbar(double pitch) {
    centerPitch = clampPitchToF0Bounds ? pitch : std::clamp(std::get<0>(pitchRange), std::get<1>(pitchRange), pitch);
    verticalScrollBar->setValue(centerPitch * 100);
}

void F0Widget::onHorizontalScroll(int value) {
    centerTime = value / 1000.0;
    update();
}

void F0Widget::onVerticalScroll(int value) {
    centerPitch = value / 100.0;
    update();
}
