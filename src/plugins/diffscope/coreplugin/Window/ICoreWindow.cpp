#include "ICoreWindow.h"
#include "ICoreWindow_p.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QMessageBox>

#include <QMDecoratorV2.h>
#include <QMMath.h>

#include <CoreApi/ILoader.h>

#include "ICore.h"
#include "Internal/Window/MainWindow.h"

#include "QsFrameworkNamespace.h"

namespace Core {

    static const char settingCatalogC[] = "ICoreWindow/RecentActions";

    static const char americanEnglishLocale[] = "en_US";

    using MOST_RECENT_ACTIONS = QMChronSet<QString>;
    Q_GLOBAL_STATIC(MOST_RECENT_ACTIONS, mostRecentActionsGlobal);

    static QString removeAllAccelerateKeys(QString text) {
        // 第一步：去掉带括号的加速键
        QRegularExpression regex1("\\(&[^)]+\\)");
        text = text.replace(regex1, QString());

        // 第二步：去掉剩余的加速键符号
        text = text.replace("&", QString());
        return text;
    }

    ICoreWindowPrivate::ICoreWindowPrivate() {
        cp = nullptr;
        mainMenuCtx = nullptr;

        auto actionMgr = ICore::instance()->actionSystem();
        if (!mostRecentActionsGlobal.exists()) {
            // Read settings
            auto settings = ILoader::instance()->settings();
            auto arr = settings->value(settingCatalogC).toArray();

            for (const auto &item : qAsConst(arr)) {
                if (!item.isString()) {
                    continue;
                }

                QString id = item.toString();
                if (!actionMgr->action(id)) {
                    continue;
                }
                mostRecentActionsGlobal->append(id);
            }
        }
    }

    ICoreWindowPrivate::~ICoreWindowPrivate() {
        // Save settings
        auto settings = ILoader::instance()->settings();

        QJsonArray arr;
        for (const auto &item : qAsConst(*mostRecentActionsGlobal)) {
            arr.append(item);
        }

        settings->insert(settingCatalogC, arr);
    }

    void ICoreWindowPrivate::init() {
    }

    void ICoreWindowPrivate::reloadMenuBar() {
        Q_Q(ICoreWindow);
        auto items = q->actionItems();
        auto bar = q->menuBar();

        mainMenuCtx->buildMenuBarWithState(items, bar);
    }

    static QString keySequenceToRichText(const QKeySequence &seq) {
        if (!seq.isEmpty()) {
            auto seqs = seq.toString(QKeySequence::PortableText).split(", ");
            QStringList seqsText;
            for (const auto &seqItem : qAsConst(seqs)) {
                auto keys = seqItem.split('+');
                for (auto &key : keys) {
                    key = "<quote> " + key + " </quote>";
                }
                seqsText.append(keys.join('+'));
            }
            return "<x0.8>" + seqsText.join("  ") + "</x0.8>";
        }
        return {};
    }

