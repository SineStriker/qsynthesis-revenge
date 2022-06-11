#ifndef QUTAUSTRINGS_H
#define QUTAUSTRINGS_H

#include "Strings/UtaCharText.h"
#include "Strings/UtaFilenames.h"
#include "Strings/UtaPluginText.h"
#include "Strings/UtaProjectText.h"
#include "Strings/UtaSettingIni.h"

QString fromUSTVoiceDir(const QString &filePath, const QString &appPath);

QString toUSTVoiceDir(const QString &filePath, const QString &appPath);

QString fromUSTToolsDir(const QString &filePath, const QString &appPath);

QString toUSTToolsDir(const QString &filePath, const QString &appPath);

#endif // QUTAUSTRINGS_H
