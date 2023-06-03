#ifndef CHORUSKIT_DSPXSPEC_H
#define CHORUSKIT_DSPXSPEC_H

#include <CoreApi/DocumentSpec.h>

namespace Core {

    namespace Internal {

        class DspxSpec : public DocumentSpec {
            Q_OBJECT
        public:
            explicit DspxSpec(QObject *parent = nullptr);
            ~DspxSpec();

            void reloadStrings();

        public:
            QStringList supportedExtensions() const override;
            QString filter() const override;

            bool open(const QString &fileName) override;

            bool canRecover() const override;
            bool recover(const QString &logDir, const QString &fileName);
        };

    }

} // Core

#endif // CHORUSKIT_DSPXSPEC_H
