#include "FolderTab_p.h"

#include "SystemHelper.h"

FolderTabPrivate::FolderTabPrivate() {
    unreachable = false;
}

FolderTabPrivate::~FolderTabPrivate() {
}

void FolderTabPrivate::init() {
}

QString FolderTabPrivate::tabText() {
    return QDir::toNativeSeparators(Sys::PathFindFileName(filename));
}
