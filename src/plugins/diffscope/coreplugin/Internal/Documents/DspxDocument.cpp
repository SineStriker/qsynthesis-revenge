#include "DspxDocument.h"

#include <QDebug>

namespace Core::Internal {

    DspxSpec::DspxSpec(QObject *parent) : DocumentSpec("core.Dspx", parent) {
    }

    DspxSpec::~DspxSpec() {
    }

    QString DspxSpec::filter() const {
        return QString("%1(%2)").arg(tr("DiffScope Project Files"), "*.dspx");
    }

    bool DspxSpec::open(const QString &filename) {
        qDebug() << "Open DSPX" << filename;
        return false;
    }

} // Core