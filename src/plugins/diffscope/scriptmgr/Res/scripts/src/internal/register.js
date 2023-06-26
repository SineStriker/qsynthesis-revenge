import { satisfies } from './compare-version.js';

import { Script, ScriptSet } from './script_definitions.js';

export var registry = new Map();

function validateClass(clazz) {
    if(!(clazz.prototype instanceof Script || clazz.prototype instanceof ScriptSet)) throw new TypeError('Invalid script class.');
    let info = clazz.info();
    if(typeof info !== 'object') throw new TypeError('Invalid script info.');
    if(typeof info.id !== 'string') throw new TypeError(`'id' in script info should be string.`);
    if(!/^[0-9_A-Za-z]+$/.test(info.id)) throw new TypeError(`Invalid script id '${info.id}'. Script id should not be empty, and only contain letters, digits and underscores.`);
    if(typeof info.name !== 'string') throw new TypeError(`'name' in script info should be string.`);
    let scriptMgrVersion = __q_loader.version();
    if(typeof info.requiredVersion !== 'string' || !satisfies(scriptMgrVersion, info.requiredVersion)) {
        if(__q_loader.coreMsgBox(
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
    if(clazz.prototype instanceof ScriptSet) {
        if(!(info.children instanceof Array)) throw new TypeError(`Script set should have valid children.`);
        for(let child of info.children) {
            if(typeof child.id !== 'string') throw new TypeError(`'id' in script info should be string.`);
            if(!/^[0-9_A-Za-z]+$/.test(child.id)) throw new TypeError(`Invalid script id '${info.id}'. Script id should not be empty, and only contain letters, digits and underscores.`);
            if(typeof child.name !== 'string') throw new TypeError(`'name' in script info should be string.`);
        }
    }
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