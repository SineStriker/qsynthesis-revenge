#ifndef PIANOWINDOW_H
#define PIANOWINDOW_H

#include "../Basic/ProjectWindow.h"
#include "QDspxModel.h"

class PianoWindowPrivate;

class PianoWindow : public ProjectWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(PianoWindow)
public:
    explicit PianoWindow(QWidget *parent = nullptr);
    ~PianoWindow();

public:
    void load(const QDspxModel &model);

    QDspxModel currentProject() const;
    QDspxModel currentTrack() const;
    QDspxModel currentClip() const;

protected:
    PianoWindow(PianoWindowPrivate &d, QWidget *parent = nullptr);
};

#endif // PIANOWINDOW_H
