#include "OpenSvipWizard.h"
#include <QApplication>
#include <QMDecoratorV2.h>
#include "Svip/QSvipModel.h"
#include "QFile"
#include "QJsonDocument"

namespace IEMgr ::Internal {

    OpenSvipWizard::OpenSvipWizard(QObject *parent) : IWizardFactory("iemgr.OpenSvipWizard", parent) {
        setCategory("score.OpenVPI");
        qIDec->installLocale(this, _LOC(OpenSvipWizard, this));
    }

    OpenSvipWizard::~OpenSvipWizard() {
    }

    void OpenSvipWizard::reloadStrings() {
        setDisplayName(tr("OpenSVIP model"));
        setDescription(tr("OpenSVIP converter intermediate model."));
        setDisplayCategory(QApplication::translate("IEMgr::WizardCategory", "OpenVPI"));
    }

    QString OpenSvipWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QString("%1(%2)").arg(tr("OpenSVIP Json Files"), "*.json") + ";;" + IWizardFactory::filter(feature);
    }

    IWizardFactory::Features OpenSvipWizard::features() const {
        return {ImportProject | ExportProject};
    }

    bool OpenSvipWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
        switch (feature) {
            case IWizardFactory::ImportProject: {
                QDspxModel model;
                if (!load(path, &model, parent)) {
                    return false;
                }
                qDebug() << model.content.tracks.size();
                break;
            }
            case IWizardFactory::ExportProject:
                break;
            default:
                break;
        }
        return false;
    }

    bool OpenSvipWizard::load(const QString &filename, QDspxModel *out, QWidget *parent) {
        QJsonObject jsonObj;
        if(!loadProjectFile(filename, &jsonObj)) return false;
        auto osProject = QSvipModel::fromJsonObject(jsonObj);
        qDebug() << osProject.TrackList.size();
        for (const auto &track : qAsConst(osProject.TrackList)) {
            qDebug() << track->Title;
            switch (track->type()) {
                case QSvipModel::Track::Singing: {
                    auto singingTrack = track.dynamicCast<QSvipModel::SingingTrack>();
                    QString lyrics;
                    for (const auto &note : qAsConst(singingTrack->NoteList)) {
                        lyrics.append(note.Lyric);
                    }
                    qDebug() << lyrics;
                    break;
                }
                case QSvipModel::Track::Instrumental: {
                }
                    break;
                default:
                    break;
            }
        }
        return true;
    }

    bool OpenSvipWizard::loadProjectFile(const QString &filename, QJsonObject *out) {
        QFile loadFile(filename);
        if(!loadFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to open project file";
            return false;
        }
        QByteArray allData = loadFile.readAll();
        loadFile.close();
        QJsonParseError err;
        QJsonDocument json = QJsonDocument::fromJson(allData, &err);
        if (err.error != QJsonParseError::NoError)
            return false;
        if(json.isObject()) {
            *out = json.object();
        }
        return true;
    }

    bool OpenSvipWizard::save(const QString &filename, const QDspxModel &in, QWidget *parent) {
        return true;
    }

}