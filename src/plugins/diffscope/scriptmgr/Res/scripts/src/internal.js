"use strict";

class _ScriptBase {
    locale() { }
    tr(text) {
        let locale = this.locale();
        if(!locale) return text;
        let entry = locale[text];
        if(!entry) return text;
        let translation = entry[_internal.getLang()];
        if (typeof(translation) !== 'string') return text;
        return translation;
    }
}

var _registry = new Map();

var _invoke = (id, index) => {
    let script = _registry.get(id);
    if(script instanceof $ds.Script) {
        script.main();
    } else if(script instanceof $ds.ScriptSet) {
        script.main(index);
    }
}

var _getName = (id, index) => {
    let script = _registry.get(id);
    if(script instanceof $ds.Script) {
        return script.info().name;
    } else if(script instanceof $ds.ScriptSet) {
        if(typeof(index) === 'number') {
            return script.info().children[index].name;
        } else {
            return script.info().name;
        }
    }
}

var _tr = _q_tr_ext.qsTranslate;

var $ds = {
    dialogSystem: {
        form: _internal.form,
        alert: (title, message) => {
            _internal.infoMsgBox(title, message);
        },
        confirm: (title, message, defaultButton) => {
            return _internal.questionMsgBox(title, message, defaultButton);
        },
        prompt: (title, message) => {
            let res =  _internal.form(
                title,
                [{ type: 'TextBox', label: message }],
            );
            if(res.result == 'Ok') {
                return res.form[0];
            } else {
                return null;
            }
        }
    },
    actionSystem: {},
    project: {},
    utils: {},
    _ScriptBase,
    Script: class Script extends _ScriptBase {
        main() { }
        info() { return null; }
    },
    ScriptSet: class ScriptSet extends _ScriptBase {
        main(index) { }
        info() { return null; }
    },
    register: (script) => {
        //TODO check version
        script.info().requiredEditorVersion;
        let flags = 0;
        for (let flag of [].concat(script.info().role)) {
            switch (flag) {
                case 'all':
                    flags |= -1;
                    break;
                case 'arrangement':
                    flags |= 1;
                    break;
                case 'pianoroll':
                    flags |= 2;
                    break;
                case 'parameter':
                    flags |= 4;
                    break;
            }
        }
        _registry.set(script.info().id, script);
        if(script instanceof $ds.Script) {
            _internal.registerScript(script.info().id, flags);
        } else if(script instanceof $ds.ScriptSet) {
            _internal.registerScriptSet(script.info().id, script.info().children.map(c => c.id), flags);
        }
    }
};