#ifndef PROJECTIMPORT_H
#define PROJECTIMPORT_H

#include "DsGuiGlobal.h"
#include "QDspxModel.h"

namespace Import {

    DSGUI_API bool loadMidi(const QString &filename, QDspx::Model *out, QObject *parent = nullptr);

    DSGUI_API bool loadSvip(const QString &filename, QDspx::Model *out, QObject *parent = nullptr);

    DSGUI_API bool loadOpenSVIP(const QString &filename, QDspx::Model *out,
                                QObject *parent = nullptr);

    DSGUI_API bool loadUst(const QString &filename, QDspx::Model *out, QObject *parent = nullptr);

} // namespace Import

#endif // PROJECTIMPORT_H
