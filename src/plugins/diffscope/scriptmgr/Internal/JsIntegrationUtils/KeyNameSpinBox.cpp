#include "KeyNameSpinBox.h"

namespace ScriptMgr::Internal {

    void KeyNameSpinBox::setAccidentalType(KeyNameValidator::AccidentalType accidentalType) {
        m_validator.setAccidentalType(accidentalType);
    }
    KeyNameValidator::AccidentalType KeyNameSpinBox::accidentalType() const {
        return m_validator.accidentalType();
    }
    KeyNameSpinBox::KeyNameSpinBox(QWidget *parent) : QSpinBox(parent) {
        QSpinBox::setRange(0, 127);
    }
    QString KeyNameSpinBox::textFromValue(int val) const {
        return KeyNameValidator::toKeyName(val, m_validator.accidentalType());
    }
    int KeyNameSpinBox::valueFromText(const QString &text) const {
        return KeyNameValidator::toNoteNumber(text);
    }
    void KeyNameSpinBox::fixup(QString &str) const {
        m_validator.fixup(str);
    }
    QValidator::State KeyNameSpinBox::validate(QString &input, int &pos) const {
        return m_validator.validate(input, pos);
    }
    void KeyNameSpinBox::setMinimum(int min) {
        QSpinBox::setMinimum(std::max(0, min));
    }
    void KeyNameSpinBox::setMaximum(int max) {
        QSpinBox::setMaximum(std::min(127, max));
    }
    void KeyNameSpinBox::setRange(int min, int max) {
        QSpinBox::setRange(std::max(0, min), std::min(127, max));
    }

} // Internal