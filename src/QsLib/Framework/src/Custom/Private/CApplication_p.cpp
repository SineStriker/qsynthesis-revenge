#include "CApplication_p.h"
#include "QMetaTypeImpl.h"

CApplicationPrivate::CApplicationPrivate() {
}

void CApplicationPrivate::init() {
    QMetaTypeImpl::Register();
}
