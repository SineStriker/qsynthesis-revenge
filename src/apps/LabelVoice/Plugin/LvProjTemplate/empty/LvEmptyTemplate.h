#ifndef LVEMPTYTEMPLATE_H
#define LVEMPTYTEMPLATE_H

#include "Api/ILvProjTemplate.h"

class Q_DECL_EXPORT LvEmptyTemplate : public ILvProjTemplate {
    Q_OBJECT
    Q_INTERFACES(ILvProjTemplate)
    Q_PLUGIN_METADATA(IID ILvProjTemplate_IID FILE "plugin.json")
public:
    explicit LvEmptyTemplate(QObject *parent = nullptr);
    ~LvEmptyTemplate();

    QByteArray descFile() const override;

    bool save(const QString &dir) override;

signals:
};

#endif // LVEMPTYTEMPLATE_H
