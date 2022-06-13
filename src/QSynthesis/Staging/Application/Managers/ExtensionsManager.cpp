#include "ExtensionsManager.h"
#include "ExtensionsManager_p.h"

#include "Logs/CRecordHolder.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>

#include "Utils/QCssAnalyzer.h"

#include <QMessageBox>

Q_SINGLETON_DECLARE(ExtensionsManager)

ExtensionsManager::ExtensionsManager(QObject *parent)
    : ExtensionsManager(*new ExtensionsManagerPrivate(), parent) {
}

ExtensionsManager::~ExtensionsManager() {
}

bool ExtensionsManager::load() {
    if (qRecordCData.themeIndex < 0 || qRecordCData.themeIndex > themeCount()) {
        qRecordData.themeIndex = 0;
    }
    themeLoad(qRecordCData.themeIndex);
    return true;
}

bool ExtensionsManager::save() {
    return true;
}

void ExtensionsManager::themeLoad(int index) {
    Q_D(ExtensionsManager);

    QCssAnalyzer qss;

#if defined(Q_OS_MAC)
    if (index == 0) {
        qss.setFileName(":/themes/theme-mac.qss");
    } else if (index == 1) {
        qss.setFileName(":/themes/theme-mac-dark.qss");
    }
#else
    if (index == 0) {
        qss.setFileName(":/themes/dark-v3.qss");
    } else if (index == 1) {
        qss.setFileName(":/themes/dark-v3.qss");
    } else if (index == 2) {
        qss.setFileName(":/themes/dark-v3.qss");
    } else if (index == 3) {
        qApp->setStyleSheet(QString());
        return;
    }
#endif

    double ratio = qApp->primaryScreen()->logicalDotsPerInch() / 96.0;
    qss.setRatio(0.8 * ratio);
    if (qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(qss.readAndApply());
        qss.close();
    }

    d->reloadAppFont();
}

int ExtensionsManager::themeCount() const {
    return 4;
}

QStringList ExtensionsManager::themeNames() const {
    QStringList list = {tr("Multi-Color (Default)"), //
                        tr("Dark (Default)"),        //
                        tr("Light (Default)"),       //
                        tr("None")};
    return list;
}

ExtensionsManager::ExtensionsManager(ExtensionsManagerPrivate &d, QObject *parent)
    : BaseManager(d, parent) {
    construct();

    d.init();
}
