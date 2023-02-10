#include "DsConsole.h"
#include "private/DsConsole_p.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include "QsFileManager.h"
#include "QsSystem.h"
#include "QsView.h"

#include "CStartInfo.h"
#include "QsLinq.h"

#include "IDsPorter.h"
#include "QsPluginManager.h"

static const char FILE_EXTENSIONS_DELIMITER[] = ";;";

static const char OPEN_FLAG[] = "%PROJ%";
static const char IMPORT_FLAG[] = "%IMPORT%";

struct PluginsAndFilter {
    QMap<QString, IDsPorter *> plugins;
    QString filters;
};

static auto GetPlugins() {
    // Get all plugins
    auto porters = qsPluginMgr->pluginSet("dsporters")->plugins<IDsPorter>();
    QMap<QString, IDsPorter *> plugins;
    for (const auto &porter : qAsConst(porters)) {
        const auto &fmt = porter->format();
        plugins.insert(fmt.suffix.toLower(), porter);
    }

    return PluginsAndFilter{
        plugins, (QsLinq::Select<IDsPorter *, QString>(
                      porters,
                      [&](const IDsPorter *val) {
                          const auto &fmt = val->format();
                          return QString("%1(*.%2)").arg(fmt.name, fmt.suffix);
                      })
                  << QString("%1(%2)").arg(DsConsole::tr("All Files"), QsOs::allFilesFilter()))
                     .join(FILE_EXTENSIONS_DELIMITER)};
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
    CConsole::MsgBox(parent, Information, title, text);
#else
    QMessageBox::information(parent, title, text);
#endif
}

bool DsConsole::openFile(QDspxModel *dspx, QWidget *parent) {
    QString filter =
        QStringList{
            tr("DiffScope Files(*.dspx)"),                                 //
            QString("%1(%2)").arg(tr("All Files"), QsOs::allFilesFilter()) //
        }
            .join(FILE_EXTENSIONS_DELIMITER);

    QString path = qsFileMgr->openFile(tr("Open file"), filter, OPEN_FLAG, parent);
    if (path.isEmpty()) {
        return false;
    }
    return dspx->load(path);
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

    auto ext = QsFs::PathFindSuffix(path).toLower();
    auto it = pf.plugins.find(ext);
    if (it == pf.plugins.end()) {
        return false;
    }

    return it.value()->load(path, dspx, parent);
}

DsConsole::DsConsole(DsConsolePrivate &d, QObject *parent) : QsConsole(d, parent) {
    d.init();
}
