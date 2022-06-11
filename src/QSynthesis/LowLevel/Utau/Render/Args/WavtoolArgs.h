#ifndef WAVTOOLARGS_H
#define WAVTOOLARGS_H

#include "../../Common/QControlPoint.h"

class WavtoolArgs {
public:
    WavtoolArgs();
    ~WavtoolArgs();
    void clear();

    QString inFile() const;
    void setInFile(const QString &inFile);

    QString outFile() const;
    void setOutFile(const QString &outFile);

    double startPoint() const;
    void setStartPoint(double startPoint);

    double voiceOverlap() const;
    void setVoiceOverlap(double voiceOverlap);

    QList<QControlPoint> envelope() const;
    void setEnvelope(const QList<QControlPoint> &envelope);

    double tempo() const;
    void setTempo(double tempo);

    int length() const;
    void setLength(int length);

    double correction() const;
    void setCorrection(double correction);

    bool isRest() const;
    void setRest(bool rest);

    QString outDuration() const;
    QStringList envArguments() const;
    QStringList fullArguments() const;

    bool operator==(const WavtoolArgs &another) const;
    bool operator!=(const WavtoolArgs &another) const;

private:
    QString m_inFile;
    QString m_outFile;

    double m_startPoint;
    double m_voiceOverlap;

    double m_tempo;
    int m_length;
    double m_correction;

    QList<QControlPoint> m_envelope;

    bool m_rest;
};

#endif // WAVTOOLARGS_H
