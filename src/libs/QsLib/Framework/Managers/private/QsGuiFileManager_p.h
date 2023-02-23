#ifndef QSGUIFILEMANAGERPRIVATE_H
#define QSGUIFILEMANAGERPRIVATE_H

#include "../QsGuiFileManager.h"
#include "private/QsFileManager_p.h"

class QsGuiFileManagerPrivate : public QsFileManagerPrivate {
    Q_DECLARE_PUBLIC(QsGuiFileManager)
public:
    QsGuiFileManagerPrivate();
    ~QsGuiFileManagerPrivate();

    void init();
};

#endif // QSGUIFILEMANAGERPRIVATE_H
