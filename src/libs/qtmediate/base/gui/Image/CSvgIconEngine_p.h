#ifndef CSVGICONENGINE_P_H
#define CSVGICONENGINE_P_H

#include <private/qguiapplication_p.h>

#include <QSvgRenderer>

class CSvgIconEnginePrivate : public QSharedData {
public:
    CSvgIconEnginePrivate();
    ~CSvgIconEnginePrivate();

    static inline int hashKey(QIcon::Mode mode, QIcon::State state) {
        return (((mode) << 4) | state);
    }

public:
    QString pmcKey(const QSize &size, QIcon::Mode mode, QIcon::State state);

    void stepSerialNum();
    bool tryLoad(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state);
    QIcon::Mode loadDataForModeAndState(QSvgRenderer *renderer, QIcon::Mode mode,
                                        QIcon::State state);

public:
    QHash<int, QString> svgFiles;
    int serialNum;

    static QAtomicInt lastSerialNum;

    // Custom
    QString currentColor;
};

#endif // CSVGICONENGINE_P_H
