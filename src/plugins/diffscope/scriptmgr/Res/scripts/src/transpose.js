$register(class extends $ScriptSet {
    static info() {
        return {
            id: 'transpose',
            vendor: 'CrSjimo',
            description: '', //TODO
            version: '0.0.1',
            requiredVersion: '0.1.0',
            role: ['pianoroll', 'arrangement'],
            name: $tr('JsBuiltIn', 'Transpose'),
            children: [{
                id: 'plus_semitone',
                name: $tr('JsBuiltIn', 'Shift Up by a Semitone'),
                shortcut: 'Ctrl+Up',
            }, {
                id: 'minus_semitone',
                name: $tr('JsBuiltIn', 'Shift Down by a Semitone'),
                shortcut: 'Ctrl+Down',
            }, {
                id: 'plus_octave',
                name: $tr('JsBuiltIn', 'Shift Up by an Octave'),
                shortcut: 'Ctrl+Shift+Up',
            }, {
                id: 'minus_octave',
                name: $tr('JsBuiltIn', 'Shift Down by an Octave'),
                shortcut: 'Ctrl+Shift+Down',
            }, {
                id: 'customize',
                name: $tr('JsBuiltIn', 'Customize Transposition')
            }, {
                id: 'shift_mode',
                name: $tr('JsBuiltIn', 'Shift Mode')
            }],
        };
    }
    main(index) {
        if(index == 0) this.transpose(1);
        else if(index == 1) this.transpose(-1);
        else if(index == 2) this.transpose(12);
        else if(index == 3) this.transpose(-12);
        else if(index == 4) {
            let res = this.ds.dialogSystem.form(
                $tr('JsBuiltIn', 'Transpose'),
                [{
                    type: 'NumberBox',
                    label: $tr('JsBuiltIn', 'Shift by'),
                    suffix: $tr('JsBuiltIn', ' semitone(s)'),
                }],
            );
            if(res.result == 'Ok') {
                this.transpose(res.form[0]);
            }
        } else {
            let res = this.ds.dialogSystem.form(
                $tr('JsBuiltIn', 'Shift Mode'),
                [{
                    type: 'Select',
                    label: $tr('JsBuiltIn', 'Current mode'),
                    options: [$tr('JsBuiltIn', 'Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', $tr('JsBuiltIn', 'Aeolian (Minor mode)'), 'Locrian'],
                }, {
                    type: 'Select',
                    label: $tr('JsBuiltIn', 'Target mode'),
                    options: [$tr('JsBuiltIn', 'Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', $tr('JsBuiltIn', 'Aeolian (Minor mode)'), 'Locrian'],
                }],
            )
        }
    }
    transpose(degrees) {
        this.ds.dialogSystem.alert($tr('JsBuiltIn', 'Transpose'), degrees);
    }
    shiftMode(currentMode, targetMode) {

    }
});