#ifndef CHORUSKIT_KEYNAMESPINBOX_H
#define CHORUSKIT_KEYNAMESPINBOX_H

#include <QSpinBox>

#include "KeyNameValidator.h"

namespace ScriptMgr::Internal {

    class KeyNameSpinBox: public QSpinBox {
        Q_OBJECT
    public:
        explicit KeyNameSpinBox(QWidget *parent = nullptr);
        void setAccidentalType(KeyNameValidator::AccidentalType accidentalType);
        KeyNameValidator::AccidentalType accidentalType() const;
        void setMinimum(int min);
        void setMaximum(int max);
        void setRange(int min, int max);
        int displayIntegerBase() const = delete;
        void setDisplayIntegerBase(int base) = delete;

    protected:
        QString textFromValue(int val) const override;
        int valueFromText(const QString &text) const override;
        void fixup(QString &str) const override;
        QValidator::State validate(QString &input, int &pos) const override;
    private:
        KeyNameValidator m_validator;
    };

} // Internal

#endif // CHORUSKIT_KEYNAMESPINBOX_H
