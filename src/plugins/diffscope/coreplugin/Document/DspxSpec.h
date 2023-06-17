#ifndef CHORUSKIT_DSPXSPEC_H
#define CHORUSKIT_DSPXSPEC_H

#include <QVersionNumber>

#include <CoreApi/DocumentSpec.h>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class DspxSpecPrivate;

    class CORE_EXPORT DspxSpec : public DocumentSpec {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxSpec)
    public:
        explicit DspxSpec(QObject *parent = nullptr);
        ~DspxSpec();

        static DspxSpec *instance();

        static QVersionNumber currentVersion();

    public:
        QStringList supportedExtensions() const override;
        QString filter() const override;

        bool open(const QString &fileName) override;

        bool canRecover() const override;
        bool recover(const QString &logDir, const QString &fileName) override;
    };

} // Core

#endif // CHORUSKIT_DSPXSPEC_H
