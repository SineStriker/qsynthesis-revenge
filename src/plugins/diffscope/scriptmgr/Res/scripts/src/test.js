"use strict";

$ds.register(new class extends $ds.Script {
    info() {
        return {
            id: 'test',
            role: 'all',
            name: this.tr('Test'),
            requiredEditorVersion: '0.1.0',
        };
    }
    main() {
        let res = $ds.dialogSystem.form(
            this.tr('Test'),
            [{
                type: 'Select',
                label: this.tr('Preset'),
                options: ['1919810', '19260817', this.tr('Customize')],
                bindings: [{
                    index: 0,
                    disable: [1],
                }, {
                    index: 1,
                    disable: [1],
                }, {
                    index: 2,
                    enable: [1],
                }],
            }, {
                type: 'NumberBox',
                label: this.tr('Test'),
                suffix: 'RMB',
                step: 2,
                defaultValue: 114514,
            }],
        );
        if (res.result == 'Ok') {
            let value;
            if(res.form[0] == 0) value = 1919810;
            else if(res.form[0] == 1) value = 19260817;
            else value = res.form[1];
            $ds.dialogSystem.alert(this.tr('Test'), value);
        }
    }
    locale(lang, text) {
        let dict = {
            'zh_CN': new Map([
                ['Test', '测试']
            ]),
            'ja_JP': new Map([
                ['Test', 'テスト']
            ]),
        };
        if(dict[lang]) {
            return dict[lang].get(text);
        } else {
            return undefined;
        }
    }
});
