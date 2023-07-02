$register(class extends $ScriptSet {
    static info() {
        return {
            id: 'transpose',
            vendor: 'CrSjimo',
            description: '', //TODO
            version: '0.0.1',
            requiredVersion: '0.1.0',
            role: ['pianoroll', 'arrangement'],
            name: __q_tr('JsBuiltIn', 'Transpose'),
            children: [{
                id: 'plus_semitone',
                name: __q_tr('JsBuiltIn', 'Shift Up by a Semitone'),
                shortcut: 'Ctrl+Up',
            }, {
                id: 'minus_semitone',
                name: __q_tr('JsBuiltIn', 'Shift Down by a Semitone'),
                shortcut: 'Ctrl+Down',
            }, {
                id: 'plus_octave',
                name: __q_tr('JsBuiltIn', 'Shift Up by an Octave'),
                shortcut: 'Ctrl+Shift+Up',
            }, {
                id: 'minus_octave',
                name: __q_tr('JsBuiltIn', 'Shift Down by an Octave'),
                shortcut: 'Ctrl+Shift+Down',
            }, {
                id: 'customize',
                name: __q_tr('JsBuiltIn', 'Customize Transposition')
            }, {
                id: 'shift_mode',
                name: __q_tr('JsBuiltIn', 'Shift Mode')
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
                __q_tr('JsBuiltIn', 'Transpose'),
                [{
                    type: 'NumberBox',
                    label: __q_tr('JsBuiltIn', 'Shift by'),
                    suffix: __q_tr('JsBuiltIn', ' semitone(s)'),
                }],
            );
            if(res.result == 'Ok') {
                this.transpose(res.form[0]);
            }
        } else {
            let res = this.ds.dialogSystem.form(
                __q_tr('JsBuiltIn', 'Shift Mode'),
                [{
                    type: 'Select',
                    label: __q_tr('JsBuiltIn', 'Current mode'),
                    options: [__q_tr('JsBuiltIn', 'Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', __q_tr('JsBuiltIn', 'Aeolian (Minor mode)'), 'Locrian'],
                }, {
                    type: 'Select',
                    label: __q_tr('JsBuiltIn', 'Target mode'),
                    options: [__q_tr('JsBuiltIn', 'Ionian (Major mode)'), 'Dorian', 'Phrygian', 'Lydian', 'Mixolydian', __q_tr('JsBuiltIn', 'Aeolian (Minor mode)'), 'Locrian'],
                }],
            )
        }
    }
    transpose(degrees) {
        this.ds.dialogSystem.alert(__q_tr('JsBuiltIn', 'Transpose'), degrees);
    }
    shiftMode(currentMode, targetMode) {

    }
});