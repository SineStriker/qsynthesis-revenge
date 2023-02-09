#include "DsConsole.h"
#include "private/DsConsole_p.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include "QsFileManager.h"
#include "QsSystem.h"
#include "QsView.h"

#include "CStartInfo.h"
#include "ProjectImport.h"

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
#ifdef Q_OS_WINDOWS
    CConsole::MsgBox(parent, Information, title, text);
#else
    QMessageBox::information(parent, title, text);
#endif
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
        return Import::loadMidi(path, dspx, parent);
    }
    if (ext == "ust") {
        return Import::loadUst(path, dspx, parent);
    }
    if (ext == "json") {
        return Import::loadOpenSVIP(path, dspx, parent);
    }
    if (ext == "svip") {
        return Import::loadSvip(path, dspx, parent);
    }

    return true;
}

DsConsole::DsConsole(DsConsolePrivate &d, QObject *parent) : QsConsole(d, parent) {
    d.init();
}
