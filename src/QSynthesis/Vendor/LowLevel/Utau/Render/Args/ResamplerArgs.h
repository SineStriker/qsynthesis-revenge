#ifndef RESAMPLERARGS_H
#define RESAMPLERARGS_H

#include "../../Common/QGenonSettings.h"

#include <QDateTime>

class ResamplerArgs {
public:
    ResamplerArgs();
    ~ResamplerArgs();
    void clear();

    QString inFile() const;
    void setInFile(const QString &inFile);

    QString outFile() const;
    void setOutFile(const QString &outFile);

    QString toneName() const;
    void setToneName(const QString &toneName);
    void setToneName(int noteNum);

    double velocity() const;
    void setVelocity(double velocity);

    QString flags() const;
    void setFlags(const QString &flags);

    double intensity() const;
    void setIntensity(double intensity);

    double modulation() const;
    void setModulation(double modulation);

    QList<int> pitchCurves() const;
    void setPitchCurves(QList<int> pitches);

    double realLength() const;
    void setRealLength(double realLength);

    QGenonSettings genonSettings() const;
    void setGenonSettings(const QGenonSettings &genonSettings);

    QCorrectGenon correctGenon() const;
    void setCorrectGenon(const QCorrectGenon &correctGenon);

    bool toBase64() const;
    void setToBase64(bool toBase64);

    QStringList paramsArguments() const;
    QStringList fullArguments() const;

    double tempo() const;
    void setTempo(double tempo);

    int sequence() const;
    void setSequence(int sequence);

    bool operator==(const ResamplerArgs &another) const;
    bool operator!=(const ResamplerArgs &another) const;

    friend QDebug operator<<(QDebug debug, const ResamplerArgs &res);

private:
    QString m_inFile;
    QString m_outFile;

    int m_sequence;

    QString m_toneName;

    double m_velocity;

    QString m_flags;

    double m_intensity;
    double m_modulation;

    double m_realLength;

    QGenonSettings m_genonSettings;
    QCorrectGenon m_correctGenon;

    QList<int> m_pitchCurves;
    double m_tempo;

    bool m_toBase64;

private:
    QDateTime m_voiceTime;
    QDateTime m_frqTime;
    QDateTime m_mrqTime;
};

#endif // RESAMPLERARGS_H
