#ifndef PROJECTWINDOWPRIVATE_H
#define PROJECTWINDOWPRIVATE_H

#include "Controls/Windows/private/PlainWindow_p.h"

#include "ProjectWindow.h"

class ProjectWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(ProjectWindow)
public:
    ProjectWindowPrivate();
    ~ProjectWindowPrivate();

    void init();

    bool closeFlag;
};

#endif // PROJECTWINDOWPRIVATE_H
