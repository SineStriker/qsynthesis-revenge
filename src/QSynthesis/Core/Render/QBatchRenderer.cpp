#include "QBatchRenderer.h"
#include "Utils/UtaPitchCurves.h"
#include "Utils/UtaTranslator.h"

#include <QPoint>

#include "Utau/QUstUtils.h"

static const char SLASH = '/';

QBatchRenderer::QBatchRenderer() {
}

QBatchRenderer::~QBatchRenderer() {
}

void QBatchRenderer::clear() {
    notes.clear();
    voice.clear();
    cache.clear();
    temp.clear();
    flags.clear();
    wavtool.clear();
    resampler.clear();
    tempBat.clear();
    helperBat.clear();
}

QList<NoteInfo> QBatchRenderer::noteInfos() const {
    return notes;
}

void QBatchRenderer::setNoteInfos(const QList<NoteInfo> &value) {
    notes = value;
}

QString QBatchRenderer::voiceDir() const {
    return voice;
}

void QBatchRenderer::setVoiceDir(const QString &dir) {
    voice = dir;
}

QString QBatchRenderer::cacheDir() const {
    return cache;
}

void QBatchRenderer::setCacheDir(const QString &dir) {
    cache = dir;
}

QString QBatchRenderer::audioName() const {
    return temp;
}

void QBatchRenderer::setAudioName(const QString &audio) {
    temp = audio;
}

QString QBatchRenderer::globalFlags() const {
    return flags;
}

void QBatchRenderer::setGlobalFlags(const QString &flags) {
    this->flags = flags;
}

QString QBatchRenderer::wavtoolPath() const {
    return wavtool;
}

void QBatchRenderer::setWavtoolPath(const QString &value) {
    wavtool = value;
}

QString QBatchRenderer::resamplerPath() const {
    return resampler;
}

void QBatchRenderer::setResamplerPath(const QString &value) {
    resampler = value;
}

QString QBatchRenderer::workingDir() const {
    return dir;
}

void QBatchRenderer::setWorkingDir(const QString &value) {
    dir = value;
}

