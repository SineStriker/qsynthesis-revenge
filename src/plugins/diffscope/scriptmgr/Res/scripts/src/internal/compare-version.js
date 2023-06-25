/**
 * Compare [semver](https://semver.org/) version strings to find greater, equal or lesser.
 * This library supports the full semver specification, including comparing versions with different number of digits like `1.0.0`, `1.0`, `1`, and pre-release versions like `1.0.0-alpha`.
 * @param v1 - First version to compare
 * @param v2 - Second version to compare
 * @returns Numeric value compatible with the [Array.sort(fn) interface](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/sort#Parameters).
 */
export var compareVersions = function (v1, v2) {
    // validate input and split into segments
    var n1 = validateAndParse(v1);
    var n2 = validateAndParse(v2);
    // pop off the patch
    var p1 = n1.pop();
    var p2 = n2.pop();
    // validate numbers
    var r = compareSegments(n1, n2);
    if (r !== 0)
        return r;
    // validate pre-release
    if (p1 && p2) {
        return compareSegments(p1.split('.'), p2.split('.'));
    }
    else if (p1 || p2) {
        return p1 ? -1 : 1;
    }
    return 0;
};
/**
 * Validate [semver](https://semver.org/) version strings.
 *
 * @param version Version number to validate
 * @returns `true` if the version number is a valid semver version number, `false` otherwise.
 *
 * @example
 * ```
 * validate('1.0.0-rc.1'); // return true
 * validate('1.0-rc.1'); // return false
 * validate('foo'); // return false
 * ```
 */
export var validate = function (version) {
    return typeof version === 'string' && /^[v\d]/.test(version) && semver.test(version);
};
/**
 * Compare [semver](https://semver.org/) version strings using the specified operator.
 *
 * @param v1 First version to compare
 * @param v2 Second version to compare
 * @param operator Allowed arithmetic operator to use
 * @returns `true` if the comparison between the firstVersion and the secondVersion satisfies the operator, `false` otherwise.
 *
 * @example
 * ```
 * compare('10.1.8', '10.0.4', '>'); // return true
 * compare('10.0.1', '10.0.1', '='); // return true
 * compare('10.1.1', '10.2.2', '<'); // return true
 * compare('10.1.1', '10.2.2', '<='); // return true
 * compare('10.1.1', '10.2.2', '>='); // return false
 * ```
 */
export var compare = function (v1, v2, operator) {
    // validate input operator
    assertValidOperator(operator);
    // since result of compareVersions can only be -1 or 0 or 1
    // a simple map can be used to replace switch
    var res = compareVersions(v1, v2);
    return operatorResMap[operator].includes(res);
};
/**
 * Match [npm semver](https://docs.npmjs.com/cli/v6/using-npm/semver) version range.
 *
 * @param version Version number to match
 * @param range Range pattern for version
 * @returns `true` if the version number is within the range, `false` otherwise.
 *
 * @example
 * ```
 * satisfies('1.1.0', '^1.0.0'); // return true
 * satisfies('1.1.0', '~1.0.0'); // return false
 * ```
 */
export var satisfies = function (version, range) {
    // handle multiple comparators
    if (range.includes('||')) {
        return range.split('||').some(function (r) { return satisfies(version, r); });
    }
    else if (range.includes(' ')) {
        return range
            .trim()
            .replace(/\s{2,}/g, ' ')
            .split(' ')
            .every(function (r) { return satisfies(version, r); });
    }
    // if no range operator then "="
    var m = range.match(/^([<>=~^]+)/);
    var op = m ? m[1] : '=';
    // if gt/lt/eq then operator compare
    if (op !== '^' && op !== '~')
        return compare(version, range, op);
    // else range of either "~" or "^" is assumed
    var _a = validateAndParse(version), v1 = _a[0], v2 = _a[1], v3 = _a[2], vp = _a[4];
    var _b = validateAndParse(range), r1 = _b[0], r2 = _b[1], r3 = _b[2], rp = _b[4];
    var v = [v1, v2, v3];
    var r = [r1, r2 !== null && r2 !== void 0 ? r2 : 'x', r3 !== null && r3 !== void 0 ? r3 : 'x'];
    // validate pre-release
    if (rp) {
        if (!vp)
            return false;
        if (compareSegments(v, r) !== 0)
            return false;
        if (compareSegments(vp.split('.'), rp.split('.')) === -1)
            return false;
    }
    // first non-zero number
    var nonZero = r.findIndex(function (v) { return v !== '0'; }) + 1;
    // pointer to where segments can be >=
    var i = op === '~' ? 2 : nonZero > 1 ? nonZero : 1;
    // before pointer must be equal
    if (compareSegments(v.slice(0, i), r.slice(0, i)) !== 0)
        return false;
    // after pointer must be >=
    if (compareSegments(v.slice(i), r.slice(i)) === -1)
        return false;
    return true;
};
var semver = /^[v^~<>=]*?(\d+)(?:\.([x*]|\d+)(?:\.([x*]|\d+)(?:\.([x*]|\d+))?(?:-([\da-z\-]+(?:\.[\da-z\-]+)*))?(?:\+[\da-z\-]+(?:\.[\da-z\-]+)*)?)?)?$/i;
var validateAndParse = function (version) {
    if (typeof version !== 'string') {
        throw new TypeError('Invalid argument expected string');
    }
    var match = version.match(semver);
    if (!match) {
        throw new Error("Invalid argument not valid semver ('".concat(version, "' received)"));
    }
    match.shift();
    return match;
};
var isWildcard = function (s) { return s === '*' || s === 'x' || s === 'X'; };
var tryParse = function (v) {
    var n = parseInt(v, 10);
    return isNaN(n) ? v : n;
};
var forceType = function (a, b) {
    return typeof a !== typeof b ? [String(a), String(b)] : [a, b];
};
var compareStrings = function (a, b) {
    if (isWildcard(a) || isWildcard(b))
        return 0;
    var _a = forceType(tryParse(a), tryParse(b)), ap = _a[0], bp = _a[1];
    if (ap > bp)
        return 1;
    if (ap < bp)
        return -1;
    return 0;
};
var compareSegments = function (a, b) {
    for (var i = 0; i < Math.max(a.length, b.length); i++) {
        var r = compareStrings(a[i] || '0', b[i] || '0');
        if (r !== 0)
            return r;
    }
    return 0;
};
var operatorResMap = {
    '>': [1],
    '>=': [0, 1],
    '=': [0],
    '<=': [-1, 0],
    '<': [-1],
    '!=': [-1, 1],
};
var allowedOperators = Object.keys(operatorResMap);
var assertValidOperator = function (op) {
    if (typeof op !== 'string') {
        throw new TypeError("Invalid operator type, expected string but got ".concat(typeof op));
    }
    if (allowedOperators.indexOf(op) === -1) {
        throw new Error("Invalid operator, expected one of ".concat(allowedOperators.join('|')));
    }
};
