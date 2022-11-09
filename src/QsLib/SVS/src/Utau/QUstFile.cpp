#include "QUstFile.h"

#include "Config/UtaConstants.h"
#include "Config/UtaProjectText.h"
#include "Utils/QUtaUtils.h"

#include "TextHelper.h"

#include <QFile>
#include <QTextCodec>
#include <QTextStream>

using namespace Utau;

Q_CHARSET_DECLARE(QUstFile)

static const char COMMA = ',';

// Project File Reader
QUstFile::QUstFile() {
    QUstFile::reset();
}

QUstFile::~QUstFile() {
}

bool QUstFile::load(const QString &filename) {
    QFile file(filename);
    QByteArray data;
    bool isUnicode = false;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    data = file.readAll();
    file.close();

    QTextStream in(&data);

    // Detect Code
    QTextCodec *codec = Txt::GetUtfCodec(data, &m_charsetDetermined);
    if (codec) {
        m_codec = codec;
    } else if (!m_codec) {
        m_codec = s_codeForDefault;
    }
    in.setCodec(m_codec);

    // Read File
    QStringList currentSection;

    QString line;
    QString sectionName;
    QString sectionHead;

    QUstNote curNote;
    int curLength;

    bool num = false;
    bool projectValid = true;

    bool findVersion, findSetting, findNote;

    findVersion = findSetting = findNote = false;
    curLength = 0;

    try {
        while (!in.atEnd()) {
            line = in.readLine();

            if (line.isEmpty() && !in.atEnd()) {
                continue;
            }

            // Continue to add until meet the start of section or end
            if (!line.startsWith(SECTION_BEGIN_MARK) && !in.atEnd()) {
                currentSection.push_back(line);
                continue;
            }

            // If meet end, append without continue
            if (!line.isEmpty() && in.atEnd()) {
                currentSection.push_back(line);
            }

            // Previous section is empty
            if (currentSection.size() <= 1) {

            } else {
                sectionHead = currentSection[0];
                // If Section Name is invalid
                if (!parseSectionName(sectionHead, sectionName)) {
                    currentSection.clear();
                    continue;
                }

                // If Section Name is a number
                num = false;
                sectionName.toInt(&num);

                if (num) {
                    // Parse Note
                    findNote = true;

                    curNote.clear();
                    if (parseSectionNote(currentSection, curNote)) {
                        // curNote.tick = curLength;

                        // Ignore note whose length is invalid
                        if (curNote.length > 0) {
                            sectionNotes.push_back(curNote);
                            curLength += curNote.length; // Add to main tick count
                        }

                    } else {
                        projectValid = false;
                    }
                } else if (sectionName == SECTION_NAME_VERSION) {
                    // Parse Version Sequence
                    findVersion = true;
                    if (!parseSectionVersion(currentSection, sectionVersion)) {
                        projectValid = false;
                    }
                    // Not Unicode
                    QString charset = sectionVersion.charset;
                    if (!isUnicode) {
                        if (!charset.isEmpty()) {
                            QTextCodec *newCodec =
                                QTextCodec::codecForName(sectionVersion.charset.toLatin1());
                            if (newCodec) {
                                in.setCodec(newCodec);
                            }
                        } else {
                        }
                    }
                } else if (sectionName == SECTION_NAME_SETTING) {
                    // Parse global settings
                    findSetting = true;
                    if (!parseSectionSettings(currentSection, sectionSettings)) {
                        projectValid = false;
                    }
                }
            }

            currentSection.clear();
            currentSection.push_back(line);
        }
        if (!(findVersion || findSetting || findNote)) {
            projectValid = false;
        }
    } catch (...) {
        return false;
    }

    Q_UNUSED(projectValid);

    return true;
}

bool QUstFile::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    if (!m_codec) {
        QString charset = sectionVersion.charset;
        if (!charset.isEmpty()) {
            m_codec = QTextCodec::codecForName(charset.toLatin1()); // Write UTF-8
        } else {
            m_codec = s_codeForDefault;
        }
    }
    out.setCodec(m_codec);

    writeSectionVersion(out);  // Write Version
    writeSectionSettings(out); // Write Global Settings

    // Write Notes
    for (int i = 0; i < sectionNotes.size(); ++i) {
        writeSectionNote(i, sectionNotes.at(i), out);
    }

    writeSectionName(SECTION_NAME_TRACKEND, out); // Write End Sign
    file.close();

    return true;
}

void QUstFile::reset() {
    m_codec = nullptr;
    m_charsetDetermined = false;

    sectionVersion.clear();
    sectionSettings.clear();
    sectionNotes.clear();
}

