"use strict";

$ds.register('test', ['all'], new class extends $ds.Script {
    info() {
        return {
            name: this.tr('Test'),
            requiredEditorVersion: '0.1.0',
        };
    }
    main() {
        let res = $ds.dialogSystem.form({
            title: this.tr('Test'),
            widgets: [{
                type: 'TextBox',
                label: this.tr('Test')
            }],
        });
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
        return dict?.[lang].get(text);
    }
});
