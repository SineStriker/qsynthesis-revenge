#ifndef ILVPROJTEMPLATE_H
#define ILVPROJTEMPLATE_H

#include <QObject>

#include "lvcore_global.h"

class LVCORE_API ILvProjTemplate : public QObject {
    Q_OBJECT
public:
    ILvProjTemplate(QObject *parent = nullptr);
    ~ILvProjTemplate();

    virtual QByteArray descFile() const = 0;

    virtual bool save(const QString &dir) = 0;

signals:
};

#define ILvProjTemplate_IID "LabelVoice.Plugin.LvProjTemplate"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ILvProjTemplate, ILvProjTemplate_IID)
QT_END_NAMESPACE

#endif // ILVPROJTEMPLATE_H
