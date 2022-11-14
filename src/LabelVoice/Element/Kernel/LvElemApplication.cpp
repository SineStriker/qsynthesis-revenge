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

void LvElemApplication::reloadStrings() {
    QsApplication::reloadStrings();
}

void LvElemApplication::reloadScreen() {
    Q_D(LvElemApplication);
    setStyleSheet(QCssAnalyzer().apply(d->stylesheets.join("\n\n"),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));

    QsApplication::reloadScreen();
}

LvElemApplication::LvElemApplication(LvElemApplicationPrivate &d, int &argc, char **argv)
    : QsApplication(d, argc, argv) {
    d.init();
}
