#include "QUstConverter.h"

#include "Utau/QUstFile.h"
#include "Utau/Utils/QUtaUtils.h"

#include <QTextCodec>

QUstConverter::QUstConverter(QObject *parent) : ISVSConverter(parent) {
}

QUstConverter::~QUstConverter() {
}

bool QUstConverter::load(const QString &filename, QSvipModel *out,
                         const QMap<QString, QVariant> &args) {
    QUstFile ust;
    {
        auto it = args.find("DefaultCharset");
        if (it != args.end()) {
            auto codec = QTextCodec::codecForName(it.value().toByteArray());
            if (codec) {
                ust.setCodec(codec);
            }
        }
    }

    if (!ust.load(filename)) {
        return false;
    }

    QSvipModel proj;
    {
        QSvipModel::SongTempo firstTempo;
        firstTempo.Position = 0;
        firstTempo.BPM = ust.sectionSettings.globalTempo;
        proj.SongTempoList.append(firstTempo);

        int curLength = 0;
        for (const auto &note : qAsConst(ust.sectionNotes)) {
            if (curLength != 0 && note.tempo != QUtaUtils::NODEF_DOUBLE) {
                QSvipModel::SongTempo curTempo;
                curTempo.Position = curLength;
                curTempo.BPM = note.tempo;
                proj.SongTempoList.append(curTempo);
            }
            curLength += note.length;
        }
    }

    // Parse Track
    {
        auto track = new QSvipModel::SingingTrack();

        int curLength = 0;
        for (const auto &note : qAsConst(ust.sectionNotes)) {
            if (!QUtaUtils::isRestLyric(note.lyric)) {
                QSvipModel::Note p;

                p.StartPos = curLength;
                p.Length = note.length;
                p.KeyNumber = note.noteNum;
                p.Lyric = note.lyric;

                track->NoteList.append(p);
            }
            curLength += note.length;
        }

        proj.TrackList.append(QSvipModel::TrackRef(track));
    }

    *out = proj;

    return true;
}

bool QUstConverter::save(const QString &filename, const QSvipModel &in,
                         const QMap<QString, QVariant> &args) {
    Q_UNUSED(args);
    return false;
}
