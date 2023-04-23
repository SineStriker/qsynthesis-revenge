//
// Created by Crs_1 on 2023/4/23.
//

#include "KeyNameValidator.h"

namespace ScriptMgr::Internal {

    QString KeyNameValidator::toKeyName(int noteNumber, KeyNameValidator::AccidentalType accidentalType) {
        noteNumber = std::min(127, noteNumber);
        noteNumber = std::max(0, noteNumber);
        auto octave = noteNumber / 12;
        auto keyNames = accidentalType == Sharp ?
                                                QStringList({"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"}):
                                                QStringList({"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"});
        auto noteName = keyNames[noteNumber % 12];
        return QString("%1 %2").arg(noteName, QString::number(octave));
    }
    int KeyNameValidator::toNoteNumber(const QString &keyName) {
        return 0; //TODO
    }

    KeyNameValidator::KeyNameValidator(QObject *parent) : KeyNameValidator(0, 127, Sharp, parent) {
    }
    KeyNameValidator::KeyNameValidator(int bottom, int top, AccidentalType accidentalType, QObject *parent) : QIntValidator(parent) {
        b = std::max(0, bottom);
        t = std::min(127, top);
        m_accidentalType = accidentalType;
    }
    KeyNameValidator::~KeyNameValidator() {

    }

    static bool validateAccidental(const QString &keyChar, const QString &currentAcc, KeyNameValidator::AccidentalType accidentalType) {
        if(accidentalType == KeyNameValidator::Sharp && currentAcc == "b") return false;
        if(accidentalType == KeyNameValidator::Flat && currentAcc == "#") return false;
        if(currentAcc == "#" && (keyChar == "E" || keyChar == "B")) return false;
        if(currentAcc == "b" && (keyChar == "F" || keyChar == "C")) return false;
        return true;
    }

    QValidator::State KeyNameValidator::validate(QString &input, int &pos) const {
        Q_UNUSED(pos)
        QRegExp rx1("^[A-G]$");
        QRegExp rx2("^([A-G])([#b]?) ?$");
        QRegExp rx3("^([A-G])([#b]?) (\\d+)$");
        if(rx3.exactMatch(input)) {
            auto keyChar = rx3.cap(1).toUpper();
            auto currentAcc = rx3.cap(2);
            auto octave = rx3.cap(3).toInt();
            if(!validateAccidental(keyChar, currentAcc, m_accidentalType)) return Invalid;
            if(octave > 10) return Invalid;
            if(octave == 10) {
                if(keyChar == "A" || keyChar == "B") return Invalid;
                if(keyChar == "G" && !currentAcc.isEmpty()) return Invalid;
            }
            return Acceptable;
        } else if(rx2.exactMatch(input)) {
            auto keyChar = rx2.cap(1).toUpper();
            auto currentAcc = rx2.cap(2);
            if(!validateAccidental(keyChar, currentAcc, m_accidentalType)) return Invalid;
            return Intermediate;
        } else if(rx1.exactMatch(input)) {
            return Intermediate;
        } else {
            return Invalid;
        }
    }
    static QString convertEnharmonic(const QString &keyChar, const QString &currentAcc, KeyNameValidator::AccidentalType targetAcc) {
        if(currentAcc.isEmpty()) return keyChar;
        if(currentAcc == "#" && targetAcc == KeyNameValidator::Sharp) {
            if(keyChar == "E") return "F";
            if(keyChar == "B") return "C";
            return keyChar + "#";
        } else if(currentAcc == "b" && targetAcc == KeyNameValidator::Flat) {
            if(keyChar == "F") return "E";
            if(keyChar == "C") return "B";
            return keyChar + "b";
        } else if(currentAcc == "#") {
            if(keyChar == "A") return "Bb";
            if(keyChar == "B") return "C";
            if(keyChar == "C") return "Db";
            if(keyChar == "D") return "Eb";
            if(keyChar == "E") return "F";
            if(keyChar == "F") return "Gb";
            if(keyChar == "G") return "Ab";
        } else {
            if(keyChar == "A") return "G#";
            if(keyChar == "B") return "A#";
            if(keyChar == "C") return "B";
            if(keyChar == "D") return "C#";
            if(keyChar == "E") return "D#";
            if(keyChar == "F") return "E";
            if(keyChar == "G") return "F#";
        }
    }

    void KeyNameValidator::fixup(QString &input) const {
        auto inputTrimmed = input.trimmed();
        QRegExp numberRx("^\\d+$");
        if(numberRx.exactMatch(inputTrimmed)) {
            input = toKeyName(inputTrimmed.toInt(), m_accidentalType);
            return;
        }
        QRegExp keyNameRx("^([A-Ga-g])([#b]?)\\s*(\\d+)$");
        if(keyNameRx.exactMatch(inputTrimmed)) {
            auto keyChar = keyNameRx.cap(1).toUpper();
            auto currentAcc = keyNameRx.cap(2);
            auto octave = keyNameRx.cap(3).toInt();
            octave = std::min(octave, 10);
            if(octave == 10) {
                if(keyChar == "A" || keyChar == "B") keyChar = "G";
                if(keyChar == "G" && !currentAcc.isEmpty()) currentAcc.clear();
            }
            input = QString("%1 %2").arg(convertEnharmonic(keyChar, currentAcc, m_accidentalType), QString::number(octave));
            return;
        }
    }
    void KeyNameValidator::setRange(int bottom, int top) {
        bool rangeChanged = false;
        bottom = std::max(0, bottom);
        top = std::min(127, top);
        if (b != bottom) {
            b = bottom;
            rangeChanged = true;
            emit bottomChanged(b);
        }

        if (t != top) {
            t = top;
            rangeChanged = true;
            emit topChanged(t);
        }

        if (rangeChanged)
            emit changed();
    }

    void KeyNameValidator::setAccidentalType(KeyNameValidator::AccidentalType accidentalType) {
        if(accidentalType != m_accidentalType) {
            m_accidentalType = accidentalType;
            emit accidentalTypeChanged(accidentalType);
            emit changed();
        }
    }
    KeyNameValidator::AccidentalType KeyNameValidator::accidentalType() const {
        return m_accidentalType;
    }
} // Internal