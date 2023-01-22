#ifndef PIANOWINDOW_H
#define PIANOWINDOW_H

#include "../Basic/ProjectWindow.h"

class PianoWindowPrivate;

class PianoWindow : public ProjectWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(PianoWindow)
public:
    PianoWindow(QWidget *parent = nullptr);
    ~PianoWindow();

    void reloadStrings(int locale, const QString &key);

public:
    QString filename() const;
    void setFilename(const QString &filename);

    bool load();
    bool save();

protected:
    PianoWindow(PianoWindowPrivate &d, QWidget *parent = nullptr);
};

#endif // PIANOWINDOW_H
