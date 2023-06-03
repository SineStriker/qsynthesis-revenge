#include "DspxSpec.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include <QMDecoratorV2.h>

#include "ICore.h"
#include "Window/IProjectWindow.h"

namespace Core::Internal {

    DspxSpec::DspxSpec(QObject *parent) : DocumentSpec("org.ChorusKit.dspx", parent) {
        setIcon(QIcon(":/images/dspx.png"));
        qIDec->installLocale(this, _LOC(DspxSpec, this));
    }

    DspxSpec::~DspxSpec() {
    }

    void DspxSpec::reloadStrings() {
        setDisplayName(tr("OpenVPI DiffSinger Editor"));
        setDescription(tr("Built-in"));
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
                QTimer::singleShot(0, iWin->window(), &QWidget::close);

            return false;
        }

        return true;
    }

} // Core