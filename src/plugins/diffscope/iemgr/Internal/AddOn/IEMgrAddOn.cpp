#include "IEMgrAddOn.h"

#include "CoreApi/IWindow.h"
#include "IManager.h"

#include <QMDecorator.h>

#include <QDebug>
#include <QMetaObject>

// #include <extensionsystem/invoker.h>

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

        // Test add button
        {
            importButton = nullptr;

            auto buttonsLayout = iWin->widget("core.recentWidget.buttonsLayout");
            if (buttonsLayout) {
                QAbstractButton *button;

                bool res = QMetaObject::invokeMethod(buttonsLayout, "addButton", Qt::DirectConnection,
                                                     Q_RETURN_ARG(QAbstractButton *, button), // Ret
                                                     Q_ARG(QString, "import-button")          // Arg
                );

                if (res) {
                    button->setObjectName("import-button");
                    connect(button, &QAbstractButton::clicked, this, &IEMgrAddOn::_q_importButtonClicked);
                    importButton = button;

                    qIDec->installTheme(importButton, {"IEMgr_AddOns"});
                }
            }
        }

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

        if (importButton) {
            importButton->setText(tr("Import"));
        }
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
            IWizardContext ctx(windowHandle());
            IManager::instance()->runImport(&ctx); //
        });

        connect(importAudioItem->action(), &QAction::triggered, this, [this]() {
            //
        });

        connect(exportProjectItem->action(), &QAction::triggered, this, [this]() {
            IWizardContext ctx(windowHandle());
            IManager::instance()->runExport(&ctx); //
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

    void IEMgrAddOn::_q_importButtonClicked() {
        importProjectItem->action()->trigger();
    }
}