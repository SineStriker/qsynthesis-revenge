#include "BatProcPlugin.h"

#include <QApplication>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMSystem.h>

#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

#include <QJsEngine>

namespace BatPorc {

    using namespace Core;

    namespace Internal {

        class BatchProcessPrivate {
        public:
            QJSEngine engine;
        };

        static BatchProcessPrivate *d = nullptr;

        BatchProcess::BatchProcess() {
        }

        BatchProcess::~BatchProcess() {
        }

        class TestObject: public QObject {
            Q_OBJECT
        public slots:
            QString f(const QStringList &list) {
                qDebug() << list[0].toStdString().c_str();
                return list[1];
            }
        };

        bool BatchProcess::initialize(const QStringList &arguments, QString *errorMessage) {
            auto &d = Internal::d;
            d = new BatchProcessPrivate();

            d->engine.globalObject().setProperty("$", d->engine.newQObject(new TestObject));
            qDebug() << d->engine.evaluate("$.f(['114514', '1919810'])").toString().toStdString().c_str();

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing JavaScript runtime..."));
            }
            // QThread::msleep(2000);



            return true;
        }

        void BatchProcess::extensionsInitialized() {
        }

        bool BatchProcess::delayedInitialize() {
            return true;
        }

    }

}

#include "BatProcPlugin.moc"