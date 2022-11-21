#include "QUtaElemApplication_p.h"
#include "QUtaDistConfig_p.h"

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

static QString loadAppleFont() {
    QString fontDir = qApp->applicationDirPath() + "/resources/fonts";
    int fontId = QFontDatabase::addApplicationFont(fontDir + "/PingFang SC.ttf");
    QStringList fonts = QFontDatabase::applicationFontFamilies(fontId);
    if (fonts.isEmpty()) {
        return QString();
    }
    return fonts.front();
}

QUtaElemApplicationPrivate::QUtaElemApplicationPrivate() : QsApplicationPrivate(new QUtaDistConfig()) {
}

QUtaElemApplicationPrivate::~QUtaElemApplicationPrivate() {
}

void QUtaElemApplicationPrivate::init() {
    Q_Q(QUtaElemApplication);

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

    Q_TR_NOTIFY_PRIVATE(QUtaElemApplication);
    Q_SS_NOTIFY_PRIVATE(QUtaElemApplication);
}

void QUtaElemApplicationPrivate::deinit() {
    // Save Modules
}
