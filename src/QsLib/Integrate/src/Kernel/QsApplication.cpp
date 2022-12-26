#include "QsApplication.h"
#include "private/QsApplication_p.h"

#include "Styles/QCssAnalyzer.h"

#include <QScreen>
#include <QStandardPaths>
#include <QWidget>

QsApplication::QsApplication(int &argc, char **argv)
    : QsApplication(*new QsApplicationPrivate(), argc, argv) {
}

QsApplication::~QsApplication() {
    Q_D(QsApplication);
    d->deinit();
}

void QsApplication::reloadStrings(int locale) {
    Q_D(QsApplication);

    switch (locale) {
        case MultimodeHandle::UnitedStates:
            d->ll->eliminate();
            break;
        case MultimodeHandle::China:
            d->ll->translateOnly(":/translations/QsIntegrate_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->translateOnly(":/translations/QsIntegrate_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->translateOnly(":/translations/QsIntegrate_ja_JP.qm");
            break;
        default:
            break;
    }
}

void QsApplication::reloadScreen(int theme) {
    Q_UNUSED(theme);
}

void QsApplication::applyTheme(QWidget *w, const QStringList &stylesheets) {
    w->setStyleSheet(QCssAnalyzer().apply(stylesheets.join("\n\n"),
                                          primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

QString QsApplication::untitledFileName() {
    return tr("Untitled");
}

QString QsApplication::unsavedPrefix() {
    return "*";
}

QString QsApplication::deletedPrefix() {
    return tr("(Deleted)");
}

QsApplication::QsApplication(QsApplicationPrivate &d, int &argc, char **argv)
    : CApplication(argc, argv), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
