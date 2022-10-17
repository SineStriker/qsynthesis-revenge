#include "Application/Managers/WindowManager.h"
#include "Application/QsApplication.h"

#include <QDebug>
#include <QMessageBox>
#include <QPluginLoader>

#include "api/IFormatImporter.h"

int main(int argc, char *argv[]) {
    QsApplication a(argc, argv);

    WindowManager::instance()->showHome();

    QPluginLoader loader(qApp->applicationDirPath() + "/QMidiImporter.dll");
    QObject *instance = loader.instance(); //
    if (instance != NULL) {
        qDebug() << loader.fileName() + " is loaded";
//        auto avc = qobject_cast<IFormatImporter *>(instance);
        qDebug() << instance;
        qDebug() << loader.metaData();
    } else {
        QMessageBox::information(nullptr, "failed to load plugin", loader.errorString());
    }
    // 需要手动释放
    delete instance;

    return a.exec();
}
