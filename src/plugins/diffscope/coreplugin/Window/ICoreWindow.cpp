#include "ICoreWindow.h"
#include "ICoreWindow_p.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include "ICore.h"
#include "Internal/Window/MainWindow.h"

#include "QsFrameworkNamespace.h"

namespace Core {

    static void removeAllAccelerateKeys(QString &text) {
        // 第一步：去掉带括号的加速键
        QRegularExpression regex1("\\(&[^)]+\\)");
        text = text.replace(regex1, "");

        // 第二步：去掉剩余的加速键符号
        text = text.replace("&", "");
    }

    ICoreWindowPrivate::ICoreWindowPrivate() {
        mainMenuCtx = nullptr;
    }

    void ICoreWindowPrivate::init() {
    }

    void ICoreWindowPrivate::reloadMenuBar() {
        Q_Q(ICoreWindow);
        auto items = q->actionItems();
        auto bar = q->menuBar();

        mainMenuCtx->buildMenuBarWithState(items, bar);
    }

    void ICoreWindowPrivate::showAllActions_helper() {
        Q_Q(ICoreWindow);
        cp->abandon();

        // Remove obsolete
        for (auto it = mostRecentActions.begin(); it != mostRecentActions.end(); ++it) {
            if (it->isNull() || !actionItemMap.value((*it)->id())) {
                mostRecentActions.erase(it);
            }
        }

        QList<ActionItem *> actionItems;
        for (const auto &ai : qAsConst(mostRecentActions)) {
            actionItems.append(ai.data());
        }
        for (const auto &ai : actionItemMap.values()) {
            if (mostRecentActions.contains(ai)) {
                continue;
            }
            actionItems.append(ai);
        }

        for (const auto &ai : qAsConst(actionItems)) {
            if (!ai->isAction()) {
                continue;
            }

            auto action = ai->action();
            if (!action->isVisible() || !action->isEnabled()) {
                continue;
            }

            auto item = new QListWidgetItem();

            QString category;
            QString text = ai->commandName();
            if (text.isEmpty()) {
                text = ai->id();
            }
            QString desc = ai->commandDescription();
            if (desc.isEmpty()) {
                desc = (ai->text());
                removeAllAccelerateKeys(desc);
            }

            // If text contains colon
            int index = text.indexOf(':');
            if (index >= 0) {
                category = text.left(index).trimmed();
                if (!category.isEmpty()) {
                    text = category + ": " + text.mid(index + 1).trimmed();
                    desc.prepend(category + ": ");
                }
            }

            QString extra;
            QKeySequence seq = action->shortcut();
            if (!seq.isEmpty()) {
                auto seqs = seq.toString(QKeySequence::PortableText).split(' ');
                QStringList seqsText;
                for (const auto &seqItem : qAsConst(seqs)) {
                    auto keys = seqItem.split('+');
                    for (auto &key : keys) {
                        key = "` " + key + " `";
                    }
                    seqsText.append(keys.join('+'));
                }
                extra = seqsText.join(' ');
            }

            item->setText(desc);
            item->setData(QsApi::SubtitleRole, text);
            item->setData(QsApi::DescriptionRole, extra);
            item->setData(QsApi::ObjectPointerRole, QVariant::fromValue(intptr_t(ai)));

            cp->addItem(item);
        }

        cp->setFilterHint(tr("Search for action"));
        cp->setCurrentRow(0);
        cp->start();

        auto obj = new QObject();
        connect(cp, &QsApi::CommandPalette::finished, obj, [obj, this](QListWidgetItem *item) {
            delete obj;
            if (!item) {
                return;
            }

            auto ai = reinterpret_cast<ActionItem *>(item->data(QsApi::ObjectPointerRole).value<intptr_t>());
            if (!ai) {
                return;
            }

            mostRecentActions.remove(ai);
            mostRecentActions.prepend(ai);

            QTimer::singleShot(0, ai->action(), &QAction::trigger);
        });

        connect(q, &IWindow::shortcutAboutToCome, obj, [this]() { cp->abandon(); });
    }

