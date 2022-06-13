#include "QsApplication_p.h"

#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QMessageBox>
#include <QScreen>
#include <QTextCodec>

#include "QOtoIni.h"
#include "QPrefixMap.h"
#include "QReadmeText.h"
#include "QUPluginInfo.h"
#include "QVoiceInfo.h"
#include "SequenceTextFile.h"

#include "SystemHelper.h"

QsApplicationPrivate::QsApplicationPrivate() {
}

QsApplicationPrivate::~QsApplicationPrivate() {
}

void QsApplicationPrivate::init() {
    Q_Q(QsApplication);

    initLocale();
    initFonts();

    // Create
    record = new CRecordHolder(q);

    data = new DataManager(q);
    themes = new ExtensionsManager(q);
    windows = new WindowManager(q);

    initModules();

    q->connect(q->primaryScreen(), &QScreen::logicalDotsPerInchChanged, q,
               &QsApplication::q_screenRatioChanged);
}

void QsApplicationPrivate::deinit() {
    quitModules();

    // Destroy
    delete windows;
    delete themes;
    delete data;

    delete record;
}

void QsApplicationPrivate::initLocale() {
    QTextCodec *gbk = QTextCodec::codecForName("GBK");

#ifdef Q_OS_WINDOWS
    QTextCodec::setCodecForLocale(gbk);
#endif

    SequenceTextFile::setCodeForDefault(gbk); // *.ust
    QOtoIni::setCodeForDefault(gbk);          // oto.ini
    QPrefixMap::setCodeForDefault(gbk);       // prefix.map
    QReadmeText::setCodeForDefault(gbk);      // readme.txt
    QVoiceInfo::setCodeForDefault(gbk);       // character.txt
    QUPluginInfo::setCodeForDefault(gbk);     // plugin.txt
}

void QsApplicationPrivate::initFonts() {
    Q_Q(QsApplication);
#if defined(Q_OS_WINDOWS)
    QFont f("Microsoft YaHei");
    f.setStyleStrategy(QFont::PreferAntialias);
    q->setFont(f);
#elif defined(Q_OS_LINUX)
    // ?
#endif
}

void QsApplicationPrivate::initModules() {
    if (!qData->load()) {
        ::exit(1);
    }
    if (!qTheme->load()) {
    }
}

void QsApplicationPrivate::quitModules() {
    if (!qTheme->save()) {
    }
    if (!qData->save()) {
    }
}