bool QUstFile::isEmpty() const {
    return sectionNotes.isEmpty();
}

bool QUstFile::parseSectionName(const QString &str, QString &name) {
    if (str.startsWith(SECTION_BEGIN_MARK) && str.endsWith(SECTION_END_MARK)) {
        name =
            str.mid(int(strlen(SECTION_BEGIN_MARK)),
                    str.size() - int(strlen(SECTION_BEGIN_MARK)) - int(strlen(SECTION_END_MARK)));
        return true;
    } else {
        return false;
    }
}

bool QUstFile::parseSectionNote(const QStringList &sectionList, QUstNote &note) {
    QStringList::size_type i;
    bool isValid = true;

    int eq;
    QString line;
    QString key, value;

    int valueInt;
    double valueDouble;
    bool isInt, isDouble;

    QUtaUtils::PBStrings mode2;
    QString strEnv;

    for (i = 0; i < sectionList.size(); ++i) {
        line = sectionList.at(i);
        eq = line.indexOf('=');
        if (eq <= 0) {
            continue;
        }

        key = line.left(eq);
        value = line.mid(eq + 1);

        valueInt = value.toInt(&isInt);
        valueDouble = value.toDouble(&isDouble);

        if (key == KEY_NAME_LYRIC) {
            note.lyric = value; // Lyric
        } else if (key == KEY_NAME_NOTE_NUM) {
            if (isInt) {
                note.noteNum = valueInt; // Note Num
            }
        } else if (key == KEY_NAME_LENGTH) {
            if (isInt) {
                note.length = valueInt; // Length
            }
        } else if (key == KEY_NAME_FLAGS) {
            note.flags = value; // Flags
        } else if (key == KEY_NAME_INTENSITY) {
            if (isDouble) {
                note.intensity = valueDouble; // Volume
            }
        } else if (key == KEY_NAME_MODULATION || key == KEY_NAME_MODURATION) {
            if (isDouble) {
                note.modulation = valueDouble; // Modulation
            }
        } else if (key == KEY_NAME_PRE_UTTERANCE) {
            if (isDouble) {
                note.preUttr = valueDouble; // PreUtterence
            }
        } else if (key == KEY_NAME_VOICE_OVERLAP) {
            if (isDouble) {
                note.overlap = valueDouble; // Voice Overlap
            }
        } else if (key == KEY_NAME_VELOCITY) {
            if (isDouble) {
                note.velocity = valueDouble; // Consonant Velocity
            }
        } else if (key == KEY_NAME_START_POINT) {
            if (isDouble) {
                note.stp = valueDouble; // StartPoint
            }
        } else if (key == KEY_NAME_TEMPO) {
            if (isDouble) {
                note.tempo = valueDouble; // Tempo
            }
        } else if (key == KEY_NAME_REGION_START) {
            note.region = value; // Start of region
        } else if (key == KEY_NAME_REGION_END) {
            note.regionEnd = value; // End of region
        } else if (key == KEY_NAME_PB_START) {
            if (isDouble) {
                note.pbstart = valueDouble; // Mode1 Start
            }
        } else if (key == KEY_NAME_PBS) {
            mode2.PBS = value; // Mode2 Start
        } else if (key == KEY_NAME_PBW) {
            mode2.PBW = value; // Mode2 Intervals
        } else if (key == KEY_NAME_PBY) {
            mode2.PBY = value; // Mode2 Offsets
        } else if (key == KEY_NAME_PBM) {
            mode2.PBM = value; // Mode2 Types
        } else if (key == KEY_NAME_PB_START) {
            if (isDouble) {
                note.pbstart = valueDouble; // Mode1 Start
            }
        } else if (key == KEY_NAME_PICHES || key == KEY_NAME_PITCHES ||
                   key == KEY_NAME_PITCH_BEND) {
            note.pitches = QUtaUtils::StringsToDoubles(value.split(COMMA)); // Mode1 Pitch
        } else if (key == KEY_NAME_VBR) {
            note.vibrato = QUtaUtils::StringsToDoubles(value.split(COMMA)); // Vibrato
        } else if (key == KEY_NAME_ENVELOPE) {
            strEnv = value; // Envelope
        } else if (!key.startsWith('@')) {
            note.customData.append(qMakePair(key, value)); // Custom Values
        }
    }
    note.Mode2Pitch = QUtaUtils::StringToPortamento(mode2); // Mode2 Pitch
    note.envelope = QUtaUtils::StringToEnvelope(strEnv);

    return isValid;
}

