#ifndef LVEMPTYTEMPLATE_H
#define LVEMPTYTEMPLATE_H

#include "Api/ITemplateGenerator.h"

class Q_DECL_EXPORT LvEmptyTemplate : public ITemplateGenerator {
    Q_OBJECT
    Q_INTERFACES(ITemplateGenerator)
    Q_PLUGIN_METADATA(IID ITemplateGenerator_IID FILE "plugin.json")
public:
    explicit LvEmptyTemplate(QObject *parent = nullptr);
    ~LvEmptyTemplate();

    QByteArray descFile() const override;

    bool save(const QString &dir) override;

signals:
};

#endif // LVEMPTYTEMPLATE_H
