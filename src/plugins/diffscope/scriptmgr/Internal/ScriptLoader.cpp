//
// Created by Crs_1 on 2023/4/17.
//

#include "ScriptLoader.h"

#include <utility>

namespace ScriptMgr::Internal {

    static ScriptLoader *m_instance = nullptr;

    ScriptLoader *ScriptLoader::instance() {
        return m_instance;
    }

    void ScriptLoader::registerAddon(ScriptMgrAddOn *addon) {
    }

    void ScriptLoader::reloadScripts() {
    }

    ScriptLoader::ScriptLoader(QObject *parent) : QObject(parent) {
        m_instance = this;
    }

    ScriptLoader::~ScriptLoader() {
        m_instance = nullptr;
    }

    QString ScriptLoader::userScriptDir() {
        return {};
    }
}