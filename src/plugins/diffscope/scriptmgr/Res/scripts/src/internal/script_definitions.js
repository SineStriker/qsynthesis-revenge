class ScriptBase {
    constructor(ds) {
        this.ds = ds;
    }
    static locale() { }
    static tr(text) {
        let locale = this.locale();
        if(!locale) return __q_tr_ext.qsTranslate('JsBuiltIn', text);
        let entry = locale[text];
        if(!entry) return __q_tr_ext.qsTranslate('JsBuiltIn', text);
        let translation = entry[__q_loader.getLang()];
        if (typeof(translation) !== 'string') return __q_tr_ext.qsTranslate('JsBuiltIn', text);
        return translation;
    }
}

export class Script extends ScriptBase {
    static info() { return null; }
}

export class ScriptSet extends ScriptBase {
    static info() { return null; }
}