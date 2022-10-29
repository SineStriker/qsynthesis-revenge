#ifndef PIANOWINDOW_H
#define PIANOWINDOW_H

#include "Windows/Basic/ProjectCommonBlock.h"
#include "Windows/Basic/ProjectWindow.h"

class PianoWindowPrivate;

class PianoWindow : public ProjectWindow, public ProjectCommonBlock {
    Q_OBJECT
    Q_DECLARE_PRIVATE(PianoWindow)
public:
    PianoWindow(QWidget *parent = nullptr);
    ~PianoWindow();

public:
    QString filename() const;
    void setFilename(const QString &filename);

    bool load();
    bool save();

    void reloadStrings();

protected:
    PianoWindow(PianoWindowPrivate &d, QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event) override;

private:
    void _q_actionTriggered(int actionId);
};

#endif // PIANOWINDOW_H
