#include "JBNavigableFileContainers.h"
#include "JBFileWatcherUtils.h"

#include <QDebug>

bool JBFilePath_Comparator::operator()(const QString &path1, const QString &path2) const {
    return JBFileWatcherUtils::OSAgnosticPathUtil::COMPARATOR_compare(path1, path2) < 0;
}
