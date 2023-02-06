#include "DsApplication.h"
#include "private/DsApplication_p.h"

DsApplication::DsApplication(int &argc, char **argv)
    : DsApplication(*new DsApplicationPrivate(), argc, argv) {
}

DsApplication::~DsApplication() {
}

DsApplication::DsApplication(DsApplicationPrivate &d, int &argc, char **argv)
    : QApplication(argc, argv), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
