class $DS {

    constructor(internal) {

        this.internal = internal;

        this.dialogSystem = {
            form: (title, widgets, listener) => {
                return this.internal.form(title, widgets, listener ? (rawHandle) => {
                    listener(widgets.map((v, i) => {
                        let handle = {};
                        Object.defineProperties(handle, {
                            value: {
                                get: () => rawHandle.value(i),
                            },
                            visible: {
                                get: () => rawHandle.visible(i),
                                set: v => { rawHandle.setVisible(i, v) },
                            },
                            disabled: {
                                get: () => rawHandle.disabled(i),
                                set: v => { rawHandle.setDisabled(i, v) },
                            },
                            label: {
                                get: () => rawHandle.label(i),
                                set: v => { rawHandle.setLabel(i, v) },
                            },
                        });
                        return handle;
                    }));
                } : undefined);
            },
            alert: (title, message) => {
                this.internal.infoMsgBox(title, message);
            },
            confirm: (title, message, defaultButton = 'Yes') => {
                return this.internal.questionMsgBox(title, message, defaultButton);
            },
            prompt: (title, message, defaultValue) => {
                let res =  this.internal.form(
                    title,
                    [{ type: 'TextBox', label: message, defaultValue }],
                );
                if(res.result == 'Ok') {
                    return res.form[0];
                } else {
                    return null;
                }
            },
            msgBox: (title, message, icon = '', buttons = ['Ok'], defaultButton = '') => {
                return this.internal.msgBox(title, message, icon, buttons, defaultButton);
            }
        };

        this.actionSystem = {};

        this.project = {};

        this.utils = {};
    }

}

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
        let translation = entry[this.internal.getLang()];
        if (typeof(translation) !== 'string') return __q_tr_ext.qsTranslate('JsBuiltIn', text);
        return translation;
    }
}

export class Script extends ScriptBase {
    main() { }
    static info() { return null; }
}

export class ScriptSet extends ScriptBase {
    main(index) { }
    static info() { return null; }
}

var registry = new Map();

export function register(clazz) {
    let info = clazz.info();
    //TODO check info
    if(registry.has(info.id)) return;
    registry.set(info.id, clazz);
    if(clazz.prototype instanceof Script) {
        __q_loader.registerScript(info.id, [].concat(info.role), info.shortcut ?? "");
    } else if(clazz.prototype instanceof ScriptSet) {
        __q_loader.registerScriptSet(info.id, info.children.map(c => c.id), [].concat(info.role), info.children.map(c => c.shortcut ?? ""));
    }
}

var instanceRegistry = new Map();

export var __q_callbacks = {};

__q_callbacks.createHandles = function (internal) {
    let windowKey = __q_loader.uuid();
    let currentWindowRegistry = new Map()
    let ds = new $DS(internal)
    for(let [id, clazz] of registry.entries()) {
        currentWindowRegistry.set(id, new clazz(ds));
    }
    instanceRegistry.set(windowKey, currentWindowRegistry);
    return windowKey;
}

__q_callbacks.invoke = function (windowKey, id, index) {
    let script = instanceRegistry.get(windowKey).get(id);
    if(script instanceof Script) {
        script.main();
    } else if(script instanceof ScriptSet) {
        script.main(index);
    }
}

__q_callbacks.getName = function (id, index) {
    let clazz = registry.get(id);
    if(clazz.prototype instanceof Script) {
        return clazz.info().name;
    } else if(clazz.prototype instanceof ScriptSet) {
        if(typeof(index) === 'number') {
            return clazz.info().children[index].name;
        } else {
            return clazz.info().name;
        }
    }
}

__q_callbacks.getInfo = function(id) {
    let clazz = registry.get(id);
    return clazz.info();
}

__q_callbacks.removeHandles = function (windowKey) {
    instanceRegistry.delete(windowKey);
}