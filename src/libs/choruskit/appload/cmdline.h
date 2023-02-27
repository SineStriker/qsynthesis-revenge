#ifndef CMDLINE_H
#define CMDLINE_H

#include <QTextStream>
#include <QCoreApplication>

#include "extensionsystem/pluginmanager.h"

#include "QMConsole.h"
#include "QMSystem.h"

enum { OptionIndent = 4, DescriptionIndent = 34 };

static const char fixedOptionsC[] = " [options]... [files]...\n";

static const char HELP_OPTION1[] = "-h";
static const char HELP_OPTION2[] = "--help";
static const char VERSION_OPTION1[] = "-v";
static const char VERSION_OPTION2[] = "--version";
static const char ALLOW_ROOT_OPTION[] = "--allow-root";
static const char RESET_CONFIG_OPTION[] = "--reset-config";
static const char PLUGIN_PATH_OPTION[] = "--plugin-path";

static inline void indent(QTextStream &str, int indent) {
    const QChar blank = QLatin1Char(' ');
    for (int i = 0; i < indent; i++)
        str << blank;
}

static inline void formatOption(QTextStream &str, const QStringList &opts, const QString &parm,
                                const QString &description) {
    QString opt = opts.join('/');
    int remainingIndent = DescriptionIndent - OptionIndent - opt.size();
    indent(str, OptionIndent);
    str << opt;
    if (!parm.isEmpty()) {
        str << " <" << parm << '>';
        remainingIndent -= 3 + parm.size();
    }
    if (remainingIndent >= 1) {
        indent(str, remainingIndent);
    } else {
        str << '\n';
        indent(str, DescriptionIndent);
    }
    str << description << '\n';
}

static inline void displayError(const QString &t) {
    qmCon->MsgBox(nullptr, QMConsole::Critical, qAppName(), t);
    QMOs::exitApp(0);
}

static inline void displayHelpText(const QString &t) {
    qmCon->MsgBox(nullptr, QMConsole::Information, qAppName(), t);
    QMOs::exitApp(0);
}

static inline void printVersion(/*const PluginSpec *coreplugin*/) {
    QString version;
    QTextStream str(&version);
    str << '\n' << qAppName() /*<< ' ' << coreplugin->version()*/ << " based on Qt " << qVersion() << "\n\n";
    // PluginManager::formatPluginVersions(str);
    // str << '\n' << coreplugin->copyright() << '\n';
    displayHelpText(version);
}

static inline void printHelp() {
    QString help;
    QTextStream str(&help);

    str << "Usage: " << qAppName() << fixedOptionsC;
    formatOption(str, {HELP_OPTION1, HELP_OPTION2}, {}, "Display this help");
    formatOption(str, {VERSION_OPTION1, VERSION_OPTION2}, {}, "Display application version");
    formatOption(str,
                 {
                     ALLOW_ROOT_OPTION,
                 },
                 {}, "Allow running with root privilege");
    formatOption(str,
                 {
                     RESET_CONFIG_OPTION,
                 },
                 {}, "Generate or reset configuration file");
    formatOption(str,
                 {
                     PLUGIN_PATH_OPTION,
                 },
                 {}, "Add a custom search path for plugins");
    // ExtensionSystem::PluginManager::formatOptions(str, OptionIndent, DescriptionIndent);
    // ExtensionSystem::PluginManager::PluginManager::formatPluginOptions(str, OptionIndent, DescriptionIndent);
    displayHelpText(help);
}

#endif // CMDLINE_H
