import { DiffScopeHandle } from './DiffScopeHandle.js';

import { Script, ScriptSet } from './script_definitions.js';

import { registry } from './register.js';

var instanceRegistry = new Map();

export var __q_callbacks = {};

__q_callbacks.createHandles = function (internal) {
    let windowKey = __q_loader.uuid();
    let currentWindowRegistry = new Map()
    let ds = new DiffScopeHandle(internal)
    for(let [id, clazz] of registry.entries()) {
        currentWindowRegistry.set(id, new clazz(ds));
    }
    instanceRegistry.set(windowKey, currentWindowRegistry);
    return windowKey;
}

__q_callbacks.invoke = function (windowKey, id, index) {
    let script = instanceRegistry.get(windowKey).get(id);
    let preparedValue = undefined;
    if(script.prepare instanceof Function) {
        if(script instanceof Script) {
            preparedValue = script.prepare();
        } else if(script instanceof ScriptSet) {
            preparedValue = script.prepare(index);
        }
        if(preparedValue == undefined) return;
    }
    if(!script.ds._internal.startTransaction(id, index)) {
        throw new Error('Cannot start document transaction while running script.');
    }
    script.main(preparedValue);
    script.ds._internal.endTransaction(id, index);
}

__q_callbacks.getNameFromJs = function (id, index) {
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