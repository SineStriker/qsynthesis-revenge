#include "DsConsole.h"
#include "private/DsConsole_p.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include "QsFileManager.h"
#include "QMSystem.h"
#include "QMView.h"

#include "CStartInfo.h"
#include "QMLinq.h"

#include "Dialogs/PluginSetDialog.h"
#include "IDspxPorter.h"
#include "QsPluginManager.h"

DSAPI_USING_NAMESPACE

static const char FILE_EXTENSIONS_DELIMITER[] = ";;";

static const char OPEN_FLAG[] = "%PROJ%";
static const char IMPORT_FLAG[] = "%IMPORT%";

struct PluginsAndFilter {
    QMap<QString, IDspxPorter *> plugins;
    QString filters;
};

static auto GetPlugins() {
    // Get all plugins
    auto porters = qsPluginMgr->pluginSet("dspxporters")->plugins<IDspxPorter>();
    QMap<QString, IDspxPorter *> plugins;
    for (const auto &porter : qAsConst(porters)) {
        const auto &fmt = porter->format();
        for (const auto &suffix : fmt.suffixes) {
            plugins.insert(suffix.toLower(), porter);
        }
    }

    return PluginsAndFilter{
        plugins, (QMLinq::Select<IDspxPorter *, QString>(
                      porters,
                      [&](const IDspxPorter *val) {
                          const auto &fmt = val->format();
                          return QString("%1(%2)").arg(
                              fmt.name, QMLinq::Select<QString, QString>(
                                            fmt.suffixes, [](const QString &s) { return "*." + s; })
                                            .join(" ")

                          );
                      })
                  << QString("%1(%2)").arg(DsConsole::tr("All Files"), QMOs::allFilesFilter()))
                     .join(FILE_EXTENSIONS_DELIMITER)};
}

static auto GetDspxFilter() {
    return QStringList{
        DsConsole::tr("DiffScope Files(*.dspx)"),                                 //
        QString("%1(%2)").arg(DsConsole::tr("All Files"), QMOs::allFilesFilter()) //
    }
        .join(FILE_EXTENSIONS_DELIMITER);
}

DsConsole::DsConsole(QObject *parent) : DsConsole(*new DsConsolePrivate(), parent) {
}

DsConsole::~DsConsole() {
}

void DsConsole::reloadStrings() {
}

void DsConsole::aboutApp(QWidget *parent) {
    QString title = tr("About %1").arg(qAppName());
    QString text =
        tr("%1 %2, Copyright OpenVPI.").arg(qAppName(), QApplication::applicationVersion());
#ifdef Q_OS_WINDOWS
    QMConsole::MsgBox(parent, Information, title, text);
#else
    QMessageBox::information(parent, title, text);
#endif
}

void DsConsole::viewPlugins(QWidget *parent) {
    PluginSetDialog dlg(parent);
    dlg.exec();
}

bool DsConsole::openFile(QDspxModel *dspx, QWidget *parent) {
    QString path = qsFileMgr->openFile(tr("Open file"), GetDspxFilter(), OPEN_FLAG, parent);
    if (path.isEmpty()) {
        return false;
    }
    if (QMFs::PathFindSuffix(path).toLower() != "dspx") {
        return false;
    }
    return openFile(dspx, path, parent);
}

bool DsConsole::openFile(QDspxModel *model, const QString &filename, QWidget *parent) {
    if (!model->load(filename)) {
        qCs->MsgBox(parent, DsConsole::Critical, qIStup->mainTitle(),
                    tr("Fail to load file \"%1\"!").arg(filename));
        qsFileMgr->commitRecent(QsFileManager::Project, QsFileManager::Remove, filename);
        return false;
    }
    qsFileMgr->commitRecent(QsFileManager::Project, QsFileManager::Advance, filename);
    return true;
}

bool DsConsole::saveAsFile(const QDspxModel &dspx, const QString &nameHint, QWidget *parent) {
    QString path =
        qsFileMgr->saveFile(tr("Open file"), nameHint, GetDspxFilter(), OPEN_FLAG, parent);
    if (path.isEmpty()) {
        return false;
    }
    return saveFile(dspx, path, parent);
}

bool DsConsole::saveFile(const QDspxModel &model, const QString &filename, QWidget *parent) {
    if (!model.save(filename)) {
        qCs->MsgBox(parent, DsConsole::Critical, qIStup->mainTitle(),
                    tr("Fail to save file \"%1\"!").arg(filename));
        return false;
    }
    qsFileMgr->commitRecent(QsFileManager::Project, QsFileManager::Remove, filename);
    return true;
}

bool DsConsole::importFile(QDspxModel *dspx, QWidget *parent) {
    auto pf = GetPlugins();
    if (pf.plugins.isEmpty()) {
        qCs->MsgBox(parent, QsConsole::Critical, qIStup->mainTitle(),
                    tr("No plugins found for importing files!"));
        return false;
    }

    QString path = qsFileMgr->openFile(tr("Import file"), pf.filters, IMPORT_FLAG, parent);
    if (path.isEmpty()) {
        return false;
    }

    auto ext = QMFs::PathFindSuffix(path).toLower();
    auto it = pf.plugins.find(ext);
    if (it == pf.plugins.end()) {
        return false;
    }

    return it.value()->load(path, dspx, parent);
}

DsConsole::DsConsole(DsConsolePrivate &d, QObject *parent) : QsConsole(d, parent) {
    d.init();
}
