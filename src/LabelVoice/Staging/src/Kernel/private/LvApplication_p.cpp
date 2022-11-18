#include "LvApplication_p.h"

#include "Kernel/Events.h"

#include "SystemHelper.h"
#include "ViewHelper.h"

#include <QMessageBox>

LvApplicationPrivate::LvApplicationPrivate() {
}

LvApplicationPrivate::~LvApplicationPrivate() {
}

void LvApplicationPrivate::init() {
    Q_Q(LvApplication);

    // Register user types
    QEventImpl::Register();

    allowRoot = false;
    const QStringList &args = q->arguments();
    for (auto it = args.begin(); it != args.end(); ++it) {
        const QString &arg = *it;
        if (arg.startsWith("-") && arg.size() == 2) {
            // Flags
        } else if (arg.startsWith("--")) {
            // Options
            QStringRef word = arg.midRef(2);
            if (word == "allow-root") {
                allowRoot = true;
            }
        } else if (Sys::isFileExist(arg)) {
            // Files
            filenames.append(arg);
        }
    }

    windowMgr = new WindowManager(q);

    ll = new LocalLinguist(q);
    ld = new LocalDecorator(q);
}

void LvApplicationPrivate::deinit() {
    delete windowMgr;
}

void LvApplicationPrivate::init2() {
    Q_Q(LvApplication);

    // Determine super user
    if (Sys::isUserRoot()) {
        QString msg =
            LvApplication::tr("You're trying to start %1 as the %2 which isn't recommended.")
                .arg(qAppName(), qApp->sysRootUserName());
        if (allowRoot) {
            //            if (QMessageBox::warning(nullptr, q->mainTitle(), msg,
            //            LvApplication::tr("Continue"),
            //                                     LvApplication::tr("Exit"), "", 1) != 0) {
            //                Sys::exitApp(0);
            //            }
        } else {
            QMessageBox::warning(nullptr, q->mainTitle(), msg);
            Sys::exitApp(0);
        }
    }
}

void LvApplicationPrivate::messageReceived_helper(const QStringList &args) {
    Q_UNUSED(args)

    View::bringWindowToForeground(windowMgr->firstWindow());
}
