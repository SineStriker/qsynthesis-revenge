//
// Created by Crs_1 on 2023/7/2.
//

#include "VstClientAddOn.h"

#include <CoreApi/IWindow.h>
#include <ICore.h>

#include "VstBridge.h"
#include "Window/IProjectWindow.h"

namespace Vst::Internal {

    VstClientAddOn::VstClientAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }

    VstClientAddOn::~VstClientAddOn() {
        if(VstBridge::instance()->m_clientAddOn == this) {
            VstBridge::instance()->m_clientAddOn = nullptr;
        }
    }

    void VstClientAddOn::initialize() {
        auto iWin = windowHandle()->cast<Core::IProjectWindow>();
        if(!iWin->doc()->isVST()) return;
        if(!VstBridge::instance()->m_clientAddOn) {
            VstBridge::instance()->m_clientAddOn = this;
        }
    }

    void VstClientAddOn::extensionsInitialized() {
    }

    bool VstClientAddOn::delayedInitialize() {
        return IWindowAddOn::delayedInitialize();
    }

    void VstClientAddOn::showWindow() const {
        auto iWin = windowHandle()->cast<Core::IProjectWindow>();
        iWin->window()->show();
        emit iWin->doc()->raiseRequested();
    }

    void VstClientAddOn::hideWindow() const {
        auto win = windowHandle()->window();
        win->hide();
    }

    bool VstClientAddOn::isWindowHidden() const {
        auto win = windowHandle()->window();
        return win->isHidden();
    }
} // Internal
