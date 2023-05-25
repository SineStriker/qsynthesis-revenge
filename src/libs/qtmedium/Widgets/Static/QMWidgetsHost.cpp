#include "QMWidgetsHost.h"
#include "private/QMWidgetsHost_p.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFontDatabase>

#include "private/QMWidgetsFactory.h"
#include "private/QMetaTypeImpl_p.h"

#include "QMDecoratorV2.h"

QMWidgetsHostPrivate::QMWidgetsHostPrivate() {
    Register_QMetaTypeImpl();
}

QMWidgetsHostPrivate::~QMWidgetsHostPrivate() {
}

void QMWidgetsHostPrivate::init() {
    // Add theme paths
    for (const auto &path : qAsConst(themePaths))
        qIDec->addThemePath(path);

    for (const auto &path : qAsConst(fontPaths)) {
        QDir directory(path);

        QStringList fontFiles = directory.entryList(QStringList() << "*.ttf"
                                                                  << "*.otf",
                                                    QDir::Files);
        foreach (const QString &fontFile, fontFiles) {
            QString fontFilePath = directory.absoluteFilePath(fontFile);
            int fontId = QFontDatabase::addApplicationFont(fontFilePath);
            if (fontId != -1) {
                QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
                qmDebug() << "qtmedium: add font families: " << fontFamilies.join(", ");
            }
        }
    }

    if (!appFont.isEmpty()) {
        qApp->setFont(QFont(appFont));
    }
#ifdef Q_OS_WINDOWS
    else {
        qApp->setFont(QFont("Microsoft YaHei"));
    }
#endif
}

QMCoreFactory *QMWidgetsHostPrivate::createFactory() {
    return new QMWidgetsFactory();
}

QMWidgetsHost::QMWidgetsHost(QObject *parent) : QMWidgetsHost(*new QMWidgetsHostPrivate(), parent) {
}

QMWidgetsHost::~QMWidgetsHost() {
}

QMWidgetsHost::QMWidgetsHost(QMWidgetsHostPrivate &d, QObject *parent) : QMCoreHost(d, parent) {
    d.init();
}