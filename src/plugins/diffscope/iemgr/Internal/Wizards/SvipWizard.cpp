#include "SvipWizard.h"

#include <QApplication>
#include <QFile>

#include <QMDecoratorV2.h>

#include <QNrbfStream.h>

namespace IEMgr ::Internal {

    SvipWizard::SvipWizard(QObject *parent) : IWizardFactory("iemgr.SvipWizard", parent) {
        setCategory("score.XiaoIce");
        setDisplayName([]() { return tr("SVIP file"); });
        setDescription([]() { return tr("XStudio 2.0 project file."); });
        setDisplayCategory([]() { return QApplication::translate("IEMgr::WizardCategory", "XiaoIce"); });
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

    bool SvipWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
        switch (feature) {
            case IWizardFactory::ImportProject: {
                QFile file(path);
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