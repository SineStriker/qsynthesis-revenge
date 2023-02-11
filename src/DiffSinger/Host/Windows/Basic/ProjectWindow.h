#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include "Windows/PlainWindow.h"

class ProjectWindowPrivate;

class ProjectWindow : public PlainWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(ProjectWindow)
public:
    explicit ProjectWindow(QWidget *parent = nullptr);
    ~ProjectWindow();

public:
    QString filename() const;
    void setFilename(const QString &filename);

    bool untitled() const;
    void setUntitled(bool untitled);

protected:
    ProjectWindow(ProjectWindowPrivate &d, QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event) override;
};

#endif // PROJECTWINDOW_H
