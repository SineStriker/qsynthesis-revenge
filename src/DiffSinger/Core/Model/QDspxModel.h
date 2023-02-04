#ifndef QDSPXMODEL_H
#define QDSPXMODEL_H

#include "DsCoreGlobal.h"

#include <QString>

class DSCORE_API QDspxModel {
public:
    QDspxModel();
    ~QDspxModel();

    QString version;

    // public static QDspxModel FromMidi(string filename);

    static bool fromMidi(const QString &filename, QDspxModel *out);
};

// int main1() {
//     QDspxModel dspx;
//     if (!QDspxModel::fromMidi("C:\\a.mid", &dspx)) {
//         printf("Failed\n");
//         return -1;
//     }

//     printf("Success\n");

//     //

//     return 0;
// }

#endif // QDSPXMODEL_H