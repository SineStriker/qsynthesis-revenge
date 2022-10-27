#ifndef PROJECTCOMMONBLOCK_H
#define PROJECTCOMMONBLOCK_H

#include <QWidget>

class ProjectCommonBlock {
public:
    ProjectCommonBlock(QWidget *w);
    ~ProjectCommonBlock();

public:
    void newProject() const;
    void openProject() const;

    void aboutApp() const;
    void aboutQt() const;

protected:
    QWidget *w;
};

#endif // PROJECTCOMMONBLOCK_H
