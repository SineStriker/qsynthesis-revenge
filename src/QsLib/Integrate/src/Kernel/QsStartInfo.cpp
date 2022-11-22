#include "QsStartInfo.h"

QsStartInfo::QsStartInfo() {
    parser.addOption(QCommandLineOption("reset-config", "Reset or generate app config file."));
}

QsStartInfo::~QsStartInfo() {
}
