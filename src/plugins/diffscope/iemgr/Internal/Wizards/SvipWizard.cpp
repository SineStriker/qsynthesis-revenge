#include "SvipWizard.h"

#include <QNrbfStream.h>

#include <QFile>

namespace IEMgr ::Internal {

    SvipWizard::SvipWizard(QObject *parent) : IWizardFactory("iemgr.SvipWizard", parent) {
        setDisplayName(tr("SVIP file"));
        setDescription(tr("XStudio 2.0 project file."));

        setCategory("score.XiaoIce");
        setDisplayCategory(tr("XiaoIce"));
    }

    SvipWizard::~SvipWizard() {
    }

    QString SvipWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QString("%1(%2)").arg(tr("XStudio Svip Files"), "*.svip") + ";;" + IWizardFactory::filter(feature);
    }

    IWizardFactory::Features SvipWizard::features() const {
        return {ImportProject | ExportProject};
    }

    bool SvipWizard::runWizard(Feature feature, IWizardContext *context) {
        switch (feature) {
            case IWizardFactory::ImportProject: {
                QFile file(context->path());
                if (!file.open(QIODevice::ReadOnly)) {
                    return false;
                }

                QByteArray bytes = file.readAll();
                file.close();

                QNrbfStream in(bytes);

                QString svipName;
                QString version;

                in >> svipName;
                in >> version;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                qDebug() << version;

                QNrbf::XSAppModel model;
                in >> model;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                qDebug() << model.trackList.size();
                for (const auto &track : qAsConst(model.trackList)) {
                    switch (track->type()) {
                        case QNrbf::XSITrack::Singing: {
                            auto singingTrack = track.dynamicCast<QNrbf::XSSingingTrack>();
                            for (const auto &note : qAsConst(singingTrack->noteList)) {
                                qDebug() << note.lyric;
                            }
                            break;
                        }
                        case QNrbf::XSITrack::Instrument:
                            break;
                        default:
                            break;
                    }
                }

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

}