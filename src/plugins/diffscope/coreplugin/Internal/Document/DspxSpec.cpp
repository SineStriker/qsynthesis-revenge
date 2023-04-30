#include "DspxSpec.h"

#include "ICore.h"
#include "Window/IProjectWindow.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>

namespace Core::Internal {

    DspxSpec::DspxSpec(QObject *parent) : DocumentSpec("org.ChorusKit.dspx", parent) {
        setIcon(QIcon(":/images/dspx.png"));
    }

    DspxSpec::~DspxSpec() {
    }

    QStringList DspxSpec::supportedExtensions() const {
        return {"dspx"};
    }

    QString DspxSpec::filter() const {
        return QString("%1(%2)").arg(tr("DiffScope Project Files"), "*.dspx");
    }

    bool DspxSpec::open(const QString &fileName) {
        if (DocumentSpec::open(fileName))
            return true;

        auto iWin = ICore::instance()->windowSystem()->createWindow("project")->cast<IProjectWindow>();
        if (!iWin)
            return false;

        auto doc = iWin->doc();
        if (!doc->open(fileName)) {
            QMessageBox::critical(nullptr, tr("File Error"), doc->errorMessage());
            iWin->window()->close();
            return false;
        }

        return true;
    }

    bool DspxSpec::canRecover() const {
        return true;
    }

    bool DspxSpec::recover(const QString &logDir, const QString &fileName) {
        auto iWin = ICore::instance()->windowSystem()->createWindow("project")->cast<IProjectWindow>();
        if (!iWin)
            return false;

        auto doc = iWin->doc();
        doc->setLogDirectory(logDir);

        if (!doc->open(fileName)) {
            QMessageBox::critical(nullptr, tr("File Error"), doc->errorMessage());

            if (qApp->property("closeHomeOnOpen").toBool())
                iWin->window()->close();

            return false;
        }

        return true;
    }

} // Core