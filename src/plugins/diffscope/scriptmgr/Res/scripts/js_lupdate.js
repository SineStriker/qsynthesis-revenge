"use strict";

const fs = require('fs');
const cp = require('child_process');

if(!fs.existsSync('tmp')) fs.mkdirSync('tmp')

for(let filename of fs.readdirSync('src')) {
    let fileContent = fs.readFileSync('src/' + filename).toString();
    fs.writeFileSync('tmp/' + filename, fileContent);
    fileContent = fileContent.replace(/this.tr/g, `qsTr`);
    fs.writeFileSync('src/' + filename, fileContent);
}

let langCode = process.argv[2];

cp.execSync(`lupdate src/ -ts ../translations/ScriptMgr_${langCode}.ts`);

fs.rmSync('src', {recursive: true});
fs.renameSync('tmp', 'src');