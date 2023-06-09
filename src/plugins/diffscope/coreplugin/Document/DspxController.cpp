#include "DspxController.h"
#include "DspxController_p.h"


#include <ItemModel/AceTreeModel.h>

namespace Core {

    DspxRootControllerPrivate::DspxRootControllerPrivate() {
        metadata = workspace = nullptr;
    }

    DspxRootControllerPrivate::~DspxRootControllerPrivate() {
    }

    void DspxRootControllerPrivate::init() {
        Q_Q(DspxRootController);
    }

    void DspxRootControllerPrivate::_q_propertyChanged(QsApi::AceTreeItem *item, const QString &key,
                                                       const QVariant &oldValue, const QVariant &newValue) {
        Q_Q(DspxRootController);

        Q_UNUSED(oldValue)
        if (item == metadata) {
            if (key == "version") {
                emit q->projectVersionChanged(newValue.toString());
            } else if (key == "author") {
                emit q->projectAuthorChanged(newValue.toString());
            } else if (key == "name") {
                emit q->projectNameChanged(newValue.toString());
            }
        }
    }

    DspxRootController::DspxRootController(QObject *parent)
        : DspxRootController(*new DspxRootControllerPrivate(), parent) {
    }

    DspxRootController::~DspxRootController() {
    }

    QString DspxRootController::projectVersion() const {
        Q_D(const DspxRootController);
        return d->metadata->property("version").toString();
    }

    void DspxRootController::setProjectVersion(const QString &projectVersion) {
        Q_D(DspxRootController);
        d->metadata->setProperty("version", projectVersion);
    }

    QString DspxRootController::projectAuthor() const {
        Q_D(const DspxRootController);
        return d->metadata->property("author").toString();
    }

    void DspxRootController::setProjectAuthor(const QString &projectAuthor) {
        Q_D(DspxRootController);
        d->metadata->setProperty("author", projectAuthor);
    }

    QString DspxRootController::projectName() const {
        Q_D(const DspxRootController);
        return d->metadata->property("name").toString();
    }

    void DspxRootController::setProjectName(const QString &projectName) {
        Q_D(DspxRootController);
        d->metadata->setProperty("name", projectName);
    }

    void DspxRootController::setupImpl(QsApi::AceTreeItem *item) {
        Q_D(DspxRootController);

        d->metadata = item->nameToNode("metadata");
        d->workspace = item->nameToNode("workspace");

        AceTreeController::setupImpl(item);

        connect(item->model(), &QsApi::AceTreeModel::propertyChanged, d,
                &DspxRootControllerPrivate::_q_propertyChanged);
    }

    void DspxRootController::cleanImpl() {
        Q_D(DspxRootController);

        auto item = d->treeItem;
        disconnect(item->model(), &QsApi::AceTreeModel::propertyChanged, d,
                   &DspxRootControllerPrivate::_q_propertyChanged);

        AceTreeController::cleanImpl();

        d->metadata = nullptr;
        d->workspace = nullptr;
    }

    DspxRootController::DspxRootController(DspxRootControllerPrivate &d, QObject *parent)
        : QsApi::AceTreeController(d, "root", parent) {
        d.init();
    }

}