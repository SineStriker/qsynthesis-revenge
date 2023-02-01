#include "CStartInfo_p.h"
#include "private/QMetaTypeImpl_p.h"

CStartInfoPrivate::CStartInfoPrivate() {
}

CStartInfoPrivate::~CStartInfoPrivate() {
}

void CStartInfoPrivate::init() {
    Register_QMetaTypeImpl();
}