#ifndef CHORUSKIT_DSPXSPEC_H
#define CHORUSKIT_DSPXSPEC_H

#include <ItemModel/AceTreeEntity.h>
#include <ItemModel/AceTreeSerializer.h>

#include <CoreApi/DocumentSpec.h>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class DspxSpecPrivate;

    class CORE_EXPORT DspxSpec : public DocumentSpec {
        Q_OBJECT
    public:
        explicit DspxSpec(QObject *parent = nullptr);
        ~DspxSpec();

        static DspxSpec *instance();

    public:
        QStringList supportedExtensions() const override;
        QString filter() const override;

        bool open(const QString &fileName) override;

        bool canRecover() const override;
        bool recover(const QString &logDir, const QString &fileName) override;

    public:
        AceTreeSerializer *serializer() const;

        AceTreeEntityBuilder *entityBuilder() const;

    private:
        DspxSpecPrivate *d;
    };

} // Core

#endif // CHORUSKIT_DSPXSPEC_H
