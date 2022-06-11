#ifndef QBATCHRENDERER_H
#define QBATCHRENDERER_H

#include "../QUtauBasic.h"
#include "Args/RenderArgs.h"

struct NoteInfo {
    QLinkNote n;
    QGenonSettings g;
    QCorrectGenon c;
};

class QBatchRenderer {
public:
    QBatchRenderer();
    ~QBatchRenderer();

public:
    void clear();

    QList<NoteInfo> noteInfos() const;
    void setNoteInfos(const QList<NoteInfo> &value);

    QString voiceDir() const;
    void setVoiceDir(const QString &dir);

    QString cacheDir() const;
    void setCacheDir(const QString &dir);

    QString audioName() const;
    void setAudioName(const QString &audio);

    QString globalFlags() const;
    void setGlobalFlags(const QString &flags);

    QString wavtoolPath() const;
    void setWavtoolPath(const QString &value);

    QString resamplerPath() const;
    void setResamplerPath(const QString &value);

    QString workingDir() const;
    void setWorkingDir(const QString &value);

public:
    QList<RenderArgs> getArgs(QPoint range) const;

    bool generateBatch(const QList<RenderArgs> &args) const;
    bool generateHelper() const;

private:
    QList<NoteInfo> notes;

    QString voice;
    QString cache;
    QString temp;
    QString flags;

    QString wavtool;
    QString resampler;

    QString tempBat;
    QString helperBat;

    QString dir;

#ifdef Q_OS_WINDOWS
    bool generateWindowsBatch(const QList<RenderArgs> &args) const;
    bool generateWindowsHelper() const;
#else
    bool generateUnixBatch(const QList<RenderArgs> &args) const;
    bool generateUnixHelper() const;
#endif

private:
    static QString GetReativePath(const QString &filename, const QString &dirname);
};

#endif // QBATCHRENDERER_H
