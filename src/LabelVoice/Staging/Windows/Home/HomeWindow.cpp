#include "HomeWindow.h"
#include "private/HomeWindow_p.h"

#include "QPixelSize.h"
#include "QSvgUri.h"

#include "Kernel/LvApplication.h"
#include "Managers/FileManager.h"
#include "Managers/PluginManager.h"
#include "Managers/WindowManager.h"

#include "HomeMainWidget.h"

#include <QDebug>

HomeWindow::HomeWindow(QWidget *parent) : HomeWindow(*new HomeWindowPrivate(), parent) {
}

HomeWindow::~HomeWindow() {
}

void HomeWindow::reloadStrings() {
    Q_D(HomeWindow);
    d->reloadStrings_helper();
}

QTypeList HomeWindow::templateStyleData() const {
    Q_D(const HomeWindow);
    return d->mainWidget->templateStyleData();
}

void HomeWindow::setTemplateStyleData(const QTypeList &list) {
    Q_D(HomeWindow);

    d->mainWidget->setTemplateStyleData(list);
    emit styleDataChanged();
}

QTypeList HomeWindow::recentStyleData() const {
    Q_D(const HomeWindow);
    return d->mainWidget->recentStyleData();
}

void HomeWindow::setRecentStyleData(const QTypeList &list) {
    Q_D(HomeWindow);

    d->mainWidget->setRecentStyleData(list);
    emit styleDataChanged();
}

HomeWindow::HomeWindow(HomeWindowPrivate &d, QWidget *parent)
    : PlainWindow(d, parent), ProjectCommonBlock(this) {
    d.init();

    Q_TR_NOTIFY(HomeWindow)
}

void HomeWindow::_q_browseRequested() {
    openProject();
}

void HomeWindow::_q_newRequested(int type) {
    Q_D(HomeWindow);
    switch (type) {
        case HomeMainWidget::Empty:
            d->cb_switchIn();
            break;
        case HomeMainWidget::Opencpop:
            break;
        case HomeMainWidget::DiffSinger:
            break;
        case HomeMainWidget::OpenVPI:
            break;
        default:
            break;
    }
}

void HomeWindow::_q_openRequested(const QString &filename) {
    openProject(filename);
}

void HomeWindow::_q_cancelProjectConfigure() {
    Q_D(HomeWindow);
    d->cb_switchOut();
}

void HomeWindow::_q_confirmProjectConfigure(const QString &filename) {
    openProject(filename);
}