bool QUstFile::parseSectionVersion(const QStringList &sectionList, QUstVersion &version) {
    int i;
    bool flag = false;

    int eq;
    QString line;
    QString key, value;

    for (i = 0; i < sectionList.size(); ++i) {
        line = sectionList.at(i);
        eq = line.indexOf('=');
        if (eq >= 0) {
            key = line.left(eq);
            value = line.mid(eq + 1);
            if (key == KEY_NAME_CHARSET) {
                sectionVersion.charset = value;
            }
            continue;
        }
        if (line.indexOf(UST_VERSION_PREFIX_NOSPACE) == 0) {
            version.version = line.mid(int(strlen(UST_VERSION_PREFIX_NOSPACE))).simplified();
            flag = true;
        }
    }

    return flag;
}

bool QUstFile::parseSectionSettings(const QStringList &sectionList, QUstSettings &settings) {
    QStringList::size_type i;
    bool isValid = true;

    int eq;
    QString key, value;
    double num;
    bool isNum;

    for (i = 0; i < sectionList.size(); ++i) {
        eq = sectionList[i].indexOf("=");
        if (eq <= 0) {
            continue;
        }

        key = sectionList[i].left(eq);
        value = sectionList[i].mid(eq + 1);

        if (key == KEY_NAME_PROJECT_NAME) {
            settings.projectName = value; // Project Name
        } else if (key == KEY_NAME_OUTPUT_FILE) {
            settings.outputFileName = value; // Output File Name
        } else if (key == KEY_NAME_VOICE_DIR) {
            settings.voiceDirectory =
                QUtaUtils::fromUSTVoiceDir(value, voiceDir); // Voice Directory
        } else if (key == KEY_NAME_CACHE_DIR) {
            settings.cacheDirectory = value; // Cache Directory
        } else if (key == KEY_NAME_TOOL1) {
            settings.wavtoolPath = QUtaUtils::fromUSTToolsDir(value); // Wavtool
        } else if (key == KEY_NAME_TOOL2) {
            settings.resamplerPath = QUtaUtils::fromUSTToolsDir(value); // Resampler
        } else if (key == KEY_NAME_MODE2) {
            if (value != "True") {
                isValid = false;
            }
            settings.isMode2 = true; // Mode2
        } else if (key == KEY_NAME_TEMPO) {
            num = value.toDouble(&isNum);
            if (!isNum) {
                isValid = false;
            } else {
                settings.globalTempo = num; // Global Tempo
            }
        } else if (key == KEY_NAME_FLAGS) {
            settings.globalFlags = value; // Flags
        }
    }
    return isValid;
}

void QUstFile::writeSectionName(const int &name, QTextStream &out) {
    QString newName = QString::number(name);
    int nums = newName.size();
    for (int i = 0; i < 4 - nums; ++i) {
        newName.prepend("0");
    }
    writeSectionName(newName, out);
}

void QUstFile::writeSectionName(const QString &name, QTextStream &out) {
    out << SECTION_BEGIN_MARK + name + SECTION_END_MARK << Qt::endl;
}

