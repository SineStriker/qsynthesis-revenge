#ifndef MYCOMMANDLINEPARSER_H
#define MYCOMMANDLINEPARSER_H

#include "qsutils_global.h"

#include <QCommandLineOption>
#include <QCoreApplication>
#include <QStringList>

/*
 * Revised from QCommandLineParser source code
 */

class MyCommandLineParserPrivate;

class QSUTILS_API MyCommandLineParser {
    Q_DECLARE_TR_FUNCTIONS(MyCommandLineParser)
public:
    MyCommandLineParser();
    ~MyCommandLineParser();

    enum SingleDashWordOptionMode { ParseAsCompactedShortOptions, ParseAsLongOptions };
    void setSingleDashWordOptionMode(SingleDashWordOptionMode parsingMode);

    enum OptionsAfterPositionalArgumentsMode { ParseAsOptions, ParseAsPositionalArguments };
    void setOptionsAfterPositionalArgumentsMode(OptionsAfterPositionalArgumentsMode mode);

    bool addOption(const QCommandLineOption &commandLineOption);
    bool addOptions(const QList<QCommandLineOption> &options);

    QCommandLineOption addVersionOption();
    QCommandLineOption addHelpOption();
    void setApplicationDescription(const QString &description);
    QString applicationDescription() const;
    void addPositionalArgument(const QString &name, const QString &description,
                               const QString &syntax = QString());
    void clearPositionalArguments();

    void process(const QStringList &arguments);
    void process(const QCoreApplication &app);

    bool parse(const QStringList &arguments);
    QString errorText() const;

    bool isSet(const QString &name) const;
    QString value(const QString &name) const;
    QStringList values(const QString &name) const;

    bool isSet(const QCommandLineOption &option) const;
    QString value(const QCommandLineOption &option) const;
    QStringList values(const QCommandLineOption &option) const;

    QStringList positionalArguments() const;
    QStringList optionNames() const;
    QStringList unknownOptionNames() const;

    Q_NORETURN void showVersion();
    Q_NORETURN void showHelp(int exitCode = 0);
    QString helpText() const;

private:
    Q_DISABLE_COPY(MyCommandLineParser)

    MyCommandLineParserPrivate *const d;
};

#endif // MYCOMMANDLINEPARSER_H