    void ICoreWindowPrivate::showAllActions_helper() {
        Q_Q(ICoreWindow);
        cp->abandon();

        // Remove obsolete
        QList<ActionItem *> actionItems;

        auto &mostRecentActions = *mostRecentActionsGlobal;
        for (const auto &item : qAsConst(mostRecentActions)) {
            auto ai = actionItemMap.value(item);
            if (!ai) {
                continue;
            }
            actionItems.append(ai);
        }

        bool recent = !actionItems.isEmpty();
        for (const auto &ai : actionItemMap.values()) {
            if (mostRecentActions.contains(ai->id())) {
                continue;
            }
            actionItems.append(ai);
        }

        bool isEn = !qIDec->locale().compare(americanEnglishLocale, Qt::CaseInsensitive);
        for (const auto &ai : qAsConst(actionItems)) {
            if (!ai->isAction() || ai->property("no-command-palette").toBool()) {
                continue;
            }

            auto action = ai->action();
            if (!action->isEnabled()) {
                continue;
            }

            auto item = new QListWidgetItem();

            QString desc = ai->commandDisplayName();
            if (desc.isEmpty()) {
                desc = removeAllAccelerateKeys(ai->text());
            }

            QString category;
            QString text = ai->commandName();
            if (text.isEmpty()) {
                text = ai->id();
            }

            // If text contains colon
            int index = text.indexOf(':');
            if (index >= 0) {
                category = text.left(index).trimmed();
                if (!category.isEmpty()) {
                    text = category + ": " + text.mid(index + 1).trimmed();
                    desc.prepend(QApplication::translate("Core::CommandCategory", category.toLatin1()) + ": ");
                }
            }

            if (action->isCheckable()) {
                QString checkedDesc = action->isChecked() ? ai->commandCheckedDescription().second
                                                          : ai->commandCheckedDescription().first;
                if (checkedDesc.isEmpty()) {
                    checkedDesc = action->isChecked() ? tr("Off") : tr("On");
                }
                (isEn ? text : desc).append(" (" + checkedDesc + ")");
            }

            QString extra = keySequenceToRichText(action->shortcut());

            if (recent) {
                recent = false;
                if (!extra.isEmpty())
                    extra += " ";
                extra += QString("<highlight>%1</highlight>").arg(tr("Recently Used"));
            }

            item->setText(desc);
            if (isEn) {
                item->setText(text);
            } else {
                item->setText(desc);
                item->setData(QsApi::SubtitleRole, text);
            }
            item->setData(QsApi::DescriptionRole, extra);
            item->setData(QsApi::ObjectPointerRole, QVariant::fromValue(intptr_t(ai)));
            item->setData(QsApi::AlignmentRole, int(Qt::AlignTop));

            cp->addItem(item);
        }

        cp->setFilterHint(tr("Search for action"));
        cp->setCurrentRow(0);
        cp->start();

        auto obj = new QObject();
        connect(cp, &QsApi::CommandPalette::finished, obj, [obj, q](QListWidgetItem *item) {
            delete obj;
            if (!item) {
                return;
            }

            auto ai = reinterpret_cast<ActionItem *>(item->data(QsApi::ObjectPointerRole).value<intptr_t>());
            if (!ai) {
                return;
            }

            auto &mostRecentActions = *mostRecentActionsGlobal;
            mostRecentActions.remove(ai->id());
            mostRecentActions.prepend(ai->id());

            QTimer::singleShot(0, ai->action(), &QAction::trigger);
        });
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

    void ICoreWindowPrivate::selectColorThemes_helper() {
        Q_Q(ICoreWindow);
        cp->abandon();

        auto themes = qIDec->themes();
        std::sort(themes.begin(), themes.end());

        QListWidgetItem *curItem = nullptr;
        for (const auto &theme : qAsConst(themes)) {
            auto item = new QListWidgetItem();
            item->setText(theme);
            cp->addItem(item);

            if (theme == qIDec->theme()) {
                curItem = item;
            }
        }

        cp->setFilterHint(tr("Select color theme (Press Up/Down to preview)"));
        cp->setCurrentItem(curItem);
        cp->start();

        QString orgTheme = qIDec->theme();

        auto obj = new QObject();
        connect(cp, &QsApi::CommandPalette::currentItemChanged, obj, [](QListWidgetItem *item) {
            if (!item) {
                return;
            }
            QString theme = item->text();
            qIDec->setTheme(theme);
        });

        connect(cp, &QsApi::CommandPalette::finished, obj, [obj, orgTheme](QListWidgetItem *item) {
            delete obj;
            if (!item) {
                qIDec->setTheme(orgTheme);
                return;
            }
            QString theme = item->text();
            qIDec->setTheme(theme);

            ILoader::instance()->settings()->insert("Theme", theme);
        });
    }

    void ICoreWindowPrivate::selectTranslations_helper() {
        Q_Q(ICoreWindow);
        cp->abandon();

        auto locales = qIDec->locales();
        if (!locales.contains(americanEnglishLocale)) {
            locales.append(americanEnglishLocale);
        }
        std::sort(locales.begin(), locales.end());

        QListWidgetItem *curItem = nullptr;
        for (const auto &loc : qAsConst(locales)) {
            auto item = new QListWidgetItem();

            QLocale locale(loc);

            QString languageName = locale.nativeLanguageName();
            QString countryName = locale.nativeCountryName();
            QString text = (languageName.isEmpty() || countryName.isEmpty())
                               ? loc
                               : QString("%1 (%2)").arg(languageName, countryName);

            item->setText(text);
            item->setData(QsApi::DescriptionRole, QString("<x0.9>%1</x0.9>").arg(loc));
            item->setData(Qt::UserRole + 1, loc);
            cp->addItem(item);

            if (loc == qIDec->locale()) {
                curItem = item;
            }
        }

        cp->setFilterHint(tr("Select locale and language"));
        cp->setCurrentItem(curItem);
        cp->start();

        QString orgLoc = qIDec->locale();

        auto obj = new QObject();
        connect(cp, &QsApi::CommandPalette::finished, obj, [obj, orgLoc](QListWidgetItem *item) {
            delete obj;
            if (!item) {
                return;
            }
            QString loc = item->data(Qt::UserRole + 1).toString();
            qIDec->setLocale(loc);

            ILoader::instance()->settings()->insert("Translation", loc);
        });
    }

    void ICoreWindowPrivate::selectRecentFiles_helper(bool dirsAtTop) {
        cp->abandon();

        auto docMgr = ICore::instance()->documentSystem();

        auto addFiles = [&]() {
            for (const auto &file : docMgr->recentFiles()) {
            }
        };

        auto addDirs = [&]() {

        };
    }

    void ICoreWindowPrivate::showMenuInPalette_helper(QMenu *menu, QMenu *menuToDelete) {
        cp->abandon();

        QListWidgetItem *lastItem = nullptr;
        for (const auto &action : menu->actions()) {
            QString desc;
            if (action->isSeparator()) {
                if (lastItem) {
                    lastItem->setData(QsApi::SeparatorRole, true);
                }
                continue;
            } else if (action->menu()) {
                desc = tr("Menu");
            } else {
                desc = keySequenceToRichText(action->shortcut());
            }

            auto item = new QListWidgetItem();
            item->setText(removeAllAccelerateKeys(action->text()));
            item->setData(QsApi::DescriptionRole, desc);
            item->setData(QsApi::ObjectPointerRole, QVariant::fromValue(intptr_t(action)));

            cp->addItem(item);
            lastItem = item;
        }

        if (!lastItem) {
            return;
        }

        cp->setFilterHint(tr("Select action in \"%1\"").arg(removeAllAccelerateKeys(menu->title())));
        cp->setCurrentRow(0);
        cp->start();

        auto obj = new QObject();
        connect(cp, &QsApi::CommandPalette::finished, obj, [obj, menuToDelete, this](QListWidgetItem *item) {
            bool handled = true;

            {
                delete obj;
                if (!item) {
                    handled = false;
                    goto out;
                }

                auto action = reinterpret_cast<QAction *>(item->data(QsApi::ObjectPointerRole).value<intptr_t>());
                if (!action) {
                    handled = false;
                    goto out;
                }

                if (action->menu()) {
                    QTimer::singleShot(0, action, [action, menuToDelete, this]() {
                        showMenuInPalette_helper(action->menu(), menuToDelete);
                    });
                } else {
                    QTimer::singleShot(0, action, [action, menuToDelete]() {
                        action->trigger();
                        delete menuToDelete;
                    });
                }
            }

        out:
            if (!handled)
                delete menuToDelete;
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

    bool ICoreWindowPrivate::eventFilter(QObject *obj, QEvent *event) {
        Q_Q(ICoreWindow);
        auto win = q->window();
        if (obj == win) {
            switch (event->type()) {
                case QEvent::WindowDeactivate:
                    cp->abandon();
                    break;
                default:
                    break;
            }
        }
        return QObject::eventFilter(obj, event);
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

    void ICoreWindow::openDirectory(const QString &path) {
        // TODO: add open directory support
    }

    void ICoreWindow::showAllActions() {
        Q_D(ICoreWindow);
        d->showAllActions_helper();
    }

    void ICoreWindow::selectColorThemes() {
        Q_D(ICoreWindow);
        d->selectColorThemes_helper();
    }

    void ICoreWindow::selectTranslations() {
        Q_D(ICoreWindow);
        d->selectTranslations_helper();
    }

    void ICoreWindow::selectRecentFiles(bool dirsAtTop) {
        Q_D(ICoreWindow);
        d->selectRecentFiles_helper(dirsAtTop);
    }

    void ICoreWindow::showMenuInPalette(QMenu *menu, bool deleteLater) {
        Q_D(ICoreWindow);
        if (!menu) {
            return;
        }

        d->showMenuInPalette_helper(menu, deleteLater ? menu : nullptr);
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

        win->installEventFilter(d);
    }

    void ICoreWindow::windowAddOnsInitialized() {
        Q_D(ICoreWindow);
        connect(d->mainMenuCtx, &ActionContext::stateChanged, d, &ICoreWindowPrivate::reloadMenuBar);
        d->reloadMenuBar();

        // Shortcut will cause immediate close of command palette
        connect(this, &IWindow::shortcutAboutToCome, d->cp, &QsApi::CommandPalette::abandon);
    }

    void ICoreWindow::windowAddOnsFinished() {
    }

    ICoreWindow::ICoreWindow(ICoreWindowPrivate &d, const QString &id, QObject *parent) : IWindow(d, id, parent) {
        d.init();
    }

}