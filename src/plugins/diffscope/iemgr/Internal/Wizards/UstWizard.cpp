#include "UstWizard.h"

#include "Utau/QUstFile.h"
#include <QMessageBox>

#include <set>

#include <QMConsole.h>
#include <QMLinq.h>
#include <coreplugin/ICore.h>

#include "Utau/Utils/QUtaUtils.h"

#include "Internal/Utils/ImportDialog.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

namespace IEMgr ::Internal {

    UstWizard::UstWizard(QObject *parent) : IWizardFactory("iemgr.UstWizard", parent) {
        setDisplayName(tr("UST file"));
        setDescription(tr("Utau sequence text file."));

        setCategory("score.SimpleScore");
        setDisplayCategory(IWizardFactory::tr("Simple Score"));
    }

    UstWizard::~UstWizard() {
    }

    QString UstWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QString("%1(%2)").arg(tr("Utau Sequence Texts"), "*.ust") + ";;" + IWizardFactory::filter(feature);
    }

    IWizardFactory::Features UstWizard::features() const {
        return {ImportProject | ExportProject};
    }

    bool UstWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
        switch (feature) {
            case IWizardFactory::ImportProject: {
                QDspxModel model;
                if (!load(path, &model, parent)) {
                    return false;
                }
                qDebug() << model.content.tracks.size();
                break;
            }
            case IWizardFactory::ExportProject: {
                break;
            }
            default:
                break;
        }
        return true;
    }
    bool UstWizard::load(const QString &filename, QDspxModel *out, QWidget *parent) {
        QUstFile ust;

        if (!ust.load(filename)) {
            qmCon->MsgBox(parent, QMConsole::Critical, Core::ICore::mainTitle(), tr("Fail to open MIDI file!"));
            return false;
        }

        QList<QByteArray> byteLyrics; // key: pos; value: lyric;
        std::set<qint32> staticKeyNum;

        QFile file(filename);
        QByteArray fileName = file.fileName().section("/", -1, -1).toUtf8();
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Could not open file.";
            return false;
        }

        // 筛选Lyric行
        while (!file.atEnd()) {
            QByteArray line = file.readLine().trimmed(); // 去除行末换行符等空格字符
            if (line.startsWith("Lyric=")) {
                // 将行内剩余内容加入列表
                byteLyrics.append(line.mid(6)); // 从第6个字符开始取，避免将Lyric=也加入列表
            } else if (line.startsWith("NoteNum=")) {
                staticKeyNum.insert(line.mid(8).toInt());
            }
        }

        file.close();

        // 获取逻辑轨道音域
        QString ustPitchRange;
        if (!staticKeyNum.empty()) {
            ustPitchRange = QUtaUtils::ToneNumToToneName(*staticKeyNum.begin()) + "-" +
                            QUtaUtils::ToneNumToToneName(*staticKeyNum.rbegin());
        }

        ImportDialog::TrackInfo info(fileName, byteLyrics);

        info.format = tr("%1(%2): (%3 notes, %4)")
                          .arg("%1", QString::number(1), QString::number(byteLyrics.size()),
                               ustPitchRange.isEmpty() ? tr("None") : ustPitchRange);

        QList<qint32> selectID;
        QTextCodec *codec = nullptr;

        // 获取选中轨道
        {
            ImportDialog dlg(parent);
            dlg.setWindowTitle(tr("Import MIDI file"));

            ImportDialog::ImportOptions opt;
            opt.minTracks = 1;
            opt.maxTracks = 32;

            QList<qint32> logicIndexList;

            logicIndexList.append(1);
            opt.tracks.append(info);

            if (logicIndexList.empty()) {
                qmCon->MsgBox(parent, QMConsole::Warning, Core::ICore::mainTitle(),
                              tr("This file doesn't contain any notes."));
            }

            dlg.setImportOptions(opt);
            dlg.testUtf8();
            if (dlg.exec() == 0) {
                return false;
            }
            codec = dlg.codecResult();

            auto selectResult = dlg.selectResult();

            for (auto index : qAsConst(selectResult)) {
                selectID.append(logicIndexList[index]);
            }
        }

        ust.setCodec(codec);

        QDspxModel model;
        QDspx::Track track;

        QDspx::Timeline timeLine;

        auto clip = QDspx::SingingClipRef::create();


        auto ustNotes = ust.sectionNotes;
        int noteCursor = 0;
        for (const auto &ustNote : qAsConst(ustNotes)) {
            if (!QUtaUtils::isRestLyric(ustNote.lyric)) {
                QDspx::Note note;

                note.pos = noteCursor;
                note.length = ustNote.length;
                note.keyNum = ustNote.noteNum;
                note.lyric = ustNote.lyric;
                clip->notes.append(note);
            }
            if (ustNote.tempo > 0) {
                QDspx::Tempo tempo;
                tempo.pos = noteCursor;
                tempo.value = ustNote.tempo;
                timeLine.tempos.append(tempo);
            }
            if (!ustNote.label.isEmpty()) {
                QDspx::Label label;
                label.pos = noteCursor;
                label.text = ustNote.label;
                timeLine.labels.append(label);
            }
            noteCursor += ustNote.length;
        }

        QDspx::TimeSignature timeSignature;
        timeSignature.pos = 0;
        timeSignature.num = 4;
        timeSignature.den = 4;
        timeLine.timeSignatures.append(timeSignature);

        clip->time = QDspx::ClipTime(0, noteCursor, 0, noteCursor);
        clip->name = ust.sectionSettings.projectName;

        track.name = clip->name;
        track.clips.append(clip);
        model.content.timeline = timeLine;

        *out = std::move(model);

        return true;
    }

}