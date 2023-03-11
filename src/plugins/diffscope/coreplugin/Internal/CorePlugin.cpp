#include "CorePlugin.h"

#include <QApplication>
#include <QMainWindow>
#include <QSplashScreen>

#include "Windows/PlainWindow.h"
#include "plugindialog.h"

static QSplashScreen *splashScreen = nullptr;

static QWidget *mw = nullptr;

namespace Core {

    namespace Internal {

        CorePlugin::CorePlugin() {
        }

        CorePlugin::~CorePlugin() {
        }

        bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            qDebug() << "initialize Core Plugin" << arguments;

            // Get splash screen handle
            auto var = qApp->property("__choruskit_init_splash__");
            auto screen = var.value<QSplashScreen *>();
            if (screen) {
                splashScreen = screen;
            }

            return true;
        }

        void CorePlugin::extensionsInitialized() {
            qDebug() << "extensions initialized Core Plugin";

            auto w = new PlainWindow();
            w->show();
            mw = w;
        }

        bool CorePlugin::delayedInitialize() {
            qDebug() << "delayed initialize Core Plugin";

            if (splashScreen) {
                if (mw) {
                    splashScreen->finish(mw);
                } else {
                    splashScreen->close();
                }
            }

            PluginDialog dlg(nullptr);
            dlg.exec();

            return true;
        }

        QObject *CorePlugin::remoteCommand(const QStringList &options, const QString &workingDirectory,
                                           const QStringList &args) {
            qDebug() << options;
            qDebug() << workingDirectory;
            qDebug() << args;
            return nullptr;
        }

    }

}