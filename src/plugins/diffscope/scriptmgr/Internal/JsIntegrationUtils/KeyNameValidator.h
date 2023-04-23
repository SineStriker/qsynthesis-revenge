//
// Created by Crs_1 on 2023/4/23.
//

#ifndef CHORUSKIT_KEYNAMEVALIDATOR_H
#define CHORUSKIT_KEYNAMEVALIDATOR_H

#include <QIntValidator>
namespace ScriptMgr::Internal {

    class KeyNameValidator: public QIntValidator {
        Q_OBJECT
    public:
        enum AccidentalType {
            Sharp,
            Flat,
        };
        explicit KeyNameValidator(QObject * parent = nullptr);
        KeyNameValidator(int bottom, int top, AccidentalType accidentalType = Sharp, QObject *parent = nullptr);
        ~KeyNameValidator();

        QValidator::State validate(QString &, int &) const override;
        void fixup(QString &input) const override;

        void setRange(int bottom, int top) override;

        void setAccidentalType(AccidentalType accidentalType);
        AccidentalType accidentalType() const;
    signals:
        void accidentalTypeChanged(AccidentalType accidentalType);
    public:
        static QString toKeyName(int noteNumber, AccidentalType accidentalType = Sharp);
        static int toNoteNumber(const QString &keyName);
    private:
        int b;
        int t;
        AccidentalType m_accidentalType;
    };

} // Internal

#endif // CHORUSKIT_KEYNAMEVALIDATOR_H
