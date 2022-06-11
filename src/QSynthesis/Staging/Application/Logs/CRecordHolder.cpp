#include "CRecordHolder.h"

#include <QJsonDocument>

#include "MathHelper.h"
#include "QJsonFormatter.h"
#include "SystemHelper.h"

// Temp
const char SECTION_NAME_TEMP[] = "Temporary";

const char KEY_NAME_TOOL1_PATH[] = "Tool1Path";
const char KEY_NAME_TOOL2_PATH[] = "Tool2Path";

const char KEY_NAME_INSERT_LYRICS_STATE[] = "InsertLyricsState";

// Temp/Insert
const char KEY_NAME_IGNORE_RESTS[] = "IgonreRests";
const char KEY_NAME_REPLACE_LYRICS[] = "ReplaceLyrics";
const char KEY_NAME_SEPARATE_CHARS[] = "SeparateChars";

// GUI
const char SECTION_NAME_GUI[] = "GUI";

const char KEY_NAME_MAINWINDOW[] = "MainWindow";
const char KEY_NAME_LAST_QUANTIZE[] = "LastQuantize";
const char KEY_NAME_FORMS_VISIBILITY[] = "FormsVisibility";
const char KEY_NAME_ELEMENTS_STATE[] = "ElementsState";
const char KEY_NAME_FOREGROUND_STATE[] = "ForegroundState";
const char KEY_NAME_PLAYHEAD_STATE[] = "PlayheadState";
const char KEY_NAME_THEME_INDEX[] = "ThemeIndex";
const char KEY_NAME_TRANSLATION_INDEX[] = "TranslationIndex";

// GUI/MainWindow
const char KEY_NAME_MAINWINDOW_X[] = "X";
const char KEY_NAME_MAINWINDOW_Y[] = "Y";
const char KEY_NAME_MAINWINDOW_WIDTH[] = "Width";
const char KEY_NAME_MAINWINDOW_HEIGHT[] = "Height";
const char KEY_NAME_MAINWINDOW_MAXIMIZED[] = "Maximized";

// GUI/Forms
const char KEY_NAME_TRACKS_FORM[] = "TracksForm";
const char KEY_NAME_EDITOR_FORM[] = "EditorForm";
const char KEY_NAME_PARAMS_FORM[] = "ParamsForm";

// GUI/Elements
const char KEY_NAME_NOTE[] = "Note";
const char KEY_NAME_PITCH[] = "Pitch";
const char KEY_NAME_ENVELOPE[] = "Envelope";
const char KEY_NAME_PARAMS[] = "Params";

// GUI/Foreground
const char KEY_NAME_SPRITE_OPACITY[] = "SpriteAlpha";
const char KEY_NAME_SPRITE_VISIBILITY[] = "SpriteVisible";
const char KEY_NAME_SPRITE_POSITION[] = "SpritePosition";

// Paths
const char SECTION_NAME_PATHS[] = "Paths";

const char SECTION_NAME_PROJECT[] = "Histories";
const char SECTION_NAME_VOICE[] = "Voices";
const char SECTION_NAME_WAVTOOL[] = "Wavtools";
const char SECTION_NAME_RESAMPLER[] = "Resamplers";

const char SECTION_NAME_LAST_OPEN[] = "LastOpen";

Q_SINGLETON_DECLARE(CRecordHolder)

CRecordHolder::CRecordHolder(QObject *parent) : FileHolder(parent) {
    construct();
    qRegisterMetaType<RecentType>("CRecordHolder::RecentType");
    qRegisterMetaType<ChangeType>("CRecordHolder::ChangeType");
}

CRecordHolder::CRecordHolder(const QString &filename, QObject *parent) : FileHolder(parent) {
    construct();
    setFilename(filename);
}

QString CRecordHolder::filename() const {
    return m_filename;
}

void CRecordHolder::setFilename(const QString &filename) {
    m_filename = filename;
}

