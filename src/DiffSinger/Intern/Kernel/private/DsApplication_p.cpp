#include "DsApplication_p.h"

#include "Events_p.h"

#include "QsSystem.h"
#include "QsView.h"

#include "CDecorator.h"

#include "../DsDistConfig.h"
#include "../DsStartInfo.h"

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

DsApplicationPrivate::DsApplicationPrivate() : QsApplicationPrivate() {
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
    f.setPixelSize(12 * (qApp->primaryScreen()->logicalDotsPerInch() / QsSys::osUnitDpi()));
    q->setFont(f);
#endif

    // Register user types
    Register_Events();

    qIDec->addThemeTemplate("HomeWindow", ":/themes/home.qss.in");
    qIDec->addThemeTemplate("PianoWindow", ":/themes/piano.qss.in");

    dd.setDir(qAppConf->appDir(QsCoreConfig::AppShare));
    dd.loadDefault("DsIntern");

    windowMgr = new WindowManager(q);
}

void DsApplicationPrivate::deinit() {
    delete windowMgr;
}
