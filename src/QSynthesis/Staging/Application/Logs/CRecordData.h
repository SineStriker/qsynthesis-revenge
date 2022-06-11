#ifndef CRECORDDATA_H
#define CRECORDDATA_H

#include <QFile>
#include <QMap>
#include <QRect>

#include "QFileSet.h"

class CRecordData {
public:
    CRecordData();
    ~CRecordData();

private:
    void init();

public:
    void reset();

public:
    QString tool1Path;
    QString tool2Path;

    QFileSet projects;
    QFileSet folders;
    QFileSet wavtools;
    QFileSet resamplers;

    QRect windowRect;
    bool windowMaximized;

    bool tracksFormVisibility;
    bool editorFormVisibility;
    bool paramsFormVisibility;

    int lastQuantize;
    bool ignoreRestsWhenInsert;
    bool replaceLyricsWhenInsert;
    bool separateCharWhenInsert;

    bool noteVisibility;
    bool pitchVisibility;
    bool envelopeVisibility;
    bool paramsVisibility;

    bool playHeadCenter;

    bool spriteVisibility;
    double spriteOpacity;
    int spritePosition;

    int themeIndex;
    int translationIndex;

    QMap<QString, QString> lastOpenPaths;
};

#endif // CRECORDDATA_H
