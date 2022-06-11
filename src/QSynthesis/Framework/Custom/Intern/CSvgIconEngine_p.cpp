#include "CSvgIconEngine_p.h"

QAtomicInt CSvgIconEnginePrivate::lastSerialNum;

CSvgIconEnginePrivate::CSvgIconEnginePrivate() {
    stepSerialNum();
}

CSvgIconEnginePrivate::~CSvgIconEnginePrivate() {
}

QString CSvgIconEnginePrivate::pmcKey(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    return QLatin1String("$qt_svgicon_") +
            QString::number(serialNum, 16).append(QLatin1Char('_')) +
            QString::number(
                (((((qint64(size.width()) << 11) | size.height()) << 11) | mode) << 4) | state,
                16);
}

void CSvgIconEnginePrivate::stepSerialNum() {
    serialNum = lastSerialNum.fetchAndAddRelaxed(1);
}

bool CSvgIconEnginePrivate::tryLoad(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state) {
    QString svgFile = svgFiles.value(hashKey(mode, state));
    if (!svgFile.isEmpty()) {
        QFile file(svgFile);
        QByteArray data;
        if (file.open(QIODevice::ReadOnly)) {
            data = file.readAll();
            if (!currentColor.isEmpty()) {
                data.replace("currentColor", currentColor.toUtf8());
            }
            renderer->load(data);
            return true;
        }
    }
    return false;
}

QIcon::Mode CSvgIconEnginePrivate::loadDataForModeAndState(QSvgRenderer *renderer, QIcon::Mode mode,
                                                           QIcon::State state) {
    if (tryLoad(renderer, mode, state))
        return mode;

    const QIcon::State oppositeState = (state == QIcon::On) ? QIcon::Off : QIcon::On;
    if (mode == QIcon::Disabled || mode == QIcon::Selected) {
        const QIcon::Mode oppositeMode =
            (mode == QIcon::Disabled) ? QIcon::Selected : QIcon::Disabled;
        if (tryLoad(renderer, QIcon::Normal, state))
            return QIcon::Normal;
        if (tryLoad(renderer, QIcon::Active, state))
            return QIcon::Active;
        if (tryLoad(renderer, mode, oppositeState))
            return mode;
        if (tryLoad(renderer, QIcon::Normal, oppositeState))
            return QIcon::Normal;
        if (tryLoad(renderer, QIcon::Active, oppositeState))
            return QIcon::Active;
        if (tryLoad(renderer, oppositeMode, state))
            return oppositeMode;
        if (tryLoad(renderer, oppositeMode, oppositeState))
            return oppositeMode;
    } else {
        const QIcon::Mode oppositeMode = (mode == QIcon::Normal) ? QIcon::Active : QIcon::Normal;
        if (tryLoad(renderer, oppositeMode, state))
            return oppositeMode;
        if (tryLoad(renderer, mode, oppositeState))
            return mode;
        if (tryLoad(renderer, oppositeMode, oppositeState))
            return oppositeMode;
        if (tryLoad(renderer, QIcon::Disabled, state))
            return QIcon::Disabled;
        if (tryLoad(renderer, QIcon::Selected, state))
            return QIcon::Selected;
        if (tryLoad(renderer, QIcon::Disabled, oppositeState))
            return QIcon::Disabled;
        if (tryLoad(renderer, QIcon::Selected, oppositeState))
            return QIcon::Selected;
    }
    return QIcon::Normal;
}
