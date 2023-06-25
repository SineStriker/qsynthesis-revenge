import { satisfies } from './compare-version.js';

import { Script, ScriptSet } from './script_definitions.js';

export var registry = new Map();

function validateClass(clazz) {
    let err = new TypeError('Invalid script class.');
    if(!(clazz.prototype instanceof Script || clazz.prototype instanceof ScriptSet)) throw err;
    let info = clazz.info();
    if(typeof info !== 'object') throw err;
    if(typeof info.id !== 'string') throw err;
    if(typeof info.name !== 'string') throw err;
    let scriptMgrVersion = __q_internal.version();
    if(typeof info.requiredVersion !== 'string' || !satisfies(scriptMgrVersion, info.requiredVersion)) {
        if(__q_internal.msgBox(
            $tr('JsBuiltIn', 'Version Mismatch'),
            $tr('JsBuiltIn', "The version of DiffScope Script Manager (%1) does not satisfy the requirement in script '%2' (%3).\n\nContinue loading anyway?")
                .replace('%1', scriptMgrVersion)
                .replace('%2', info.id)
                .replace('%3', info.requiredVersion),
            'Warning',
            ['Yes', 'No'],
            'No'
        ) == 'No') {
            throw new Error(`The version of DiffScope Script Manager (${scriptMgrVersion}) does not satisfy the requirement in script '${info.id}' (${info.requiredVersion}).`);
        }
    }
    if(clazz.prototype instanceof ScriptSet && !(info.children instanceof Array)) throw err;
}



export function register(clazz) {
    let info = clazz.info();
    validateClass(clazz);
    if(registry.has(info.id)) return;
    registry.set(info.id, clazz);
    if(clazz.prototype instanceof Script) {
        __q_loader.registerScript(info.id, [].concat(info.role), info.shortcut ?? "");
    } else if(clazz.prototype instanceof ScriptSet) {
        __q_loader.registerScriptSet(info.id, info.children.map(c => c.id), [].concat(info.role), info.children.map(c => c.shortcut ?? ""));
    }
}