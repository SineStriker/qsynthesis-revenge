#include "CRecordData.h"
#include "CRecordHolder.h"

#include <QDebug>

CRecordData::CRecordData() {
    init();
    reset();
}

CRecordData::~CRecordData() {
}

void CRecordData::init() {
    projects.setType(QFileSet::File);
    folders.setType(QFileSet::Directory);
    wavtools.setType(QFileSet::File);
    resamplers.setType(QFileSet::File);
}

void CRecordData::reset() {
    tool1Path.clear();
    tool2Path.clear();

    projects.clear();
    folders.clear();
    wavtools.clear();
    resamplers.clear();

    windowRect = QRect();
    windowMaximized = false;

    tracksFormVisibility = true;
    editorFormVisibility = true;
    paramsFormVisibility = true;

    lastQuantize = 1;
    ignoreRestsWhenInsert = true;
    replaceLyricsWhenInsert = true;
    separateCharWhenInsert = false;

    noteVisibility = false;
    pitchVisibility = true;
    envelopeVisibility = true;
    paramsVisibility = true;

    playHeadCenter = false;

    spriteVisibility = false;
    spriteOpacity = 0.3;
    spritePosition = 3;

    themeIndex = -1;
    translationIndex = -1;
}
