#include "../ProjectImport.h"

#include "QUstFile.h"

bool Import::loadUst(const QString &filename, QDspx::Model *out, QObject *parent) {
    QUstFile ust;
    if (!ust.load(filename)) {
        qDebug() << "Failed to parse UTAU file.";
        return false;
    }

    qDebug() << ust.sectionNotes.size();

    return true;
}
