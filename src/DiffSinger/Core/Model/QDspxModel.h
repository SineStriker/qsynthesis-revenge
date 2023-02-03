#ifndef QDSPXMODEL_H
#define QDSPXMODEL_H

#include "DsCoreGlobal.h"

class DSCORE_API QDspxModel {
public:
    QDspxModel();
    ~QDspxModel();

    static bool fromMidi(const QString &filename, QDspxModel *out);
};

#endif // QDSPXMODEL_H