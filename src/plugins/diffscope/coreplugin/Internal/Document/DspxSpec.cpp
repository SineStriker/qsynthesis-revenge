#include "DspxSpec.h"

#include "ICore.h"
#include "Window/IProjectWindow.h"

#include <QDebug>
#include <QMessageBox>

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
        auto iWin = ICore::instance()->windowSystem()->createWindow("project")->cast<IProjectWindow>();
        if (!iWin)
            return false;

        auto doc = iWin->doc();
        if (!doc->open(filename)) {
            QMessageBox::critical(nullptr, tr("File Error"), doc->errorMessage());
            iWin->window()->close();
            return false;
        }

        return true;
    }

} // Core