bool CRecordHolder::load() {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray bytes = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug() << "Record error.";
        return false;
    }

    CRecordData temp;
    QJsonObject objDoc = doc.object();

    double x = 0, y = 0, w = 0, h = 0, p = temp.spritePosition, o = temp.spriteOpacity,
           l = temp.lastQuantize, t = temp.themeIndex, t2 = temp.translationIndex;
    QStringList ps, vs, ws, rs;
    QMap<QString, QString> ls;

    bool res = false;
    {
        QJsonFormatter f;
        res = f.parse(
            objDoc,
            f.OT({{SECTION_NAME_GUI,
                   f.OT({
                       {KEY_NAME_MAINWINDOW,
                        f.OT({{KEY_NAME_MAINWINDOW_X, f.D(&x)},
                              {KEY_NAME_MAINWINDOW_Y, f.D(&y)},
                              {KEY_NAME_MAINWINDOW_WIDTH, f.D(&w)},
                              {KEY_NAME_MAINWINDOW_HEIGHT, f.D(&h)},
                              {KEY_NAME_MAINWINDOW_MAXIMIZED, f.B(&temp.windowMaximized)}})},

                       {KEY_NAME_ELEMENTS_STATE,
                        f.OT({{KEY_NAME_NOTE, f.B(&temp.noteVisibility)},
                              {KEY_NAME_PITCH, f.B(&temp.pitchVisibility)},
                              {KEY_NAME_ENVELOPE, f.B(&temp.envelopeVisibility)},
                              {KEY_NAME_PARAMS, f.B(&temp.paramsVisibility)}})},

                       {KEY_NAME_FORMS_VISIBILITY,
                        f.OT({{KEY_NAME_TRACKS_FORM, f.B(&temp.tracksFormVisibility)},
                              {KEY_NAME_EDITOR_FORM, f.B(&temp.editorFormVisibility)},
                              {KEY_NAME_PARAMS_FORM, f.B(&temp.paramsFormVisibility)}})},

                       {KEY_NAME_FOREGROUND_STATE,
                        f.OT({{KEY_NAME_SPRITE_OPACITY, f.D(&o)},
                              {KEY_NAME_SPRITE_POSITION, f.D(&p)},
                              {KEY_NAME_SPRITE_VISIBILITY, f.B(&temp.spriteVisibility)}})},

                       {KEY_NAME_LAST_QUANTIZE, f.D(&l)},
                       {KEY_NAME_PLAYHEAD_STATE, f.B(&temp.playHeadCenter)},
                       {KEY_NAME_THEME_INDEX, f.D(&t)},
                       {KEY_NAME_TRANSLATION_INDEX, f.D(&t2)},
                   })},

                  {SECTION_NAME_TEMP,
                   f.OT({{KEY_NAME_INSERT_LYRICS_STATE,
                          f.OT({{KEY_NAME_IGNORE_RESTS, f.B(&temp.ignoreRestsWhenInsert)},
                                {KEY_NAME_REPLACE_LYRICS, f.B(&temp.replaceLyricsWhenInsert)},
                                {KEY_NAME_SEPARATE_CHARS, f.B(&temp.separateCharWhenInsert)}})},
                         {KEY_NAME_TOOL1_PATH, f.S(&temp.tool1Path)},
                         {KEY_NAME_TOOL2_PATH, f.S(&temp.tool2Path)}})},

                  {SECTION_NAME_PATHS, f.OT({{SECTION_NAME_PROJECT, f.SL(&ps)},
                                             {SECTION_NAME_VOICE, f.SL(&vs)},
                                             {SECTION_NAME_WAVTOOL, f.SL(&ws)},
                                             {SECTION_NAME_RESAMPLER, f.SL(&rs)},
                                             {SECTION_NAME_LAST_OPEN, f.SM(&ls)}})}}));
        f.releaseMemory();
    }

    if (!res) {
        return false;
    }

    QSet<int> values{0, 1, 2, 4, 6, 8, 16, 24, 32};
    if (values.contains(l)) {
        temp.lastQuantize = l;
    }
    if (t >= 0) {
        temp.themeIndex = t;
    }
    if (t2 >= 0) {
        temp.translationIndex = t2;
    }
    if (o >= 0 && o <= 1) {
        temp.spriteOpacity = o;
    }
    if (p >= 0 && p <= 3) {
        temp.spritePosition = p;
    }
    if (w > 0 && h > 0 && !temp.windowMaximized) {
        temp.windowRect = QRect(x, y, w, h);
    }

    temp.projects.set(
        Math::batchReplace(Math::batchFilter(ps, Sys::isFileExist), QDir::fromNativeSeparators));
    temp.folders.set(
        Math::batchReplace(Math::batchFilter(vs, Sys::isDirExist), QDir::fromNativeSeparators));
    temp.wavtools.set(
        Math::batchReplace(Math::batchFilter(ws, Sys::isFileExist), QDir::fromNativeSeparators));
    temp.resamplers.set(
        Math::batchReplace(Math::batchFilter(rs, Sys::isFileExist), QDir::fromNativeSeparators));
    temp.lastOpenPaths = Math::batchReplaceByVal(Math::batchFilterByVal(ls, Sys::isDirExist),
                                                 QDir::fromNativeSeparators);

    m_data = temp;
    return true;
}

#define OBJ QJsonObject

