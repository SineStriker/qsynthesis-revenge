#include "DspxSpec.h"
#include "DspxSpec_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include <QMDecoratorV2.h>

#include "DspxConst.h"
#include "ICore.h"
#include "Window/IProjectWindow.h"

namespace Core {

    DspxSpecPrivate::DspxSpecPrivate() {
    }

    DspxSpecPrivate::~DspxSpecPrivate() {
    }

    void DspxSpecPrivate::init() {
    }

    DspxSpec *m_instance = nullptr;

    DspxSpec::DspxSpec(QObject *parent) : DocumentSpec(*new DspxSpecPrivate(), "org.ChorusKit.dspx", parent) {
        m_instance = this;

        Q_D(DspxSpec);
        d->init();

        setIcon(QIcon(":/images/dspx.png"));
        setDisplayName([]() { return tr("OpenVPI DiffSinger Editor"); });
        setDescription([]() { return tr("Built-in"); });
    }

    DspxSpec::~DspxSpec() {
        m_instance = nullptr;
    }

    DspxSpec *DspxSpec::instance() {
        return m_instance;
    }

    QVersionNumber DspxSpec::currentVersion() {
        static QVersionNumber ver = QVersionNumber::fromString(Dspx::Version);
        return ver;
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

        if (!doc->recover(fileName)) {
            QMessageBox::critical(nullptr, tr("File Error"), doc->errorMessage());

            if (qApp->property("closeHomeOnOpen").toBool())
                QTimer::singleShot(0, iWin->window(), &QWidget::close);

            return false;
        }

        return true;
    }

} // Core