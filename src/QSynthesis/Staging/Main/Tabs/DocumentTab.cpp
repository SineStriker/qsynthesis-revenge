#include "DocumentTab.h"
#include "Managers/DataManager.h"
#include "SystemHelper.h"

#include "DocumentTab_p.h"

DocumentTab::DocumentTab(QWidget *parent) : DocumentTab(*new DocumentTabPrivate(), parent) {
}

DocumentTab::~DocumentTab() {
}

CentralTab::Type DocumentTab::type() const {
    return Document;
}

DocumentTab::DocumentTab(DocumentTabPrivate &d, QWidget *parent) : CentralTab(d, parent) {
    d.init();
}

bool DocumentTab::isUntitled() const {
    Q_D(const DocumentTab);
    return d->untitled;
}
bool DocumentTab::isDeleted() const {
    Q_D(const DocumentTab);
    return d->deleted;
}

void DocumentTab::setUntitled(bool untitled) {
    Q_D(DocumentTab);
    d->untitled = untitled;
}

void DocumentTab::setDeleted(bool deleted) {
    Q_D(DocumentTab);
    d->deleted = deleted;
}
