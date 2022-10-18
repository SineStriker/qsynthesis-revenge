#include "Kernel/QsApplication.h"
#include "Managers/WindowManager.h"

#include <QDebug>
#include <QMessageBox>
#include <QPluginLoader>
#include <QIconEnginePlugin>
#include <QIconEngine>

#include "Api/IUstConverter.h"
#include "Api/IUtaPlugin.h"

int main(int argc, char *argv[]) {
    QsApplication a(argc, argv);

    WindowManager::instance()->showHome();

    return a.exec();
}
