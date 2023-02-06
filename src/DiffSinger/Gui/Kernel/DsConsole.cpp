#include "DsConsole.h"
#include "private/DsConsole_p.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include "QsFileManager.h"
#include "QsSystem.h"

#include "CStartInfo.h"
#include "QDspxModel.h"

static const char FILE_EXTENSIONS_DELIMITER[] = ";;";

static const char OPEN_FLAG[] = "%PROJ%";
static const char IMPORT_FLAG[] = "%IMPORT%";

DsConsole::DsConsole(QObject *parent) : DsConsole(*new DsConsolePrivate(), parent) {
}

DsConsole::~DsConsole() {
}

void DsConsole::reloadStrings() {
    Q_D(DsConsole);
    d->fileDlgFilter_project =
        QStringList{
            tr("DiffScope Files(*.dspx)"),                                 //
            QString("%1(%2)").arg(tr("All Files"), QsOs::allFilesFilter()) //
        }
            .join(FILE_EXTENSIONS_DELIMITER);

    d->fileDlgFilter_import =
        QStringList{
            tr("Standard MIDI Files(*.mid)"),                              //
            tr("UTAU Sequence Texts(*.ust)"),                              //
            tr("OpenSVIP Model Files(*.json)"),                            //
            tr("XStudio SVIP Files(*.svip)"),                              //
            QString("%1(%2)").arg(tr("All Files"), QsOs::allFilesFilter()) //
        }
            .join(FILE_EXTENSIONS_DELIMITER);
}

void DsConsole::aboutApp(QWidget *parent) {
    QString title = tr("About %1").arg(qAppName());
    QString text =
        tr("%1 %2, Copyright OpenVPI.").arg(qAppName(), QApplication::applicationVersion());
    CConsole::MsgBox(parent, Information, title, text);
    // QMessageBox::information(parent, title,text);
}

bool DsConsole::openFile(QDspxModel *dspx, QWidget *parent) {
    Q_D(DsConsole);
    QString path =
        qsFileMgr->openFile(tr("Open file"), d->fileDlgFilter_project, OPEN_FLAG, parent);
    if (path.isEmpty()) {
        return false;
    }
    qDebug() << path;
    return true;
}

bool DsConsole::importFile(QDspxModel *dspx, QWidget *parent) {
    Q_D(DsConsole);
    QString path =
        qsFileMgr->openFile(tr("Import file"), d->fileDlgFilter_import, IMPORT_FLAG, parent);
    if (path.isEmpty()) {
        return false;
    }

    auto ext = QsFs::PathFindSuffix(path).toLower();
    if (ext == "mid") {
        return QDspx::fromMidi(path, dspx);
    }
    if (ext == "ust") {
        return QDspx::fromUst(path, dspx, parent);
    }
    if (ext == "json") {
        return QDspx::fromOpenSVIP(path, dspx, parent);
    }
    if (ext == "svip") {
        return QDspx::fromSvip(path, dspx, parent);
    }

    return true;
}

DsConsole::DsConsole(DsConsolePrivate &d, QObject *parent) : CConsole(d, parent) {
    d.init();
}
