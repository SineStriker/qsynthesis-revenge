#include "DsApplication_p.h"

#include "Events_p.h"

#include "SystemHelper.h"
#include "ViewHelper.h"

#include "../DsDistConfig.h"

#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>

static QString loadAppleFont() {
    QString fontDir = qApp->applicationDirPath() + "/resources/fonts";
    int fontId = QFontDatabase::addApplicationFont(fontDir + "/PingFang SC.ttf");
    QStringList fonts = QFontDatabase::applicationFontFamilies(fontId);
    if (fonts.isEmpty()) {
        return QString();
    }
    return fonts.front();
}

DsApplicationPrivate::DsApplicationPrivate() : QsApplicationPrivate(new DsDistConfig()) {
}

DsApplicationPrivate::~DsApplicationPrivate() {
}

void DsApplicationPrivate::init() {
    Q_Q(DsApplication);

    // Load codec and fonts
#if defined(Q_OS_WINDOWS)
    auto gbk = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(gbk);

    QString fontName = loadAppleFont();
    QFont f(fontName.isEmpty() ? "Microsoft YaHei" : fontName);
    f.setStyleStrategy(QFont::PreferAntialias);
    f.setPixelSize(12 * (qApp->primaryScreen()->logicalDotsPerInch() / Sys::osUnitDpi()));
    q->setFont(f);
#endif

    // Register user types
    Register_Events();

    windowMgr = new WindowManager(q);

    ll = new LocalLinguist(q);
    ld = new LocalDecorator(q);

    Q_TR_NOTIFY_PRIVATE(DsApplication);
    Q_SS_NOTIFY_PRIVATE(DsApplication);
}

void DsApplicationPrivate::deinit() {
    delete windowMgr;
}