void QUstFile::writeSectionNote(int num, const QUstNote &note, QTextStream &out) {
    writeSectionName(num, out);

    // Items maybe not exist
    QString aVibrato = QUtaUtils::DoublesToStrings(note.vibrato).join(COMMA);
    QString aPitchBend = QUtaUtils::DoublesToStrings(note.pitches).join(COMMA);

    // Complex items
    QUtaUtils::PBStrings mode2;
    QString strEnvelope;

    mode2 = QUtaUtils::PortamentoToString(note.Mode2Pitch);
    strEnvelope = QUtaUtils::EnvelopeToString(note.envelope);

    // Items always exists
    out << KEY_NAME_LENGTH << "=" << note.length << Qt::endl;
    out << KEY_NAME_LYRIC << "=" << note.lyric << Qt::endl;
    out << KEY_NAME_NOTE_NUM << "=" << note.noteNum << Qt::endl;

    // Items can be omitted
    if (note.preUttr != QUtaUtils::NODEF_DOUBLE) {
        out << KEY_NAME_PRE_UTTERANCE << "=" << note.preUttr << Qt::endl;
    }
    if (note.overlap != QUtaUtils::NODEF_DOUBLE) {
        out << KEY_NAME_VOICE_OVERLAP << "=" << note.overlap << Qt::endl;
    }
    if (note.velocity != QUtaUtils::NODEF_DOUBLE) {
        out << KEY_NAME_VELOCITY << "=" << QString::number(note.velocity) << Qt::endl;
    }
    if (note.intensity != QUtaUtils::NODEF_DOUBLE) {
        out << KEY_NAME_INTENSITY << "=" << note.intensity << Qt::endl;
    }
    if (note.modulation != QUtaUtils::NODEF_DOUBLE) {
        out << KEY_NAME_MODULATION << "=" << note.modulation << Qt::endl;
    }
    if (note.stp != QUtaUtils::NODEF_DOUBLE) {
        out << KEY_NAME_START_POINT << "=" << note.stp << Qt::endl;
    }
    if (!note.flags.isEmpty()) {
        out << KEY_NAME_FLAGS << "=" << note.flags << Qt::endl;
    }

    // Items may not exist
    if (!note.pitches.isEmpty()) {
        out << KEY_NAME_PB_TYPE << "=" << VALUE_PITCH_TYPE << Qt::endl;
        out << KEY_NAME_PB_START << "=" << note.pbstart << Qt::endl;
        out << KEY_NAME_PITCH_BEND << "=" << aPitchBend << Qt::endl;
    }
    if (!note.Mode2Pitch.isEmpty()) {
        out << KEY_NAME_PBS << "=" << mode2.PBS << Qt::endl;
        out << KEY_NAME_PBW << "=" << mode2.PBW << Qt::endl;
        if (!mode2.PBY.isEmpty()) {
            out << KEY_NAME_PBY << "=" << mode2.PBY << Qt::endl;
        }
        if (!mode2.PBS.isEmpty()) {
            out << KEY_NAME_PBM << "=" << mode2.PBM << Qt::endl;
        }
    }
    if (!note.envelope.isEmpty()) {
        out << KEY_NAME_ENVELOPE << "=" << strEnvelope << Qt::endl;
    }
    if (!note.vibrato.isEmpty()) {
        out << KEY_NAME_VBR << "=" << aVibrato << Qt::endl;
    }
    if (note.tempo != QUtaUtils::NODEF_DOUBLE) {
        out << KEY_NAME_TEMPO << "=" << note.tempo << Qt::endl;
    }
    if (note.region != QUtaUtils::NODEF_STRING) {
        out << KEY_NAME_REGION_START << "=" << note.region << Qt::endl;
    }
    if (note.regionEnd != QUtaUtils::NODEF_STRING) {
        out << KEY_NAME_REGION_END << "=" << note.regionEnd << Qt::endl;
    }

    // Custom Values
    for (int i = 0; i < note.customData.size(); ++i) {
        out << note.customData[i].first << "=" << note.customData[i].second << Qt::endl;
    }
}

void QUstFile::writeSectionVersion(QTextStream &out) {
    writeSectionName(SECTION_NAME_VERSION, out);

    out << UST_VERSION_PREFIX_NOSPACE << sectionVersion.version << Qt::endl;

    // UTF-8 UST File?
    QString charset = sectionVersion.charset;
    if (!charset.isEmpty()) {
        out << KEY_NAME_CHARSET << "=" << charset << Qt::endl;
    }
}

void QUstFile::writeSectionSettings(QTextStream &oStream) {
    writeSectionName(SECTION_NAME_SETTING, oStream);

    oStream << KEY_NAME_TEMPO << "=" << sectionSettings.globalTempo << Qt::endl;
    oStream << KEY_NAME_TRACKS << "=" << VALUE_TRACKS_SINGLE << Qt::endl;
    oStream << KEY_NAME_PROJECT_NAME << "=" << sectionSettings.projectName << Qt::endl;
    oStream << KEY_NAME_VOICE_DIR << "="
            << QUtaUtils::toUSTVoiceDir(sectionSettings.voiceDirectory, voiceDir) << Qt::endl;
    oStream << KEY_NAME_OUTPUT_FILE << "=" << sectionSettings.outputFileName << Qt::endl;
    oStream << KEY_NAME_CACHE_DIR << "=" << sectionSettings.cacheDirectory << Qt::endl;
    oStream << KEY_NAME_TOOL1 << "=" << QUtaUtils::toUSTToolsDir(sectionSettings.wavtoolPath)
            << Qt::endl;
    oStream << KEY_NAME_TOOL2 << "=" << QUtaUtils::toUSTToolsDir(sectionSettings.resamplerPath)
            << Qt::endl;

    if (sectionSettings.isMode2) {
        oStream << KEY_NAME_MODE2 << "=" << VALUE_MODE2_ON << Qt::endl;
    }

    if (!sectionSettings.globalFlags.isEmpty()) {
        oStream << KEY_NAME_FLAGS << "=" << sectionSettings.globalFlags << Qt::endl;
    }
}
