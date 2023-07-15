#ifndef CSVGICONENGINE_P_H
#define CSVGICONENGINE_P_H

#include <private/qguiapplication_p.h>

#include <QSvgRenderer>

class CSvgIconEnginePrivate {
public:
    CSvgIconEnginePrivate();
    ~CSvgIconEnginePrivate();

public:
    QString pmcKey(const QSize &size, QIcon::Mode mode, QIcon::State state);

    void stepSerialNum();
    bool tryLoad(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state);
    QIcon::Mode loadDataForModeAndState(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state);

public:
    int serialNum;
    static QAtomicInt lastSerialNum;

    // Custom
    QByteArray svgContents[8];
    int svgContentIndexes[8];

    QString currentColors[8];
    QString colorHints[8];

    QM::ClickState currentState;

    mutable bool noSaveCache;
};

#endif // CSVGICONENGINE_P_H
