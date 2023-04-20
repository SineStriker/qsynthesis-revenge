#include "DspxSpec.h"

#include "ICore.h"

#include <QDebug>

namespace Core::Internal {

    DspxSpec::DspxSpec(QObject *parent) : DocumentSpec("org.ChorusKit.dspx", parent) {
    }

    DspxSpec::~DspxSpec() {
    }

    QStringList DspxSpec::supportedExtensions() const {
        return {"dspx"};
    }

    QString DspxSpec::filter() const {
        return QString("%1(%2)").arg(tr("DiffScope Project Files"), "*.dspx");
    }

    bool DspxSpec::open(const QString &filename) {
        qDebug() << "Open DSPX" << filename;

        ICore::instance()->windowSystem()->createWindow("project");

        //

        return true;
    }

} // Core