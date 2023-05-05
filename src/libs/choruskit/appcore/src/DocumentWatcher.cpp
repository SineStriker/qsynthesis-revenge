#include "DocumentWatcher.h"
#include "DocumentWatcher_p.h"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>

#include <QMSystem.h>

namespace Core {

    DocumentWatcherPrivate::DocumentWatcherPrivate()
        : QObject(nullptr), m_fileWatcher(nullptr), m_linkWatcher(nullptr), m_blockActivated(false),
          m_blockedIDocument(nullptr) {
        reloadSetting = DocumentWatcher::AlwaysAsk;
        promptHandler = nullptr;
    }

    DocumentWatcherPrivate::~DocumentWatcherPrivate() {
    }

    void DocumentWatcherPrivate::init() {
        qApp->installEventFilter(this);
    }

    QFileSystemWatcher *DocumentWatcherPrivate::fileWatcher() {
        if (!m_fileWatcher) {
            m_fileWatcher = new QFileSystemWatcher(this);
            connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &DocumentWatcherPrivate::changedFile);
        }
        return m_fileWatcher;
    }

    QFileSystemWatcher *DocumentWatcherPrivate::linkWatcher() {
#ifndef Q_OS_WINDOWS
        if (!m_linkWatcher) {
            m_linkWatcher = new QFileSystemWatcher(this);
            m_linkWatcher->setObjectName(QLatin1String("_qt_autotest_force_engine_poller"));
            connect(m_linkWatcher, &QFileSystemWatcher::fileChanged, this, &DocumentWatcherPrivate::changedFile);
        }
        return m_linkWatcher;
#else
        return fileWatcher();
#endif
    }

    /* only called from addFileInfo(IDocument *) */
    void DocumentWatcherPrivate::addFileInfo(const QString &fileName, IDocument *document, bool isLink) {
        auto d = this;

        FileStateItem state;
        if (!fileName.isEmpty()) {
            const QFileInfo fi(fileName);
            state.modified = fi.lastModified();
            state.permissions = fi.permissions();
            // Add watcher if we don't have that already
            if (!d->m_states.contains(fileName))
                d->m_states.insert(fileName, FileState());

            auto watcher = isLink ? linkWatcher() : fileWatcher();
            // if (!watcher->files().contains(fileName))
            watcher->addPath(fileName);

            d->m_states[fileName].lastUpdatedState.insert(document, state);

            d->m_documentsWithWatch[document].append(fileName); // inserts a new QStringList if not already there
        }
    }

    /* Adds the IDocument's file, and possibly it's final link target to both m_states
       (if it's file name is not empty), and the m_filesWithWatch list,
       and adds a file watcher for each if not already done.
       (The added file names are guaranteed to be absolute and cleaned.) */
    void DocumentWatcherPrivate::addFileInfo(IDocument *document) {
        // Patch: skip if not exist, until the instance has been set a valid path
        if (!QMFs::isFileExist(document->filePath()))
            return;

        const QString fixedName = DocumentWatcher::fixFileName(document->filePath(), DocumentWatcher::KeepLinks);
        const QString fixedResolvedName =
            DocumentWatcher::fixFileName(document->filePath(), DocumentWatcher::ResolveLinks);
        addFileInfo(fixedResolvedName, document, false);
        if (fixedName != fixedResolvedName)
            addFileInfo(fixedName, document, true);
    }

    /* Removes all occurrences of the IDocument from m_filesWithWatch and m_states.
       If that results in a file no longer being referenced by any IDocument, this
       also removes the file watcher.
    */
    void DocumentWatcherPrivate::removeFileInfo(IDocument *document) {
        auto d = this;
        if (!d->m_documentsWithWatch.contains(document))
            return;
        foreach (const QString &fileName, d->m_documentsWithWatch.value(document)) {
            if (!d->m_states.contains(fileName))
                continue;
            d->m_states[fileName].lastUpdatedState.remove(document);
            if (d->m_states.value(fileName).lastUpdatedState.isEmpty()) {
                if (d->m_fileWatcher && d->m_fileWatcher->files().contains(fileName))
                    d->m_fileWatcher->removePath(fileName);
                if (d->m_linkWatcher && d->m_linkWatcher->files().contains(fileName))
                    d->m_linkWatcher->removePath(fileName);
                d->m_states.remove(fileName);
            }
        }
        d->m_documentsWithWatch.remove(document);
    }

    void DocumentWatcherPrivate::updateExpectedState(const QString &fileName) {
        auto d = this;

        if (fileName.isEmpty())
            return;
        if (d->m_states.contains(fileName)) {
            QFileInfo fi(fileName);
            d->m_states[fileName].expected.modified = fi.lastModified();
            d->m_states[fileName].expected.permissions = fi.permissions();
        }
    }

    bool DocumentWatcherPrivate::saveDocument_helper(IDocument *document, const QString &fileName, bool *isReadOnly) {
        Q_Q(DocumentWatcher);

        bool ret = true;
        QString effName = fileName.isEmpty() ? document->filePath() : fileName;
        q->expectFileChange(effName);                  // This only matters to other IDocuments which refer to this file
        bool addWatcher = q->removeDocument(document); // So that our own IDocument gets no notification at all

        if (!document->save(fileName)) {
            if (isReadOnly) {
                QFile ofi(effName);
                // Check whether the existing file is writable
                if (!ofi.open(QIODevice::ReadWrite) && ofi.open(QIODevice::ReadOnly)) {
                    *isReadOnly = true;
                    goto out;
                }
                *isReadOnly = false;
            }

            {
                QString errorString = document->errorMessage();
                QString filePath = QDir::toNativeSeparators(fileName);
                QMessageBox::critical(
                    document->dialogParent(), QApplication::translate("Core::DocumentWatcher", "File Error"),
                    errorString.isEmpty()
                        ? QApplication::translate("Core::DocumentWatcher", "Cannot save %1").arg(filePath)
                        : QApplication::translate("Core::DocumentWatcher", "Error while saving %1: %2")
                              .arg(filePath, errorString));
            }

        out:
            ret = false;
        }

        q->addDocument(document, addWatcher);
        q->unexpectFileChange(effName);

        return ret;
    }

    void DocumentWatcherPrivate::errorOnOverwrite(const QString &fileName, QWidget *parent) {
        QMessageBox::critical(parent, QApplication::translate("Core::DocumentWatcher", "File Error"),
                              QApplication::translate("Core::DocumentWatcher", "%1 has been opened in the editor!")
                                  .arg(QDir::toNativeSeparators(fileName)));
    }

    bool DocumentWatcherPrivate::eventFilter(QObject *obj, QEvent *event) {
        if (obj == qApp && event->type() == QEvent::ApplicationActivate) {
            // activeWindow is not necessarily set yet, do checkForReload asynchronously
            QTimer::singleShot(0, this, &DocumentWatcherPrivate::checkForReload);
        }
        return QObject::eventFilter(obj, event);
    }

    void DocumentWatcherPrivate::documentDestroyed(QObject *obj) {
        auto document = static_cast<IDocument *>(obj);
        // Check the special unwatched first:
        if (!m_documentsWithoutWatch.removeOne(document))
            removeFileInfo(document);
    }

    /* Slot reacting on IDocument::changed. We need to check if the signal was sent
       because the file was saved under different name. */
    void DocumentWatcherPrivate::checkForNewFileName() {
        auto d = this;

        auto document = qobject_cast<IDocument *>(sender());
        // We modified the IDocument
        // Trust the other code to also update the m_states map
        if (!document || document == d->m_blockedIDocument || d->m_documentsWithWatch.contains(document))
            return;

        // Maybe the name has changed or file has been deleted and created again ...
        // This also updates the state to the on disk state
        removeFileInfo(document);
        addFileInfo(document);
    }

    void DocumentWatcherPrivate::checkForReload() {
        Q_Q(DocumentWatcher);
        auto d = this;

        if (d->m_changedFiles.isEmpty())
            return;
        if (!QApplication::activeWindow())
            return;

        if (QApplication::activeModalWidget() || d->m_blockActivated) {
            // We do not want to prompt for modified file if we currently have some modal dialog open.
            // If d->m_blockActivated is true, then it means that the event processing of either the
            // file modified dialog, or of loading large files, has delivered a file change event from
            // a watcher *and* the timer triggered. We may never end up here in a nested way, so
            // recheck later.
            QTimer::singleShot(200, this, &DocumentWatcherPrivate::checkForReload);
            return;
        }

        d->m_blockActivated = true;

        if (!promptHandler)
            promptHandler = new PromptHandler();

        auto defaultBehavior = reloadSetting;
        PromptHandler::ReloadPromptAnswer previousReloadAnswer = PromptHandler::ReloadCurrent;
        PromptHandler::FileDeletedPromptAnswer previousDeletedAnswer = PromptHandler::FileDeletedSave;

        QList<IDocument *> documentsToClose;
        QHash<IDocument *, QString> documentsToSave;

        // collect file information
        QMap<QString, FileStateItem> currentStates;
        QMap<QString, IDocument::ChangeType> changeTypes;
        QSet<IDocument *> changedIDocuments;
        foreach (const QString &fileName, d->m_changedFiles) {
            IDocument::ChangeType type = IDocument::TypeContents;
            FileStateItem state;
            QFileInfo fi(fileName);
            if (!fi.exists()) {
                type = IDocument::TypeRemoved;
            } else {
                state.modified = fi.lastModified();
                state.permissions = fi.permissions();
            }
            currentStates.insert(fileName, state);
            changeTypes.insert(fileName, type);
            foreach (IDocument *document, d->m_states.value(fileName).lastUpdatedState.keys())
                changedIDocuments.insert(document);
        }

        // clean up. do this before we may enter the main loop, otherwise we would
        // lose consecutive notifications.
        d->m_changedFiles.clear();

        // collect information about "expected" file names
        // we can't do the "resolving" already in expectFileChange, because
        // if the resolved names are different when unexpectFileChange is called
        // we would end up with never-unexpected file names
        QSet<QString> expectedFileNames;
        foreach (const QString &fileName, d->m_expectedFileNames) {
            const QString fixedName = DocumentWatcher::fixFileName(fileName, DocumentWatcher::KeepLinks);
            expectedFileNames.insert(fixedName);
            const QString fixedResolvedName = DocumentWatcher::fixFileName(fileName, DocumentWatcher::ResolveLinks);
            if (fixedName != fixedResolvedName)
                expectedFileNames.insert(fixedResolvedName);
        }

        // handle the IDocuments
        foreach (IDocument *document, changedIDocuments) {
            IDocument::ChangeTrigger trigger = IDocument::TriggerInternal;
            IDocument::ChangeType type = IDocument::TypePermissions;
            bool changed = false;
            // find out the type & behavior from the two possible files
            // behavior is internal if all changes are expected (and none removed)
            // type is "max" of both types (remove > contents > permissions)
            foreach (const QString &fileName, d->m_documentsWithWatch.value(document)) {
                // was the file reported?
                if (!currentStates.contains(fileName))
                    continue;

                FileStateItem currentState = currentStates.value(fileName);
                FileStateItem expectedState = d->m_states.value(fileName).expected;
                FileStateItem lastState = d->m_states.value(fileName).lastUpdatedState.value(document);

                // did the file actually change?
                if (lastState.modified == currentState.modified && lastState.permissions == currentState.permissions)
                    continue;
                changed = true;

                // was it only a permission change?
                if (lastState.modified == currentState.modified)
                    continue;

                // was the change unexpected?
                if ((currentState.modified != expectedState.modified ||
                     currentState.permissions != expectedState.permissions) &&
                    !expectedFileNames.contains(fileName)) {
                    trigger = IDocument::TriggerExternal;
                }

                // find out the type
                IDocument::ChangeType fileChange = changeTypes.value(fileName);
                if (fileChange == IDocument::TypeRemoved)
                    type = IDocument::TypeRemoved;
                else if (fileChange == IDocument::TypeContents && type == IDocument::TypePermissions)
                    type = IDocument::TypeContents;
            }

            if (!changed) // probably because the change was blocked with (un)blockFileChange
                continue;

            // handle it!
            d->m_blockedIDocument = document;

            // Update file info, also handling if e.g. link target has changed.
            // We need to do that before the file is reloaded, because removing the watcher will
            // loose any pending change events. Loosing change events *before* the file is reloaded
            // doesn't matter, because in that case we then reload the new version of the file already
            // anyhow.
            removeFileInfo(document);
            addFileInfo(document);

            bool success = true;
            // we've got some modification
            // check if it's contents or permissions:
            if (type == IDocument::TypePermissions) {
                // Only permission change
                success = document->reload(IDocument::FlagReload, IDocument::TypePermissions);
                // now we know it's a content change or file was removed
            } else if (defaultBehavior == DocumentWatcher::ReloadUnmodified && type == IDocument::TypeContents &&
                       !document->isModified()) {
                // content change, but unmodified (and settings say to reload in this case)
                success = document->reload(IDocument::FlagReload, type);
                // file was removed, or it's a content change and the default behavior for
                // unmodified files didn't kick in
            } else if (defaultBehavior == DocumentWatcher::ReloadUnmodified && type == IDocument::TypeRemoved &&
                       !document->isModified()) {
                // file removed, but unmodified files should be reloaded
                // so we close the file
                documentsToClose << document;
            } else if (defaultBehavior == DocumentWatcher::IgnoreAll) {
                // content change or removed, but settings say ignore
                success = document->reload(IDocument::FlagIgnore, type);
                // either the default behavior is to always ask,
                // or the ReloadUnmodified default behavior didn't kick in,
                // so do whatever the IDocument wants us to do
            } else {
                // check if IDocument wants us to ask
                if (document->reloadBehavior(trigger, type) == IDocument::BehaviorSilent) {
                    // content change or removed, IDocument wants silent handling
                    if (type == IDocument::TypeRemoved)
                        documentsToClose << document;
                    else
                        success = document->reload(IDocument::FlagReload, type);
                    // IDocument wants us to ask
                } else if (type == IDocument::TypeContents) {
                    // content change, IDocument wants to ask user
                    if (previousReloadAnswer == PromptHandler::ReloadNone) {
                        // answer already given, ignore
                        success = document->reload(IDocument::FlagIgnore, IDocument::TypeContents);
                    } else if (previousReloadAnswer == PromptHandler::ReloadAll) {
                        // answer already given, reload
                        success = document->reload(IDocument::FlagReload, IDocument::TypeContents);
                    } else {
                        // Ask about content change
                        previousReloadAnswer = promptHandler->reloadPrompt(document->filePath(), document->isModified(),
                                                                           document->dialogParent());
                        switch (previousReloadAnswer) {
                            case PromptHandler::ReloadAll:
                            case PromptHandler::ReloadCurrent:
                                success = document->reload(IDocument::FlagReload, IDocument::TypeContents);
                                break;
                            case PromptHandler::ReloadSkipCurrent:
                            case PromptHandler::ReloadNone:
                                success = document->reload(IDocument::FlagIgnore, IDocument::TypeContents);
                                break;
                            case PromptHandler::CloseCurrent:
                                documentsToClose << document;
                                break;
                        }
                    }
                    // IDocument wants us to ask, and it's the TypeRemoved case
                } else {
                    // Ask about removed file
                    bool unhandled = true;
                    while (unhandled) {
                        if (previousDeletedAnswer != PromptHandler::FileDeletedCloseAll) {
                            previousDeletedAnswer = promptHandler->fileDeletedPrompt(
                                document->filePath(), trigger == IDocument::TriggerExternal, document->dialogParent());
                        }
                        switch (previousDeletedAnswer) {
                            case PromptHandler::FileDeletedSave:
                                documentsToSave.insert(document, document->filePath());
                                unhandled = false;
                                break;
                            case PromptHandler::FileDeletedSaveAs: {
                                const QString &saveFileName =
                                    q->getSaveAsFileName(document, {}, document->dialogParent());
                                if (!saveFileName.isEmpty()) {
                                    if (q->searchDocument(saveFileName)) {
                                        errorOnOverwrite(saveFileName, document->dialogParent());
                                    } else {
                                        documentsToSave.insert(document, saveFileName);
                                        unhandled = false;
                                    }
                                }
                                break;
                            }
                            case PromptHandler::FileDeletedClose:
                            case PromptHandler::FileDeletedCloseAll:
                                documentsToClose << document;
                                unhandled = false;
                                break;
                        }
                    }
                }
            }
            if (!success) {
                QString errorString = document->errorMessage();
                QString filePath = QDir::toNativeSeparators(document->filePath());
                QMessageBox::critical(
                    document->dialogParent(), QApplication::translate("Core::DocumentWatcher", "File Error"),
                    errorString.isEmpty()
                        ? QApplication::translate("Core::DocumentWatcher", "Cannot reload %1").arg(filePath)
                        : QApplication::translate("Core::DocumentWatcher", "Error while reloading %1: %2")
                              .arg(filePath, errorString));
            }

            d->m_blockedIDocument = nullptr;
        }

        // handle deleted files
        for (const auto &item : qAsConst(documentsToClose)) {
            q->removeDocument(item);
            item->close();
        }

        QHashIterator<IDocument *, QString> it(documentsToSave);
        while (it.hasNext()) {
            it.next();
            saveDocument_helper(it.key(), it.value(), nullptr);

            // it.key()->checkPermissions();
        }

        d->m_blockActivated = false;
    }

    void DocumentWatcherPrivate::changedFile(const QString &fileName) {
        auto d = this;
        const bool wasempty = d->m_changedFiles.isEmpty();

        if (d->m_states.contains(fileName))
            d->m_changedFiles.insert(fileName);

        if (wasempty && !d->m_changedFiles.isEmpty())
            QTimer::singleShot(200, this, &DocumentWatcherPrivate::checkForReload);
    }

    void DocumentWatcherPrivate::filePathChanged(const QString &oldName, const QString &newName) {
        Q_Q(DocumentWatcher);

        auto d = this;

        auto doc = qobject_cast<IDocument *>(sender());
        if (!doc || doc == d->m_blockedIDocument)
            return;

        emit q->documentRenamed(doc, oldName, newName);

        q->documentChanged(doc);
    }

    DocumentWatcher::DocumentWatcher(QObject *parent) : DocumentWatcher(*new DocumentWatcherPrivate(), parent) {
    }

    DocumentWatcher::~DocumentWatcher() {
    }

    DocumentWatcher::ReloadSetting DocumentWatcher::reloadSetting() const {
        Q_D(const DocumentWatcher);
        return d->reloadSetting;
    }

    void DocumentWatcher::setReloadSetting(DocumentWatcher::ReloadSetting setting) {
        Q_D(DocumentWatcher);
        d->reloadSetting = setting;
    }

    PromptHandler *DocumentWatcher::promptHandler() const {
        Q_D(const DocumentWatcher);
        return d->promptHandler;
    }

    PromptHandler *DocumentWatcher::takePromptHandler() {
        Q_D(DocumentWatcher);

        auto org = d->promptHandler;
        if (org) {
            org->setParent(nullptr);
            d->promptHandler = nullptr;
        }

        return org;
    }

    void DocumentWatcher::setPromptHandler(PromptHandler *handler) {
        Q_D(DocumentWatcher);
        delete takePromptHandler();

        handler->setParent(this);
        d->promptHandler = handler;
    }

    void DocumentWatcher::addDocuments(const QList<IDocument *> &documents, bool addWatcher) {
        Q_D(DocumentWatcher);

        if (!addWatcher) {
            // We keep those in a separate list

            foreach (IDocument *document, documents) {
                if (document && !d->m_documentsWithoutWatch.contains(document)) {
                    connect(document, &QObject::destroyed, d, &DocumentWatcherPrivate::documentDestroyed);
                    connect(document, &IDocument::filePathChanged, d, &DocumentWatcherPrivate::filePathChanged);
                    d->m_documentsWithoutWatch.append(document);
                    documentAdded(document, addWatcher);
                }
            }
            return;
        }

        foreach (IDocument *document, documents) {
            if (document && !d->m_documentsWithWatch.contains(document)) {
                connect(document, &IDocument::changed, d, &DocumentWatcherPrivate::checkForNewFileName);
                connect(document, &QObject::destroyed, d, &DocumentWatcherPrivate::documentDestroyed);
                connect(document, &IDocument::filePathChanged, d, &DocumentWatcherPrivate::filePathChanged);
                d->addFileInfo(document);
                documentAdded(document, addWatcher);
            }
        }
    }

    void DocumentWatcher::addDocument(IDocument *document, bool addWatcher) {
        addDocuments(QList<IDocument *>() << document, addWatcher);
    }

    bool DocumentWatcher::removeDocument(IDocument *document) {
        if (!document)
            return false;

        Q_D(DocumentWatcher);

        bool addWatcher = false;
        // Special casing unwatched files
        if (!d->m_documentsWithoutWatch.removeOne(document)) {
            addWatcher = true;
            d->removeFileInfo(document);
            disconnect(document, &IDocument::changed, d, &DocumentWatcherPrivate::checkForNewFileName);
        }
        disconnect(document, &QObject::destroyed, d, &DocumentWatcherPrivate::documentDestroyed);
        documentRemoved(document);

        return addWatcher;
    }

    bool DocumentWatcher::hasDocument(IDocument *document) const {
        Q_D(const DocumentWatcher);
        return d->m_documentsWithWatch.contains(document) || d->m_documentsWithoutWatch.contains(document);
    }

    QList<IDocument *> DocumentWatcher::modifiedDocuments() const {
        Q_D(const DocumentWatcher);
        QList<IDocument *> modified;

        foreach (IDocument *document, d->m_documentsWithWatch.keys()) {
            if (document->isModified())
                modified << document;
        }

        foreach (IDocument *document, d->m_documentsWithoutWatch) {
            if (document->isModified())
                modified << document;
        }

        return modified;
    }

    IDocument *DocumentWatcher::searchDocument(const QString &filePath) const {
        Q_D(const DocumentWatcher);

        if (!QMFs::isFileExist(filePath))
            return nullptr;

        const QString &fixedFrom = fixFileName(filePath, ResolveLinks);
        foreach (IDocument *document, d->m_documentsWithWatch.keys()) {
            if (fixFileName(document->filePath(), ResolveLinks) == fixedFrom)
                return document;
        }

        foreach (IDocument *document, d->m_documentsWithoutWatch) {
            if (fixFileName(document->filePath(), ResolveLinks) == fixedFrom)
                return document;
        }

        return nullptr;
    }

    void DocumentWatcher::renamedFile(const QString &from, const QString &to) {
        Q_D(DocumentWatcher);

        const QString &fixedFrom = fixFileName(from, KeepLinks);

        // gather the list of IDocuments
        QList<IDocument *> documentsToRename;
        QMapIterator<IDocument *, QStringList> it(d->m_documentsWithWatch);
        while (it.hasNext()) {
            it.next();
            if (it.value().contains(fixedFrom))
                documentsToRename.append(it.key());
        }

        // rename the IDocuments
        foreach (IDocument *document, documentsToRename) {
            d->m_blockedIDocument = document;
            d->removeFileInfo(document);
            document->setFilePath(to);
            d->addFileInfo(document);
            d->m_blockedIDocument = nullptr;
        }

        emit allDocumentsRenamed(from, to);
    }

    void DocumentWatcher::expectFileChange(const QString &fileName) {
        Q_D(DocumentWatcher);

        if (fileName.isEmpty())
            return;
        d->m_expectedFileNames.insert(fileName);
    }

    void DocumentWatcher::unexpectFileChange(const QString &fileName) {
        Q_D(DocumentWatcher);

        // We are updating the expected time of the file
        // And in changedFile we'll check if the modification time
        // is the same as the saved one here
        // If so then it's an expected change

        if (fileName.isEmpty())
            return;
        d->m_expectedFileNames.remove(fileName);
        const QString fixedName = fixFileName(fileName, KeepLinks);
        d->updateExpectedState(fixedName);
        const QString fixedResolvedName = fixFileName(fileName, ResolveLinks);
        if (fixedName != fixedResolvedName)
            d->updateExpectedState(fixedResolvedName);
    }

    bool DocumentWatcher::saveDocument(IDocument *document, const QString &fileName, bool *isReadOnly) {
        Q_D(DocumentWatcher);

        auto &doc = document;
        auto &saveFileName = fileName;

        if (!saveFileName.isEmpty()) {
            if (hasDocument(doc)) {
                auto doc2 = searchDocument(saveFileName);
                if (doc2) {
                    if (doc2 == doc) {
                        goto lab_save;
                    }
                    DocumentWatcherPrivate::errorOnOverwrite(saveFileName, doc->dialogParent());
                    return false;
                }
                return d->saveDocument_helper(doc, saveFileName, isReadOnly);
            } else
                goto lab_save;
        }
        return false;

    lab_save:
        return doc->save(saveFileName);
    }

    void DocumentWatcher::notifyFilesChangedInternally(const QStringList &files) {
        emit filesChangedInternally(files);
    }

    QString DocumentWatcher::fixFileName(const QString &fileName, DocumentWatcher::FixMode fixmode) {
        QString s = fileName;
        QFileInfo fi(s);
        if (fi.exists()) {
            if (fixmode == ResolveLinks)
                s = fi.canonicalFilePath();
            else
                s = QDir::cleanPath(fi.absoluteFilePath());
        } else {
            s = QDir::cleanPath(s);
        }
        s = QDir::toNativeSeparators(s);
#ifndef Q_OS_LINUX
        s = s.toLower();
#endif
        return s;
    }

    QString DocumentWatcher::getSaveAsFileName(const IDocument *document, const QString &path, QWidget *parent) const {
        return QFileDialog::getSaveFileName(
            parent, QCoreApplication::translate("Core::DocumentWatcher", "Save As File"), path,
            QString("%1(%2)").arg(QApplication::translate("Core::PromptHandler", "All Files"), QMOs::allFilesFilter()));
    }

    void DocumentWatcher::documentAdded(IDocument *document, bool addWatch) {
    }

    void DocumentWatcher::documentChanged(IDocument *document) {
    }

    void DocumentWatcher::documentRemoved(IDocument *document) {
    }

    DocumentWatcher::DocumentWatcher(DocumentWatcherPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

    class FileChangeBlockerPrivate {
    public:
        QString m_fileName;
        DocumentWatcher *watcher;
    };

    FileChangeBlocker::FileChangeBlocker(const QString &fileName, DocumentWatcher *watcher)
        : d(new FileChangeBlockerPrivate{fileName, watcher}) {
        watcher->expectFileChange(fileName);
    }

    FileChangeBlocker::~FileChangeBlocker() {
        d->watcher->unexpectFileChange(d->m_fileName);
    }

    PromptHandler::PromptHandler(QObject *parent) : QObject(parent) {
    }

    PromptHandler::~PromptHandler() {
    }

    PromptHandler::ReloadPromptAnswer PromptHandler::reloadPrompt(const QString &fileName, bool modified,
                                                                  QWidget *parent) {

        const QString title = QCoreApplication::translate("Core::PromptHandler", "File Changed");
        QString prompt;

        if (modified) {
            prompt = QCoreApplication::translate("Core::PromptHandler",
                                                 "The unsaved file %1 has changed outside. "
                                                 "Do you want to reload it and discard your changes?");
        } else {
            prompt = QCoreApplication::translate("Core::PromptHandler",
                                                 "The file %1 has changed outside. Do you want to reload it?");
        }
        prompt = prompt.arg(QDir::toNativeSeparators(fileName));

        QMessageBox msgbox(parent);
        msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Close | QMessageBox::No |
                                  QMessageBox::NoToAll);
        msgbox.setDefaultButton(QMessageBox::YesToAll);
        msgbox.setWindowTitle(title);
        msgbox.setText(prompt);
        // msgbox.setDetailedText(QDir::toNativeSeparators(fileName));

        switch (msgbox.exec()) {
            case QMessageBox::Yes:
                return ReloadCurrent;
            case QMessageBox::YesToAll:
                return ReloadAll;
            case QMessageBox::No:
                return ReloadSkipCurrent;
            case QMessageBox::Close:
                return CloseCurrent;
            default:
                break;
        }
        return ReloadNone;
    }

    PromptHandler::FileDeletedPromptAnswer PromptHandler::fileDeletedPrompt(const QString &fileName,
                                                                            bool triggerExternally, QWidget *parent) {

        const QString title = QCoreApplication::translate("Core::PromptHandler", "File has been removed");
        QString msg;
        if (triggerExternally) {
            // Unreachable code
            msg = QCoreApplication::translate("Core::PromptHandler",
                                              "The file %1 has been removed outside. "
                                              "Do you want to save it under a different name, or close "
                                              "the editor?")
                      .arg(QDir::toNativeSeparators(fileName));
        } else {
            msg = QCoreApplication::translate("Core::PromptHandler",
                                              "The file %1 was removed. "
                                              "Do you want to save it under a different name, or close "
                                              "the editor?")
                      .arg(QDir::toNativeSeparators(fileName));
        }
        QMessageBox box(QMessageBox::Question, title, msg, QMessageBox::NoButton, parent);
        QPushButton *close =
            box.addButton(QCoreApplication::translate("Core::PromptHandler", "&Close"), QMessageBox::RejectRole);
        QPushButton *closeAll =
            box.addButton(QCoreApplication::translate("Core::PromptHandler", "C&lose All"), QMessageBox::RejectRole);
        QPushButton *saveas =
            box.addButton(QCoreApplication::translate("Core::PromptHandler", "Save &as..."), QMessageBox::ActionRole);
        QPushButton *save =
            box.addButton(QCoreApplication::translate("Core::PromptHandler", "&Save"), QMessageBox::AcceptRole);
        box.setDefaultButton(saveas);
        box.exec();
        QAbstractButton *clickedbutton = box.clickedButton();
        if (clickedbutton == close)
            return FileDeletedClose;
        else if (clickedbutton == closeAll)
            return FileDeletedCloseAll;
        else if (clickedbutton == saveas)
            return FileDeletedSaveAs;
        else if (clickedbutton == save)
            return FileDeletedSave;
        return FileDeletedClose;
    }

}
