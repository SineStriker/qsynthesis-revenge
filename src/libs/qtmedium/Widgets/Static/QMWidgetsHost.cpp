#include "QMWidgetsHost.h"
#include "private/QMWidgetsHost_p.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QScreen>

#include "private/QMWidgetsFactory.h"
#include "private/QMetaTypeImpl_p.h"

#include "QMDecoratorV2.h"
#include "QMSystem.h"

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

    QFont font = QApplication::font();
#ifdef Q_OS_WINDOWS
    font.setFamily("Microsoft YaHei");
#endif
    if (!appFont.isEmpty()) {
        QJsonValue value;
        value = appFont.value("Family");
        if (value.isString()) {
            font.setFamily(value.toString());
        }

        value = appFont.value("Size");
        if (value.isDouble()) {
            double ratio = (qApp->primaryScreen()->logicalDotsPerInch() / QMOs::unitDpi());
            font.setPixelSize(value.toDouble() * ratio);
        }

        value = appFont.value("Weight");
        if (value.isDouble()) {
            font.setWeight(value.toDouble());
        }
    }

    qApp->setFont(font);
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