QList<RenderArgs> QBatchRenderer::getArgs(QPoint range) const {
    QList<RenderArgs> args;

    int left = (range.x() < 0 || range.x() > notes.size() - 1) ? 0 : range.x();
    int right = (range.y() < 0 || range.y() > notes.size() - 1) ? (notes.size() - 1) : range.y();

    for (int i = left; i <= right; ++i) {
        auto aNote = notes.at(i).n;

        int aNoteNum = aNote.noteNum;
        int aLength = aNote.length;

        QString aLyric = aNote.lyric;
        QString aFlags = prop(aNote.flags, QUstUtils::Flags);

        double aTempo = QUstUtils::prop(aNote.tempo, QUstUtils::Tempo);
        double aIntensity = QUstUtils::prop(aNote.intensity, QUstUtils::Intensity);
        double aModulation = QUstUtils::prop(aNote.modulation, QUstUtils::Modulation);
        double aVelocity = QUstUtils::prop(aNote.velocity, QUstUtils::Velocity);

        QOtoItem aGenon = notes.at(i).g;
        QOtoItemBrief aCorrect = notes.at(i).c;
        QString aFilename;

        double aPreUttr = aCorrect.PreUtterance;
        double aOverlap = aCorrect.VoiceOverlap;
        double aStartPoint = aCorrect.StartPoint;

        QList<QUstPoint> aPitch;
        QList<QUstPoint> aEnvelope;
        QList<double> aVibrato;

        // Compute Mode2 Pitch Bend
        QList<QUstPoint> aPrevPitch;
        QList<double> aPrevVibrato;
        QString aPrevLyric = "";

        int aPrevLength = 480;
        int aPrevNoteNum = aNoteNum;

        int aNextLength = 480;
        int aNextNoteNum;

        double aNextPreUttr = 0;
        double aNextOverlap = 0;

        QList<QUstPoint> aNextPitch;
        QList<double> aNextVibrato;

        if (aGenon.FileName.isEmpty()) {
            aFilename = voice + SLASH + aLyric + ".wav";
        } else {
            aFilename = voice + SLASH + aGenon.FileName;
        }

        // Previous Note
        if (i >= 0) {
            auto aPrevNote = notes.at(i - 1).n;
            aPrevLength = aPrevNote.length;
            aPrevLyric = aPrevNote.lyric;
            aPrevNoteNum = aPrevNote.noteNum;
            aPrevPitch = aPrevNote.Mode2Pitch; // Mode2 Pitch Control Points
            aPrevVibrato = aPrevNote.vibrato;  // Mode2 Vibrato

            if (!aPrevPitch.isEmpty() && i - 1 >= 0) {
                auto NoteBeforePrev = notes.at(i - 2).n;
                QString tmpLyric = NoteBeforePrev.lyric;
                int tmpNoteNum = NoteBeforePrev.noteNum;

                UtaTranslator::getCorrectPBSY(tmpNoteNum, tmpLyric, aPrevNoteNum,
                                              aPrevPitch.front());
            }
        }

        // Current Note
        {
            aPitch = aNote.Mode2Pitch;  // Mode2 Mode2 Pitch Control Points
            aEnvelope = aNote.envelope; // Envelope
            aVibrato = aNote.vibrato;   // Mode2 Vibrato

            // Correct the y coordinate of first point
            if (!aPitch.isEmpty()) {
                UtaTranslator::getCorrectPBSY(aPrevNoteNum, aPrevLyric, aNoteNum, aPitch.front());
            }
        }

        // Next Note
        if (i < notes.size() - 1) {
            auto aNextNote = notes.at(i + 1).n;

            QOtoItemBrief nextGenon = notes.at(i + 1).c;
            aNextNoteNum = aNextNote.noteNum; // Note Num
            aNextLength = aNextNote.length;   // Length

            aNextPreUttr = nextGenon.PreUtterance; // PreUtterance
            aNextOverlap = nextGenon.VoiceOverlap; // Overlap

            aNextPitch = aNextNote.Mode2Pitch; // Mode2 Mode2 Pitch Control Points
            aNextVibrato = aNextNote.vibrato;  // Mode2 Vibrato

            // Correct the y coordinate of first point
            if (!aNextPitch.isEmpty()) {
                UtaTranslator::getCorrectPBSY(aNoteNum, aLyric, aNextNoteNum, aNextPitch.front());
            }
        }

        if (aPitch.isEmpty()) {
            aPitch = UtaTranslator::getDefaultPitch(aPrevNoteNum, aPrevLyric, aNoteNum);
        }

        // Convert Mode2 to Mode1
        QList<int> aPitchValues;
        aPitchValues = UtaPitchCurves::convert_from_vector_point(
            aTempo, aPitch, aVibrato, aPreUttr, aStartPoint, aLength, aNextPitch, aNextVibrato,
            aNextPreUttr, aNextOverlap, aNextLength, aPrevPitch, aPrevVibrato, aPrevLength);

        // Real Length
        double aDuration = QUstUtils::TickToTime(aLength, aTempo); // convert from ticks
        double aDurationFix = aPreUttr - aNextPreUttr + aNextOverlap;

        double aRealLength = aDuration + aDurationFix + aStartPoint + 50;
        aRealLength = (aRealLength < aGenon.Constant) ? aGenon.Constant : aRealLength;
        aRealLength = int((aRealLength + 25) / 50) * 50;

        // Cache Name
        QString aToneName = QUstUtils::ToneNumToToneName(aNoteNum);
        QString aCacheName = QString::number(i) + "_" + UtaTranslator::fixFilename(aLyric) + "_" +
                             aToneName + "_" + QString::number(aLength) + ".wav";

        // Save Values to Class
        ResamplerArgs res;
        WavtoolArgs wav;

        QString aAbsoultCacheName = cache + SLASH + aCacheName;
        QString aFullFlags = UtaTranslator::fixFlags(flags + aFlags);

        // Resampler Arguments
        res.setGenonSettings(aGenon);      // Genon Settings
        res.setToneName(aToneName);        // Note Num
        res.setInFile(aFilename);          // Input File
        res.setOutFile(aAbsoultCacheName); // Output File
        res.setIntensity(aIntensity);      // Intensity
        res.setModulation(aModulation);    // Modulation
        res.setVelocity(aVelocity);        // Velocity
        res.setFlags(aFullFlags);          // Flags
        res.setTempo(aTempo);              // Tempo
        res.setPitchCurves(aPitchValues);  // Pitch
        res.setRealLength(aRealLength);    // Real Length
        res.setCorrectGenon(aCorrect);     // Useless...

        res.setSequence(i);

        // Wavtool Arguments
        wav.setInFile(aAbsoultCacheName);
        wav.setOutFile(temp); // Modify Later
        wav.setStartPoint(aStartPoint);
        wav.setLength(aLength);          // Out Duration Arg 1
        wav.setTempo(aTempo);            // Out Duration Arg 2
        wav.setCorrection(aDurationFix); // Out Duration Arg 3
        wav.setVoiceOverlap(aOverlap);
        wav.setEnvelope(aEnvelope);

        if (QUstUtils::isRestLyric(aLyric)) {
            wav.setRest(true);
        }

        // Add to list
        args.append(RenderArgs(res, wav));
    }
    return args;
}

bool QBatchRenderer::generateBatch(const QList<RenderArgs> &args) const {
#ifdef Q_OS_WINDOWS
    return generateWindowsBatch(args);
#else
    return generateUnixBatch(args);
#endif
}

bool QBatchRenderer::generateHelper() const {
#ifdef Q_OS_WINDOWS
    return generateWindowsHelper();
#else
    return generateUnixHelper();
#endif
}

QString QBatchRenderer::GetReativePath(const QString &filename, const QString &dirname) {
    QString folder = dirname;
    if (folder.rightRef(1) != SLASH) {
        folder += SLASH;
    }
    if (filename.size() <= folder.size() || !filename.startsWith(folder)) {
        return filename;
    } else {
        return filename.mid(folder.size());
    }
}
