'use strict';

$ds.register(new class extends $ds.ScriptSet {
    info() {
        return {
            id: 'transpose',
            role: ['pianoroll', 'arrangement'],
            name: _tr("JsBuiltIn", 'Transpose'),
            children: [{
                id: 'plus_octave',
                name: _tr("JsBuiltIn", 'Shift Up by an Octave')
            }, {
                id: 'minus_octave',
                name: _tr("JsBuiltIn", 'Shift Down by an Octave')
            }, {
                id: 'customize',
                name: _tr("JsBuiltIn", 'Customize Transposition')
            }, {
                id: 'shift_mode',
                name: _tr("JsBuiltIn", 'Shift Mode')
            }],
            requiredEditorVersion: '0.1.0',
        };
    }
    main(index) {
        if(index == 0) this._transpose(12);
        else if(index == 1) this._transpose(-12);
        else if(index == 2) {
            let res = $ds.dialogSystem.form(
                _tr("JsBuiltIn", 'Transpose'),
                [{
                    type: 'NumberBox',
                    label: _tr("JsBuiltIn", 'Shift by'),
                    suffix: _tr("JsBuiltIn", ' semitone(s)'),
                }],
            );
            if(res.result == 'Ok') {
                this._transpose(res.form[0]);
            }
        } else {
            let res = $ds.dialogSystem.form(
                _tr("JsBuiltIn", 'Shift Mode'),
                [{
                    type: 'Select',
                    label: _tr("JsBuiltIn", 'Current mode'),
                    options: [_tr("JsBuiltIn", 'Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', _tr("JsBuiltIn", 'Aeolian (Minor mode)'), 'Locrian'],
                }, {
                    type: 'Select',
                    label: _tr("JsBuiltIn", 'Target mode'),
                    options: [_tr("JsBuiltIn", 'Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', _tr("JsBuiltIn", 'Aeolian (Minor mode)'), 'Locrian'],
                }],
            )
        }
    }
    _transpose(degrees) {

    }
    _shiftMode(currentKeynote, currentMode, targetKeynote, targetMode) {

    }
});