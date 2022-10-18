#include "Kernel/QsApplication.h"
#include "Managers/WindowManager.h"

#include <QDebug>
#include <QIconEngine>
#include <QIconEnginePlugin>
#include <QMessageBox>
#include <QPluginLoader>

#include "Api/IUstConverter.h"
#include "Api/IUtaPlugin.h"

#include "fsnotifier.h"

int main(int argc, char *argv[]) {
    QsApplication a(argc, argv);

    qDebug() << "fsnotifier:" << FileSystemNotifier::ExecutableFilePath();

    WindowManager::instance()->showHome();

    return a.exec();
}
