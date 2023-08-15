//
// Created by fluty on 2023/8/7.
//

#ifndef CHORUSKIT_TRACKCONTROLPANEL_H
#define CHORUSKIT_TRACKCONTROLPANEL_H

#include <QWidget>
#include "QMWidgetsGlobal.h"

class TrackControlPanelPrivate;

class QMWIDGETS_EXPORT TrackControlPanel : public QWidget {
    Q_OBJECT

public:
    explicit TrackControlPanel(QWidget *parent = nullptr);
    ~TrackControlPanel();

    void setTrackIndex(int index);
    void setTrackColor(QColor color);
    void setTrackName(QString name);
    void setTrackPan(double pan);
    void setTrackVolume(double volume);

protected:
    void resizeEvent(QResizeEvent *event) override;
    int m_trackIndex;
    QColor m_trackColor;
    QString m_trackName;
    double m_trackPan = 0;
    double m_trackVolume = 0;

private:
    TrackControlPanelPrivate *d;
};



#endif // CHORUSKIT_TRACKCONTROLPANEL_H
