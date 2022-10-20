#include "MusicMath.h"

#include <QtMath>

static const double a = qPow(2, 1.0 / 12);

double MusicMath::TickToMillisecond(double tick, double BPM, int beatUnit, int resolution) {
    return tick * 60000.0 / BPM * beatUnit / 4 / resolution;
}

int MusicMath::MillisecondToTick(double ms, double BPM, int beatUnit, int resolution) {
    return qCeil(ms / 60000.0 * BPM / beatUnit * 4 * resolution);
}

double MusicMath::SinEasingInOut(double x0, double x1, double y0, double y1, double x) {
    return y0 + (y1 - y0) * (1 - qCos((x - x0) / (x1 - x0) * M_PI)) / 2;
}

double MusicMath::SinEasingInOutX(double x0, double x1, double y0, double y1, double y) {
    return qAcos(1 - (y - y0) * 2 / (y1 - y0)) / M_PI * (x1 - x0) + x0;
}

double MusicMath::SinEasingIn(double x0, double x1, double y0, double y1, double x) {
    return y0 + (y1 - y0) * (1 - qCos((x - x0) / (x1 - x0) * M_PI / 2));
}

double MusicMath::SinEasingInX(double x0, double x1, double y0, double y1, double y) {
    return qAcos(1 - (y - y0) / (y1 - y0)) / M_PI * 2 * (x1 - x0) + x0;
}

double MusicMath::SinEasingOut(double x0, double x1, double y0, double y1, double x) {
    return y0 + (y1 - y0) * qSin((x - x0) / (x1 - x0) * M_PI / 2);
}

double MusicMath::SinEasingOutX(double x0, double x1, double y0, double y1, double y) {
    return qAsin((y - y0) / (y1 - y0)) / M_PI * 2 * (x1 - x0) + x0;
}

double MusicMath::Linear(double x0, double x1, double y0, double y1, double x) {
    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

double MusicMath::LinearX(double x0, double x1, double y0, double y1, double y) {
    return (y - y0) / (y1 - y0) * (x1 - x0) + x0;
}

double MusicMath::DecibelToLinear(double db) {
    return qPow(10, db / 20);
}

double MusicMath::LinearToDecibel(double v) {
    return std::log10(v) * 20;
}

double MusicMath::ToneToFreq(int tone) {
    return 440.0 * qPow(a, tone - 69);
}

double MusicMath::ToneToFreq(double tone) {
    return 440.0 * qPow(a, tone - 69);
}

double MusicMath::FreqToTone(double freq) {
    return std::log(freq / 440.0) / std::log(a) + 69;
}
