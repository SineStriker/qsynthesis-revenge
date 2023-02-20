#include "DsStartInfo_p.h"
#include "CDecorator.h"
#include "QsSystem.h"

#include "Events_p.h"

#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>
#include <QTextCodec>

static QString loadAppleFont() {
    QString fontDir = qApp->applicationDirPath() + "/resources/fonts";
    int fontId = QFontDatabase::addApplicationFont(fontDir + "/PingFang SC.ttf");
    QStringList fonts = QFontDatabase::applicationFontFamilies(fontId);
    if (fonts.isEmpty()) {
        return QString();
    }
    return fonts.front();
}

DsStartInfoPrivate::DsStartInfoPrivate() {
}

DsStartInfoPrivate::~DsStartInfoPrivate() {
}

void DsStartInfoPrivate::init() {
    // Register user types
    Register_Events();

    // Load codec and fonts
#if defined(Q_OS_WINDOWS)
    auto gbk = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(gbk);

    QString fontName = loadAppleFont();
    QFont f(fontName.isEmpty() ? "Microsoft YaHei" : fontName);
    f.setStyleStrategy(QFont::PreferAntialias);
    f.setPixelSize(12 * (qApp->primaryScreen()->logicalDotsPerInch() / QsOs::unitDpi()));
    qApp->setFont(f);
#endif
}

void DsStartInfoPrivate::load_helper() {
    Q_Q(DsStartInfo);
    qIDec->addThemeTemplate("Global", ":/themes/global.qss.in");
    qIDec->addThemeTemplate("HomeWindow", ":/themes/home.qss.in");
    qIDec->addThemeTemplate("PianoWindow", ":/themes/piano.qss.in");

    dd_gui.setDir(qAppConf->appDir(QsCoreConfig::AppShare));
    dd_gui.loadDefault("DsGui");

    dd.setDir(qAppConf->appDir(QsCoreConfig::AppShare));
    dd.loadDefault("DsHost");

    windowMgr = new WindowManager(q);
}
