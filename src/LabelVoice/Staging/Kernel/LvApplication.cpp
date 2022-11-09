#include "LvApplication.h"

#include "private/LvApplication_p.h"

LvApplication::LvApplication(int &argc, char **argv)
    : LvApplication(*new LvApplicationPrivate(), argc, argv) {
}

LvApplication::~LvApplication() {
    Q_D(LvApplication);
    d->deinit();
}

LvApplication::LvApplication(LvApplicationPrivate &d, int &argc, char **argv)
    : LvElemApplication(d, argc, argv) {
    d.init();
}
