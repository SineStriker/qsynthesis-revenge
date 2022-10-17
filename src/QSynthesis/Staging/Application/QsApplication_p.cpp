#include "QsApplication_p.h"

#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>
#include <QStandardPaths>
#include <QTextCodec>

#include "SystemHelper.h"

#include "Types/Events.h"

QsApplicationPrivate::QsApplicationPrivate() {
}

QsApplicationPrivate::~QsApplicationPrivate() {
}

void QsApplicationPrivate::init() {
    Q_Q(QsApplication);

    QEventImpl::Register();

    initLocale();
    initFonts();

    // Create
    dataMgr = new DataManager(q);
    extMgr = new ExtentManager(q);
    winMgr = new WindowManager(q);

    initModules();

    q->connect(q->primaryScreen(), &QScreen::logicalDotsPerInchChanged, q,
               &QsApplication::q_screenRatioChanged);
}

void QsApplicationPrivate::deinit() {
    quitModules();

    // Destroy
    delete winMgr;
    delete extMgr;
    delete dataMgr;
}

void QsApplicationPrivate::initLocale() {
    QTextCodec *gbk = QTextCodec::codecForName("GBK");

#ifdef Q_OS_WINDOWS
    QTextCodec::setCodecForLocale(gbk);
#endif
}

void QsApplicationPrivate::initFonts() {
    Q_Q(QsApplication);
#if defined(Q_OS_WINDOWS)
    QFont f("Microsoft YaHei");
    f.setStyleStrategy(QFont::PreferAntialias);
    f.setPixelSize(12 * (qApp->primaryScreen()->logicalDotsPerInch() / 96.0));
    q->setFont(f);
#elif defined(Q_OS_LINUX)
    // ?
#endif
}

void QsApplicationPrivate::initModules() {
    if (!qData->load()) {
        ::exit(1);
    }
    if (!qExt->load()) {
    }
}

void QsApplicationPrivate::quitModules() {
    if (!qExt->save()) {
    }
    if (!qData->save()) {
    }
}