bool CRecordHolder::save() {
    QFile file(m_filename);
    if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
        return false;
    }

    const CRecordData &temp = m_data;
    QRect rect = temp.windowRect;

    QStringList ps, vs, ws, rs;
    QMap<QString, QString> ls;

    ps = Math::batchReplace(temp.projects.valid(), QDir::toNativeSeparators);
    vs = Math::batchReplace(temp.folders.valid(), QDir::toNativeSeparators);
    ws = Math::batchReplace(temp.wavtools.valid(), QDir::toNativeSeparators);
    rs = Math::batchReplace(temp.resamplers.valid(), QDir::toNativeSeparators);
    ls = Math::batchReplaceByVal(Math::batchFilterByVal(temp.lastOpenPaths, Sys::isDirExist),
                                 QDir::toNativeSeparators);

    QJsonObject objDoc(
        {{SECTION_NAME_GUI,
          OBJ({{KEY_NAME_MAINWINDOW, OBJ({{KEY_NAME_MAINWINDOW_X, rect.x()},
                                          {KEY_NAME_MAINWINDOW_Y, rect.y()},
                                          {KEY_NAME_MAINWINDOW_WIDTH, rect.width()},
                                          {KEY_NAME_MAINWINDOW_HEIGHT, rect.height()},
                                          {KEY_NAME_MAINWINDOW_MAXIMIZED, temp.windowMaximized}})},

               {KEY_NAME_ELEMENTS_STATE, OBJ({{KEY_NAME_NOTE, temp.noteVisibility},
                                              {KEY_NAME_PITCH, temp.pitchVisibility},
                                              {KEY_NAME_ENVELOPE, temp.envelopeVisibility},
                                              {KEY_NAME_PARAMS, temp.paramsVisibility}})},

               {KEY_NAME_FORMS_VISIBILITY,
                OBJ({{KEY_NAME_TRACKS_FORM, temp.tracksFormVisibility},
                     {KEY_NAME_EDITOR_FORM, temp.editorFormVisibility},
                     {KEY_NAME_PARAMS_FORM, temp.paramsFormVisibility}})},

               {KEY_NAME_FOREGROUND_STATE,
                OBJ({{KEY_NAME_SPRITE_OPACITY, temp.spriteOpacity},
                     {KEY_NAME_SPRITE_POSITION, temp.spritePosition},
                     {KEY_NAME_SPRITE_VISIBILITY, temp.spriteVisibility}})},

               {KEY_NAME_LAST_QUANTIZE, temp.lastQuantize},
               {KEY_NAME_PLAYHEAD_STATE, temp.playHeadCenter},
               {KEY_NAME_THEME_INDEX, temp.themeIndex},
               {KEY_NAME_TRANSLATION_INDEX, temp.translationIndex}})},

         {SECTION_NAME_TEMP, OBJ({{KEY_NAME_INSERT_LYRICS_STATE,
                                   OBJ({{KEY_NAME_IGNORE_RESTS, temp.ignoreRestsWhenInsert},
                                        {KEY_NAME_REPLACE_LYRICS, temp.replaceLyricsWhenInsert},
                                        {KEY_NAME_SEPARATE_CHARS, temp.separateCharWhenInsert}})},
                                  {KEY_NAME_TOOL1_PATH, temp.tool1Path},
                                  {KEY_NAME_TOOL2_PATH, temp.tool2Path}})},

         {SECTION_NAME_PATHS, OBJ({{SECTION_NAME_PROJECT, QJsonArray::fromStringList(ps)},
                                   {SECTION_NAME_VOICE, QJsonArray::fromStringList(vs)},
                                   {SECTION_NAME_WAVTOOL, QJsonArray::fromStringList(ws)},
                                   {SECTION_NAME_RESAMPLER, QJsonArray::fromStringList(rs)},
                                   {SECTION_NAME_LAST_OPEN, QJsonFormatter::mapToObject(ls)}})}});

    QJsonDocument doc;
    doc.setObject(objDoc);

    file.write(doc.toJson());
    file.close();

    return true;
}

void CRecordHolder::reset() {
    m_data.reset();
}

const CRecordData &CRecordHolder::constData() const {
    return m_data;
}

CRecordData &CRecordHolder::data() {
    return m_data;
}

void CRecordHolder::commitRecent(CRecordHolder::RecentType rType, CRecordHolder::ChangeType cType,
                                 const QString &filename) {
    QFileSet *fs;
    switch (rType) {
    case Project:
        fs = &m_data.projects;
        break;
    case Folder:
        fs = &m_data.folders;
        break;
    case Wavtool:
        fs = &m_data.wavtools;
        break;
    default:
        fs = &m_data.resamplers;
        break;
    }
    switch (cType) {
    case Push:
        fs->push(filename);
        break;
    case Unshift:
        fs->unshift(filename);
        break;
    case Advance:
        fs->advance(filename);
        break;
    case Remove:
        fs->remove(filename);
        break;
    default:
        fs->clear();
        break;
    }
    emit recentCommited(rType);
}

QStringList CRecordHolder::fetchRecent(CRecordHolder::RecentType rType) const {
    QStringList res;
    switch (rType) {
    case Project:
        res = m_data.projects.valid();
        break;
    case Folder:
        res = m_data.folders.valid();
        break;
    case Wavtool:
        res = m_data.wavtools.valid();
        break;
    default:
        res = m_data.resamplers.valid();
        break;
    }
    return res;
}
