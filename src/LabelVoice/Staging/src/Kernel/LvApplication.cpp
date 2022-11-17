#include "LvApplication.h"

#include "private/LvApplication_p.h"

LvApplication::LvApplication(int &argc, char **argv)
    : LvApplication(*new LvApplicationPrivate(), argc, argv) {
}

LvApplication::~LvApplication() {
    Q_D(LvApplication);
    d->deinit();
}

void LvApplication::reloadStrings(int locale) {
    Q_UNUSED(locale);
}

void LvApplication::reloadScreen(int theme) {
    Q_UNUSED(theme);
}

LvApplication::LvApplication(LvApplicationPrivate &d, int &argc, char **argv)
    : LvElemApplication(d, argc, argv) {
    d.init();

    Q_TR_NOTIFY(LvApplication);
    Q_SS_NOTIFY(LvApplication);
}
