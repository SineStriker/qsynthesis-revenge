
#pragma once

#include "intervaltree.hpp"
#include <QFrame>
#include <QScrollBar>
#include <tuple>

class F0Widget : public QFrame {
    Q_OBJECT
public:
    explicit F0Widget(QWidget *parent = nullptr);
    ~F0Widget();

    void setDsSentenceContent(const QJsonObject &content);
    void setErrorStatusText(const QString &text);

    void clear();

    struct ReturnedDsString {
        QString note_seq;
        QString note_dur;
        QString note_slur;
    };
    ReturnedDsString getSavedDsStrings();

public slots:
    void setPlayheadPos(double pos);

    static int NoteNameToMidiNote(const QString &noteName);
    static QString MidiNoteToNoteName(int midiNote);
    static double FrequencyToMidiNote(double f);

protected:
    // Protected methods
    std::tuple<size_t, size_t> refF0IndexRange(double startTime, double endTime);

    // Events
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    // Stored DS file data
    struct MiniNote {
        double duration;
        int pitch; // Semitone from A0
        QString text;
        bool isSlur;

        // Required by IntervalTree
        bool operator<(const MiniNote &other) const {
            return duration < other.duration;
        }
    };
    Intervals::IntervalTree<double, MiniNote> midiIntervals;
    Intervals::IntervalTree<double> markerIntervals;
    QVector<double> f0Values;
    double f0Timestep;

    // Size constants
    static constexpr int KeyWidth = 60, ScrollBarWidth = 25;
    static constexpr int TimeAxisHeight = 40, HorizontalScrollHeight = ScrollBarWidth,
                         TimelineHeight = TimeAxisHeight + HorizontalScrollHeight;
    static constexpr int NotePadding = 4;

    // Display state
    double semitoneHeight = 30.0, secondWidth = 200.0;
    double centerPitch = 60.0, centerTime = 0.0;
    std::tuple<double, double> pitchRange = {0.0, 0.0}, timeRange = {0.0, 0.0};
    double playheadPos = 0.0;

    bool clampPitchToF0Bounds = true; // Also affects scroll bar range

    bool hasError;
    QString errorStatusText;

    // Embedded widgets
    QScrollBar *horizontalScrollBar, *verticalScrollBar;

private:
    // Private unified methods
    void setTimeAxisCenterAndSyncScrollbar(double time);
    void setF0CenterAndSyncScrollbar(double pitch);

private slots:
    void onHorizontalScroll(int value);
    void onVerticalScroll(int value);
};