    void ICoreWindowPrivate::selectEditor_helper(QList<DocumentSpec *> &specs, const QString &path) {
        Q_Q(ICoreWindow);
        if (specs.size() == 1) {
            openEditor(specs.front(), path);
            return;
        }

        cp->abandon();

        for (const auto &spec : qAsConst(specs)) {
            auto item = new QListWidgetItem();

            auto desc = spec->description();
            auto name = spec->displayName();
            if (name.isEmpty()) {
                name = spec->id();
            }
            if (desc.isEmpty()) {
                desc = name;
            }

            item->setText(desc);
            item->setData(QsApi::SubtitleRole, name);
            item->setData(QsApi::ObjectPointerRole, QVariant::fromValue(intptr_t(spec)));

            cp->addItem(item);
        }

        cp->setFilterHint(tr("Select editor for \"%1\"").arg(QFileInfo(path).fileName()));
        cp->setCurrentRow(0);
        cp->start();

        auto obj = new QObject();
        connect(cp, &QsApi::CommandPalette::finished, obj, [obj, this, path](QListWidgetItem *item) {
            delete obj;
            if (!item) {
                return;
            }
            auto spec = reinterpret_cast<DocumentSpec *>(item->data(QsApi::ObjectPointerRole).value<intptr_t>());
            openEditor(spec, path);
        });
    }

    void ICoreWindowPrivate::openEditor(DocumentSpec *spec, const QString &path) {
        Q_Q(ICoreWindow);
        if (spec->open(path)) {
            if (qApp->property("closeHomeOnOpen").toBool() && q->id() == "home") {
                QTimer::singleShot(0, q->window(), &QWidget::close);
            }
        }
    }

    QMenuBar *ICoreWindow::menuBar() const {
        return qobject_cast<Internal::MainWindow *>(window())->menuBar();
    }

    void ICoreWindow::setMenuBar(QMenuBar *menuBar) {
        qobject_cast<Internal::MainWindow *>(window())->setMenuBar(menuBar);
    }

    QWidget *ICoreWindow::centralWidget() const {
        return qobject_cast<Internal::MainWindow *>(window())->centralWidget();
    }

    void ICoreWindow::setCentralWidget(QWidget *widget) {
        qobject_cast<Internal::MainWindow *>(window())->setCentralWidget(widget);
    }

    QStatusBar *ICoreWindow::statusBar() const {
        return qobject_cast<Internal::MainWindow *>(window())->statusBar();
    }

    void ICoreWindow::setStatusBar(QStatusBar *statusBar) {
        qobject_cast<Internal::MainWindow *>(window())->setStatusBar(statusBar);
    }

    QsApi::CommandPalette *ICoreWindow::commandPalette() const {
        Q_D(const ICoreWindow);
        return d->cp;
    }

    void ICoreWindow::openFile(const QString &path) {
        Q_D(ICoreWindow);

        QFileInfo info(path);
        if (!info.isFile()) {
            return;
        }

        auto docMgr = ICore::instance()->documentSystem();
        auto specs = docMgr->supportedDocTypes(info.completeSuffix());
        if (specs.isEmpty()) {
            QMessageBox::critical(
                window(), ICore::mainTitle(),
                tr("Can't find editor of the file %1!").arg(QDir::toNativeSeparators(info.canonicalFilePath())));
            return;
        }

        d->selectEditor_helper(specs, path);
    }

    void ICoreWindow::showAllActions() {
        Q_D(ICoreWindow);
        d->showAllActions_helper();
    }

    ICoreWindow::ICoreWindow(const QString &id, QObject *parent) : ICoreWindow(*new ICoreWindowPrivate(), id, parent) {
    }

    ICoreWindow::~ICoreWindow() {
    }

    QString ICoreWindow::correctWindowTitle(const QString &title) const {
        return IWindow::correctWindowTitle(ICore::displayTitle(title));
    }

    QWidget *ICoreWindow::createWindow(QWidget *parent) const {
        return new Internal::MainWindow(parent);
    }

    void ICoreWindow::setupWindow() {
        Q_D(ICoreWindow);

        auto win = window();
        win->setProperty("top-window", true);

        // Add window and menubar as basic shortcut contexts
        addShortcutContext(win);
        addShortcutContext(menuBar());

        d->mainMenuCtx = ICore::instance()->actionSystem()->context(QString("%1.MainMenu").arg(id()));

        // Init command palette
        d->cp = new QsApi::CommandPalette(win);
    }

    void ICoreWindow::windowAddOnsInitialized() {
        Q_D(ICoreWindow);
        connect(d->mainMenuCtx, &ActionContext::stateChanged, d, &ICoreWindowPrivate::reloadMenuBar);
        d->reloadMenuBar();
    }

    void ICoreWindow::windowAddOnsFinished() {
    }

    ICoreWindow::ICoreWindow(ICoreWindowPrivate &d, const QString &id, QObject *parent) : IWindow(d, id, parent) {
        d.init();
    }

    uint qHash(const QPointer<ActionItem> &key, uint seed) {
        return qHash(key.data(), seed);
    }

}