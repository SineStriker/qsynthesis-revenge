#ifndef CHORUSKIT_DSPXDOCUMENT_H
#define CHORUSKIT_DSPXDOCUMENT_H

#include <CoreApi/DocumentSpec.h>

namespace Core {

    namespace Internal {

        class DspxSpec : public DocumentSpec {
            Q_OBJECT
        public:
            explicit DspxSpec(QObject *parent = nullptr);
            ~DspxSpec();

        public:
            QString filter() const override;

            bool open(const QString &filename) override;
        };

    }

} // Core

#endif // CHORUSKIT_DSPXDOCUMENT_H
