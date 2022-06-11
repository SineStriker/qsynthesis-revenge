#include "ColorTheme.h"

#include "Logs/CRecordHolder.h"
#include "Namespace.h"

#include <QApplication>
#include <QDebug>
#include <QMetaProperty>
#include <QScreen>
#include <QStyle>
#include <QWidget>

#include "DataManager.h"
#include "Utils/QCssAnalyzer.h"

#include <QMessageBox>

Q_SINGLETON_DECLARE(ColorTheme)

ColorTheme::ColorTheme(QObject *parent) : BaseManager(parent) {
    construct();
}

ColorTheme::~ColorTheme() {
}

bool ColorTheme::load() {
    // Check theme dir
//    QFileInfo themes(DataManager::themesProfile());
//    if (!themes.isDir()) {
//        Q_ERROR(nullptr, qData->ErrorTitle, tr("Cannot load color themes."));
//        return false;
//    }

    if (qRecordCData.themeIndex < 0 || qRecordCData.themeIndex > themeCount()) {
        qRecordData.themeIndex = 0;
    }
    loadTheme(qRecordCData.themeIndex);
    return true;
}

bool ColorTheme::save() {
    return true;
}

void ColorTheme::loadTheme(int index) {
    QCssAnalyzer qss;

#if defined(Q_OS_MAC)
    if (index == 0) {
        qss.setFileName(":/themes/theme-mac.qss");
    } else if (index == 1) {
        qss.setFileName(":/themes/theme-mac-dark.qss");
    }
#else
    if (index == 0) {
        qss.setFileName(":/themes/dark-v3.qss");
    } else if (index == 1) {
        qss.setFileName(":/themes/dark-v3.qss");
    } else if (index == 2) {
        qss.setFileName(":/themes/dark-v3.qss");
    } else if (index == 3) {
        qApp->setStyleSheet(QString());
        return;
    }
#endif

    double ratio = qApp->primaryScreen()->logicalDotsPerInch() / 96.0;
    qss.setRatio(0.8 * ratio);
    if (qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(qss.readAndApply());
        qss.close();
    }

    reloadAppFont();
}

int ColorTheme::themeCount() const {
    return 4;
}

QStringList ColorTheme::themeNames() const {
    QStringList list = {tr("Multi-Color (Default)"), tr("Dark (Default)"), tr("Light (Default)"),
                        tr("None")};
    return list;
}

void ColorTheme::reloadAppFont() {
    QWidget w;
    qApp->style()->polish(&w);
    qApp->setFont(w.font());
}
