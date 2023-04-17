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
                type: 'TextBox',
                label: this.tr('Test'),
                defaultValue: '114514',
            }],
        );
        if (res.result == 'Ok') {
            $ds.dialogSystem.alert(this.tr('Test'), res.form[0]);
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
