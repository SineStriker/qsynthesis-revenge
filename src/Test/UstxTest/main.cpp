#include <QCoreApplication>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QPluginLoader>
#include <QTextStream>

#include "Api/ISVSConverter.h"

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

#define PLUGINS_DIR qApp->applicationDirPath() + "/plugins"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString dllPath = PLUGINS_DIR + "/svsconverters/qustx.dll";

    // Set library loading info
#ifdef Q_OS_WINDOWS
    ::SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
                               LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_SYSTEM32 |
                               LOAD_LIBRARY_SEARCH_USER_DIRS);
    ::AddDllDirectory(QString(qApp->applicationDirPath() + "/lib").toStdWString().c_str());
#endif
    qApp->addLibraryPath(PLUGINS_DIR);

    QPluginLoader loader(dllPath);
    QObject *instance = loader.instance(); //
    if (instance != NULL) {
        auto avc = qobject_cast<ISVSConverter *>(instance);

        qDebug() << avc;

        // Enter path
        QTextStream qin(stdin);
        QTextStream qout(stdout);

        QString filename;
        qout << QStringLiteral("Enter filename:") << Qt::endl;
        qin >> filename;

        QSvipFile out;
        qDebug() << avc->load(filename, &out);

    } else {
        qDebug() << "Failed to load plugin:" << loader.errorString();
    }

    return a.exec();
}
