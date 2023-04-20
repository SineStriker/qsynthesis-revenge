'use strict';

$ds.register(new class extends $ds.ScriptSet {
    info() {
        return {
            id: 'transpose',
            role: ['pianoroll', 'arrangement'],
            name: this.tr('Transpose'),
            children: [{
                id: 'plus_octave',
                name: this.tr('Shift Up by an Octave')
            }, {
                id: 'minus_octave',
                name: this.tr('Shift Down by an Octave')
            }, {
                id: 'customize',
                name: this.tr('Customize Transposition')
            }, {
                id: 'shift_mode',
                name: this.tr('Shift Mode')
            }],
            requiredEditorVersion: '0.1.0',
        };
    }
    main(index) {
        if(index == 0) this._transpose(12);
        else if(index == 1) this._transpose(-12);
        else if(index == 2) {
            let res = $ds.dialogSystem.form(
                this.tr('Transpose'),
                [{
                    type: 'NumberBox',
                    label: this.tr('Shift by'),
                    suffix: this.tr(' semitone(s)'),
                }],
            );
            if(res.result == 'Ok') {
                this._transpose(res.form[0]);
            }
        } else {
            let res = $ds.dialogSystem.form(
                this.tr('Shift Mode'),
                [{
                    type: 'Label',
                    label: this.tr('Current mode: '),
                }, {
                    type: 'Select',
                    label: this.tr('Keynote'),
                    options: ['C', 'C#/Db', 'D', 'D#/Eb', 'E', 'F', 'F#/Gb', 'G', 'G#/Ab', 'A', 'A#/Bb', 'B'],
                }, {
                    type: 'Select',
                    label: this.tr('Mode name'),
                    options: [this.tr('Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', this.tr('Aeolian (Minor mode)'), 'Locrian'],
                }, {
                    type: 'Label',
                    label: this.tr('Target mode: '),
                }, {
                    type: 'Select',
                    label: this.tr('Keynote'),
                    options: ['C', 'C#/Db', 'D', 'D#/Eb', 'E', 'F', 'F#/Gb', 'G', 'G#/Ab', 'A', 'A#/Bb', 'B'],
                }, {
                    type: 'Select',
                    label: this.tr('Mode name'),
                    options: [this.tr('Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', this.tr('Aeolian (Minor mode)'), 'Locrian'],
                }],
            )
        }
    }
    _transpose(degrees) {

    }
    _shiftMode(currentKeynote, currentMode, targetKeynote, targetMode) {

    }
});