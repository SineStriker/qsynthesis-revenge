#include "IEMgrAddOn.h"

#include "CoreApi/IWindow.h"
#include "IManager.h"

#include <QMDecorator.h>

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

        qIDec->installLocale(this, {{}}, _LOC(IEMgrAddOn, this));
    }

    void IEMgrAddOn::extensionsInitialized() {
    }

    void IEMgrAddOn::reloadStrings() {
        importExportGroupItem->setText(tr("Import/Export Actions"));
        importItem->setText(tr("Import"));
        importProjectItem->setText(tr("Import Project"));
        importAudioItem->setText(tr("Import Audio"));
        exportItem->setText(tr("Export"));
        exportProjectItem->setText(tr("Export Project"));
        exportAudioItem->setText(tr("Export Audio"));
    }

    void IEMgrAddOn::initActions() {
        auto iWin = windowHandle();

        importExportGroupItem = new ActionItem("iemgr.ImportExportGroup", new QActionGroup(this), this);

        importItem = new ActionItem("iemgr.Import", new QMenu(), this);
        importProjectItem = new ActionItem("iemgr.ImportProject", new QAction(), this);
        importAudioItem = new ActionItem("iemgr.ImportAudio", new QAction(), this);

        exportItem = new ActionItem("iemgr.Export", new QAction());
        exportProjectItem = new ActionItem("iemgr.ExportProject", new QAction(), this);
        exportAudioItem = new ActionItem("iemgr.ExportAudio", new QAction(), this);

        connect(importProjectItem->action(), &QAction::triggered, this, [this]() {
            IManager::instance()->runImport({}, {}, windowHandle()->window()); //
        });

        connect(importAudioItem->action(), &QAction::triggered, this, [this]() {
            //
        });

        connect(exportProjectItem->action(), &QAction::triggered, this, [this]() {
            IManager::instance()->runExport({}, {}, windowHandle()->window()); //
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
    }
}