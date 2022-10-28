#ifndef PROJECTCOMMONBLOCK_H
#define PROJECTCOMMONBLOCK_H

#include <QWidget>

class ProjectCommonBlock {
public:
    ProjectCommonBlock(QWidget *w);
    ~ProjectCommonBlock();

    struct TemplateConfig {
        QString dir;
        QString file;
        QString name;
    };

public:
    QString newProject(const TemplateConfig &config) const;

    void openProject() const;
    void openProject(const QString &filename) const;

    void aboutApp() const;
    void aboutQt() const;

protected:
    QWidget *w;
};

#endif // PROJECTCOMMONBLOCK_H
