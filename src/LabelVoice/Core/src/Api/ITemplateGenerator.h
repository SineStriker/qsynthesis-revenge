#ifndef ITEMPLATEGENERATOR_H
#define ITEMPLATEGENERATOR_H

#include <QObject>

#include "lvcore_global.h"

class LVCORE_API ITemplateGenerator : public QObject {
    Q_OBJECT
public:
    ITemplateGenerator(QObject *parent = nullptr);
    ~ITemplateGenerator();

    virtual QByteArray descFile() const = 0;

    virtual bool save(const QString &dir) = 0;

signals:
};

#define ITemplateGenerator_IID "LabelVoice.Plugin.TemplateGenerator"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ITemplateGenerator, ITemplateGenerator_IID)
QT_END_NAMESPACE

#endif // ITEMPLATEGENERATOR_H
