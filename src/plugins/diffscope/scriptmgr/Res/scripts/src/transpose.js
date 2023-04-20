'use strict';

$ds.register(new class extends $ds.ScriptSet {
    info() {
        return {
            id: 'transpose',
            role: ['pianoroll', 'arrangement'],
            name: qsTranslate("JSScript", 'Transpose'),
            children: [{
                id: 'plus_octave',
                name: qsTranslate("JSScript", 'Shift Up by an Octave')
            }, {
                id: 'minus_octave',
                name: qsTranslate("JSScript", 'Shift Down by an Octave')
            }, {
                id: 'customize',
                name: qsTranslate("JSScript", 'Customize Transposition')
            }, {
                id: 'shift_mode',
                name: qsTranslate("JSScript", 'Shift Mode')
            }],
            requiredEditorVersion: '0.1.0',
        };
    }
    main(index) {
        if(index == 0) this._transpose(12);
        else if(index == 1) this._transpose(-12);
        else if(index == 2) {
            let res = $ds.dialogSystem.form(
                qsTranslate("JSScript", 'Transpose'),
                [{
                    type: 'NumberBox',
                    label: qsTranslate("JSScript", 'Shift by'),
                    suffix: qsTranslate("JSScript", ' semitone(s)'),
                }],
            );
            if(res.result == 'Ok') {
                this._transpose(res.form[0]);
            }
        } else {
            let res = $ds.dialogSystem.form(
                qsTranslate("JSScript", 'Shift Mode'),
                [{
                    type: 'Label',
                    label: qsTranslate("JSScript", 'Current mode: '),
                }, {
                    type: 'Select',
                    label: qsTranslate("JSScript", 'Keynote'),
                    options: ['C', 'C#/Db', 'D', 'D#/Eb', 'E', 'F', 'F#/Gb', 'G', 'G#/Ab', 'A', 'A#/Bb', 'B'],
                }, {
                    type: 'Select',
                    label: qsTranslate("JSScript", 'Mode name'),
                    options: [qsTranslate("JSScript", 'Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', qsTranslate("JSScript", 'Aeolian (Minor mode)'), 'Locrian'],
                }, {
                    type: 'Label',
                    label: qsTranslate("JSScript", 'Target mode: '),
                }, {
                    type: 'Select',
                    label: qsTranslate("JSScript", 'Keynote'),
                    options: ['C', 'C#/Db', 'D', 'D#/Eb', 'E', 'F', 'F#/Gb', 'G', 'G#/Ab', 'A', 'A#/Bb', 'B'],
                }, {
                    type: 'Select',
                    label: qsTranslate("JSScript", 'Mode name'),
                    options: [qsTranslate("JSScript", 'Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', qsTranslate("JSScript", 'Aeolian (Minor mode)'), 'Locrian'],
                }],
            )
        }
    }
    _transpose(degrees) {

    }
    _shiftMode(currentKeynote, currentMode, targetKeynote, targetMode) {

    }
});