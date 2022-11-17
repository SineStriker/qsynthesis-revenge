#include "LvElemApplication_p.h"
#include "LvDistConfig_p.h"

#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>
#include <QStandardPaths>
#include <QTextCodec>
#include <QTranslator>

#include "SystemHelper.h"
#include "ViewHelper.h"

static const char Slash = '/';

static QString loadAppleFont() {
    QString fontDir = qApp->applicationDirPath() + "/resources/fonts";
    int fontId = QFontDatabase::addApplicationFont(fontDir + "/PingFang SC.ttf");
    QStringList fonts = QFontDatabase::applicationFontFamilies(fontId);
    if (fonts.isEmpty()) {
        return QString();
    }
    return fonts.front();
}

LvElemApplicationPrivate::LvElemApplicationPrivate() : QsApplicationPrivate(new LvDistConfig()) {
}

LvElemApplicationPrivate::~LvElemApplicationPrivate() {
}

void LvElemApplicationPrivate::init() {
    Q_Q(LvElemApplication);

    // Set app meta data
    q->setApplicationVersion(APP_VERSION);

    // Load codec and fonts
#if defined(Q_OS_WINDOWS)
    auto gbk = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(gbk);

    QString fontName = loadAppleFont();
    QFont f(fontName.isEmpty() ? "Microsoft YaHei" : fontName);
    f.setStyleStrategy(QFont::PreferAntialias);
    f.setPixelSize(12 * (qApp->primaryScreen()->logicalDotsPerInch() / 96.0));
    q->setFont(f);
#endif

    ll = new LocalLinguist(q);
    ld = new LocalDecorator(q);
}

void LvElemApplicationPrivate::deinit() {
    // Save Modules
}
