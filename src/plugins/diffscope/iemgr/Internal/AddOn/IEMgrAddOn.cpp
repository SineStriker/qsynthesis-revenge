#include "IEMgrAddOn.h"

#include "CoreApi/IWindow.h"
#include "IManager.h"

#include <QMDecoratorV2.h>

#include <QDebug>
#include <QMetaObject>

#include <coreplugin/ICore.h>
#include <coreplugin/Interfaces/IButtonBar.h>

namespace IEMgr::Internal {

    using namespace Core;

    bool IEMgrAddOnFactory::predicate(Core::IWindow *handle) const {
        return handle->id() == "home" || handle->id() == "project";
    }

    Core::IWindowAddOn *IEMgrAddOnFactory::create(QObject *parent) {
        return new IEMgrAddOn(parent);
    }

    IEMgrAddOn::IEMgrAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }

    IEMgrAddOn::~IEMgrAddOn() {
    }

    void IEMgrAddOn::initialize() {
        auto iWin = windowHandle();

        initActions();
        initImportButton();

        qIDec->installLocale(this, _LOC(IEMgrAddOn, this));
    }

    void IEMgrAddOn::extensionsInitialized() {
    }

    void IEMgrAddOn::reloadStrings() {
        importExportGroupItem->setText(tr("Import/Export Actions"));

        importItem->setText(tr("Import"));
        importProjectItem->setText(tr("Import &Project..."));
        importAudioItem->setText(tr("Import &Audio..."));

        exportItem->setText(tr("Export"));
        exportProjectItem->setText(tr("Export &Project..."));
        exportAudioItem->setText(tr("Export &Audio..."));

        if (importButton) {
            importButton->setText(tr("Import"));
        }
    }

    void IEMgrAddOn::initActions() {
        auto iWin = windowHandle();
        auto win = iWin->window();

        importExportGroupItem = new ActionItem("iemgr.ImportExportGroup", new QActionGroup(this), this);

        importItem = new ActionItem("iemgr.Import", ICore::createCoreMenu(win), this);
        importProjectItem = new ActionItem("iemgr.ImportProject", new QAction(this), this);
        importAudioItem = new ActionItem("iemgr.ImportAudio", new QAction(this), this);

        exportItem = new ActionItem("iemgr.Export", ICore::createCoreMenu(win), this);
        exportProjectItem = new ActionItem("iemgr.ExportProject", new QAction(this), this);
        exportAudioItem = new ActionItem("iemgr.ExportAudio", new QAction(this), this);

        connect(importProjectItem->action(), &QAction::triggered, this, [iWin]() {
            IManager::instance()->runImport(iWin->window(), {}, {}); //
        });

        connect(importAudioItem->action(), &QAction::triggered, this, [iWin]() {
            //
        });

        connect(exportProjectItem->action(), &QAction::triggered, this, [iWin]() {
            IManager::instance()->runExport(iWin->window(), {}, {}); //
        });

        connect(exportAudioItem->action(), &QAction::triggered, this, [this]() {
            //
        });

        iWin->addActionItems({
            importExportGroupItem,
            importItem,
            importProjectItem,
            importAudioItem,
            exportItem,
            exportProjectItem,
            exportAudioItem,
        });

        if (iWin->id() == "home") {
            exportProjectItem->setEnabled(false);
            exportAudioItem->setEnabled(false);
        }
    }

    void IEMgrAddOn::initImportButton() {
        // Test add button
        importButton = nullptr;

        bool usingReflect = false;
        if (usingReflect) {
            // Get widget
            auto buttonBar = windowHandle()->widget("core.recentWidget.buttonBar");
            if (!buttonBar) {
                return;
            }

            QAbstractButton *button;

            // Invoke method
            bool res = QMetaObject::invokeMethod(buttonBar, "addButton", Qt::DirectConnection,
                                                 Q_RETURN_ARG(QAbstractButton *, button), // Ret
                                                 Q_ARG(QString, "import-button")          // Arg
            );
            if (!res) {
                return;
            }

            // Subsequent settings
            button->setObjectName("import-button");
            connect(button, &QAbstractButton::clicked, this, &IEMgrAddOn::_q_importButtonClicked);
            importButton = button;
        } else {
            // Get object
            auto buttonBar =
                dynamic_cast<IButtonBar *>(windowHandle()->getFirstObject("core.recentWidget.buttonBarInterface"));
            if (!buttonBar) {
                return;
            }

            QAbstractButton *button = buttonBar->addButton("import-button");

            // Subsequent settings
            button->setObjectName("import-button");
            connect(button, &QAbstractButton::clicked, this, &IEMgrAddOn::_q_importButtonClicked);
            importButton = button;
        }

        qIDec->installTheme(importButton, "iemgr.ImportButton");
    }

    void IEMgrAddOn::_q_importButtonClicked() {
        importProjectItem->action()->trigger();
    }
}