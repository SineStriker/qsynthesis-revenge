#ifndef MUSICMATH_H
#define MUSICMATH_H

namespace MusicMath {

    double TickToMillisecond(double tick, double BPM, int beatUnit, int resolution);

    int MillisecondToTick(double ms, double BPM, int beatUnit, int resolution);

    double SinEasingInOut(double x0, double x1, double y0, double y1, double x);

    double SinEasingInOutX(double x0, double x1, double y0, double y1, double y);

    double SinEasingIn(double x0, double x1, double y0, double y1, double x);

    double SinEasingInX(double x0, double x1, double y0, double y1, double y);

    double SinEasingOut(double x0, double x1, double y0, double y1, double x);

    double SinEasingOutX(double x0, double x1, double y0, double y1, double y);

    double Linear(double x0, double x1, double y0, double y1, double x);

    double LinearX(double x0, double x1, double y0, double y1, double y);

     double DecibelToLinear(double db);

     double LinearToDecibel(double v);

     double ToneToFreq(int tone);

     double ToneToFreq(double tone);

     double FreqToTone(double freq);

}; // namespace MusicMath

#endif // MUSICMATH_H
