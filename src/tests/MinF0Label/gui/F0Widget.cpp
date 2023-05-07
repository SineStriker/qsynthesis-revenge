
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>


#include "DsSentence.h"
#include "F0Widget.h"
#include "qjsonstream.h"


F0Widget::F0Widget(QWidget *parent) : QFrame(parent) {
    hasError = false;
    assert(FrequencyToMidiNote(440.0) == 69.0);
    assert(FrequencyToMidiNote(110.0) == 45.0);
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

#if 0
    // Give trailing rests in note_seq a valid pitch
    for (int i = noteSeq.size() - 1; i >= 0 && noteSeq[i] == "rest"; i--) {
        if (noteSeq[i] == "rest") {
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
        midiIntervals.insert({noteBegin, noteBegin + note.duration, note});
        noteBegin += note.duration;
    }

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
    // phonemeIntervals.clear();
    // textIntervals.clear();
    update();
}

void F0Widget::setPlayheadPos(double time) {
    qDebug() << time;
    playheadPos = time;
    auto w = width() - KeyWidth;
    double leftTime = centerTime - w / 2 / secondWidth, rightTime = centerTime + w / 2 / secondWidth;
    if (time > rightTime || time < leftTime)
        centerTime = time + (rightTime - leftTime) / 2;
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
        grad.setColorAt(0, QColor(60, 60, 60));
        grad.setColorAt(1, QColor(40, 40, 40));
        painter.fillRect(0, 0, w, TimeAxisHeight, grad);
        painter.fillRect(0, TimeAxisHeight, w, MarkerAxisHeight, QColor(60, 60, 60));

        // Draw piano roll. Find the lowest drawn key from the center pitch
        painter.translate(KeyWidth, TimelineHeight);
        h = height() - TimelineHeight;
        w = width() - KeyWidth;
        painter.setClipRect(0, 0, w, h);

        painter.setPen(Qt::black);
        double keyReferenceY =
            h / 2 - (1 - ::fmod(centerPitch, semitoneHeight)) * semitoneHeight; // Top of center pitch's key
        int lowestPitch = ::floor(centerPitch) - ::ceil((h - keyReferenceY) / semitoneHeight);
        double lowestPitchY = keyReferenceY + (int(centerPitch - lowestPitch) + 0.5) * semitoneHeight;

        // Grid
        painter.setPen(QColor(80, 80, 80));
        for (int i = 0; i < 128; i++) {
            auto y = lowestPitchY + (i - lowestPitch) * semitoneHeight;
            if (y < 0 || y > h)
                continue;
            painter.drawLine(0, y, w, y);
        }

        // Midi notes
        auto leftTime = centerTime - w / 2 / secondWidth, rightTime = centerTime + w / 2 / secondWidth;

        painter.setPen(Qt::black);
        for (auto i : midiIntervals.findOverlappingIntervals({leftTime, rightTime}, false)) {
            if (i.value.pitch == 0)
                continue; // Skip rests (pitch 0)
            auto rec =
                QRectF((i.low - leftTime) * secondWidth, lowestPitchY - (i.value.pitch - lowestPitch) * semitoneHeight,
                       i.value.duration * secondWidth, semitoneHeight);
            if (rec.bottom() < 0 || rec.top() > h)
                continue;
            painter.fillRect(rec, QColor(200, 200, 255));
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

        // Piano keys
        do {
            lowestPitch++;
            lowestPitchY -= semitoneHeight;
            auto rec = QRectF(0, lowestPitchY, 60.0, semitoneHeight);
            auto color = keyColor[lowestPitch % 12];
            painter.fillRect(rec, color);
            painter.setPen(color == Qt::white ? Qt::black : Qt::white);
            painter.drawText(rec, Qt::AlignRight | Qt::AlignVCenter, MidiNoteToNoteName(lowestPitch));
        } while (lowestPitchY > 0);

        painter.translate(0, -TimelineHeight);
        painter.setClipRect(0, 0, w, height());
        h += TimelineHeight;

        // Playhead (playheadPos)
        painter.setPen(QColor(255, 180, 0));
        painter.drawLine((playheadPos - leftTime) * secondWidth + KeyWidth, 0,
                         (playheadPos - leftTime) * secondWidth + KeyWidth, h);
    } while (false);

    painter.end();

    QFrame::paintEvent(event);
}
