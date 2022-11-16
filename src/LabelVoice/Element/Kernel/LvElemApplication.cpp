#include "LvElemApplication.h"
#include "private/LvElemApplication_p.h"

#include "Styles/QCssAnalyzer.h"

#include <QScreen>
#include <QStandardPaths>

LvElemApplication::LvElemApplication(int &argc, char **argv)
    : LvElemApplication(*new LvElemApplicationPrivate(), argc, argv) {
}

LvElemApplication::~LvElemApplication() {
    Q_D(LvElemApplication);
    d->deinit();
}

void LvElemApplication::reloadStrings(int locale) {
    Q_UNUSED(locale);
}

void LvElemApplication::reloadScreen(int theme) {
    Q_D(LvElemApplication);
    Q_UNUSED(theme);
    setStyleSheet(QCssAnalyzer().apply(d->stylesheets.join("\n\n"),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

LvElemApplication::LvElemApplication(LvElemApplicationPrivate &d, int &argc, char **argv)
    : QsApplication(d, argc, argv) {
    d.init();
}
