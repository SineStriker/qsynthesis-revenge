#include "DocumentSystem.h"
#include "DocumentSystem_p.h"

#include "IDocument_p.h"

#include "ICoreBase.h"
#include "ILoader.h"

#include <QMBatch.h>
#include <QMCoreAppExtension.h>
#include <QMSystem.h>

#include <QApplication>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QJsonArray>
#include <QListWidget>
#include <QMessageBox>
#include <QScreen>
#include <QTimer>

namespace Core {

#define myWarning(func)                                                                            \
    (qWarning().nospace() << "Core::DocumentSystem::"                                              \
                          << "():")                                                                \
        .space()

    static const char settingCategoryC[] = "DocumentSystem";

    static const char recentGroupC[] = "RecentFiles";
    static const char filesKeyC[] = "Files";
    static const char dirKeyC[] = "Directories";

    static const char lastVisitGroupC[] = "LastVisitDirs";
    static const char openFileLastVisitDirC[] = "OpenFile";
    static const char openDirLastVisitDirC[] = "OpenDir";
    static const char saveFileLastVisitDirC[] = "SaveFile";

    static const char selectAllWhenRecoverC[] = "SelectAllWhenRecover";

    DocumentSystemPrivate::DocumentSystemPrivate() {
        openFilesSaved = true;
    }

    DocumentSystemPrivate::~DocumentSystemPrivate() {
    }

    void DocumentSystemPrivate::init() {
        readSettings();

        QMFs::mkDir(DocumentSystem::logBaseDir());
    }

    void DocumentSystemPrivate::readSettings() {
        auto s = ILoader::instance()->settings();

        auto obj = s->value(settingCategoryC).toObject();

        auto recentObj = obj.value(recentGroupC).toObject();
        QStringList recentFiles =
            QMBatch::arrayToStringList(recentObj.value(QLatin1String(filesKeyC)).toArray());
        QStringList recentDirs =
            QMBatch::arrayToStringList(recentObj.value(QLatin1String(dirKeyC)).toArray());

        // clean non-existing files
        m_recentFiles.clear();
        foreach (const QString &fileName, recentFiles) {
            if (QFileInfo(fileName).isFile())
                m_recentFiles.append(fileName);
        }

        // clean non-existing dirs
        m_recentDirs.clear();
        foreach (const QString &fileName, recentDirs) {
            if (QFileInfo(fileName).isDir())
                m_recentDirs.append(fileName);
        }

        auto lastVisitObj = obj.value(lastVisitGroupC).toObject();
        openFileLastVisitDir = lastVisitObj.value(openFileLastVisitDirC).toString();
        openDirLastVisitDir = lastVisitObj.value(openDirLastVisitDirC).toString();
        saveFileLastVisitDir = lastVisitObj.value(saveFileLastVisitDirC).toString();

        selectAllWhenRecover = obj.value(selectAllWhenRecoverC).toBool();
    }

    void DocumentSystemPrivate::saveSettings() const {
        auto s = ILoader::instance()->settings();

        QJsonObject obj;

        QJsonObject recentObj;
        recentObj.insert(filesKeyC, QJsonArray::fromStringList(m_recentFiles));
        recentObj.insert(dirKeyC, QJsonArray::fromStringList(m_recentDirs));
        obj.insert(recentGroupC, recentObj);

        QJsonObject lastVisitObj;
        lastVisitObj.insert(QLatin1String(openFileLastVisitDirC), openFileLastVisitDir);
        lastVisitObj.insert(QLatin1String(openDirLastVisitDirC), openDirLastVisitDir);
        lastVisitObj.insert(QLatin1String(saveFileLastVisitDirC), saveFileLastVisitDir);
        obj.insert(lastVisitGroupC, lastVisitObj);

        obj.insert(selectAllWhenRecoverC, selectAllWhenRecover);

        s->insert(settingCategoryC, obj);
    }

    void DocumentSystemPrivate::saveOpenFileSettings() const {
        openFilesSaved = true;
    }

    void DocumentSystemPrivate::postSaveOpenFilesTask() {
        if (!openFilesSaved) {
            return;
        }

        openFilesSaved = false;
        QTimer::singleShot(0, this, &DocumentSystemPrivate::saveOpenFileSettings);
    }

    void DocumentSystemPrivate::_q_docInfoChanged() {
        auto doc = qobject_cast<IDocument *>(sender());
        docInfos.append(doc, doc->docInfo());
        postSaveOpenFilesTask();
    }

    void DocumentSystemPrivate::_q_documentDestroyed() {
        docInfos.remove(static_cast<IDocument *>(sender()));
        postSaveOpenFilesTask();
    }

    static DocumentSystem *m_instance = nullptr;

    DocumentSystem::DocumentSystem(QObject *parent)
        : DocumentSystem(*new DocumentSystemPrivate(), parent) {
    }

    DocumentSystem::~DocumentSystem() {
        Q_D(DocumentSystem);
        d->saveSettings();

        m_instance = nullptr;
    }

    QString DocumentSystem::logBaseDir() {
        return QString("%1/logs").arg(qAppExt->tempDir());
    }

    bool DocumentSystem::addDocType(DocumentSpec *doc) {
        Q_D(DocumentSystem);
        if (!doc) {
            myWarning(__func__) << "trying to add null document";
            return false;
        }
        if (d->docSpecs.contains(doc->id())) {
            myWarning(__func__) << "trying to add duplicated document:" << doc->id();
            return false;
        }
        doc->setParent(this);
        d->docSpecs.append(doc->id(), doc);

        for (const auto &ext : doc->supportedExtensions()) {
            d->extensionsMap[ext].append(doc);
        }

        return true;
    }

    bool DocumentSystem::removeDocType(DocumentSpec *doc) {
        if (doc == nullptr) {
            myWarning(__func__) << "trying to remove null document";
            return false;
        }
        return removeDocType(doc->id());
    }

    bool DocumentSystem::removeDocType(const QString &id) {
        Q_D(DocumentSystem);
        auto it = d->docSpecs.find(id);
        if (it == d->docSpecs.end()) {
            myWarning(__func__) << "document does not exist:" << id;
            return false;
        }

        auto doc = it.value();
        doc->setParent(nullptr);
        d->docSpecs.erase(it);

        for (const auto &ext : doc->supportedExtensions()) {
            auto it2 = d->extensionsMap.find(ext);
            if (it2 == d->extensionsMap.end())
                continue;
            it2->remove(doc);
            if (it2->isEmpty())
                d->extensionsMap.erase(it2);
        }

        return true;
    }

    DocumentSpec *DocumentSystem::docType(const QString &id) const {
        Q_D(const DocumentSystem);
        return d->docSpecs.value(id, nullptr);
    }

    QList<DocumentSpec *> DocumentSystem::docTypes() const {
        Q_D(const DocumentSystem);
        return d->docSpecs.values();
    }

    QStringList DocumentSystem::docTypeIds() const {
        Q_D(const DocumentSystem);
        return d->docSpecs.keys();
    }

    DocumentSpec *DocumentSystem::supportedDocType(const QString &suffix) const {
        auto id = preferredDocTypeId(suffix);
        DocumentSpec *spec;
        if (id.isEmpty() || !(spec = docType(id))) {
            auto tmp = supportedDocTypes(suffix);
            return tmp.isEmpty() ? nullptr : tmp.front();
        }
        return spec;
    }

    QList<DocumentSpec *> DocumentSystem::supportedDocTypes(const QString &suffix) const {
        Q_D(const DocumentSystem);
        return d->extensionsMap.value(suffix, {}).values();
    }

    QStringList DocumentSystem::supportedExtensions() const {
        Q_D(const DocumentSystem);
        return d->extensionsMap.keys();
    }

    QString DocumentSystem::preferredDocTypeId(const QString &suffix) const {
        Q_D(const DocumentSystem);
        return d->preferredExtensionIdMap.value(suffix);
    }

    void DocumentSystem::setPreferredDocTypeId(const QString &suffix, const QString &id) {
        Q_D(DocumentSystem);
        if (id.isEmpty()) {
            d->preferredExtensionIdMap.remove(suffix);
            return;
        }
        d->preferredExtensionIdMap.insert(suffix, id);
    }

    void _modify_unique_file(const QString &name, QStringList &files, bool remove = false) {
        QString unifiedForm(DocumentSystem::fixFileName(name, DocumentSystem::KeepLinks));
        QMutableListIterator<QString> it(files);
        while (it.hasNext()) {
            auto file = it.next();
            QString recentUnifiedForm(DocumentSystem::fixFileName(file, DocumentSystem::KeepLinks));
            if (unifiedForm == recentUnifiedForm)
                it.remove();
        }

        if (!remove)
            files.prepend(name);
    }

    void DocumentSystem::addRecentFile(const QString &fileName) {
        Q_D(DocumentSystem);

        if (fileName.isEmpty())
            return;
        _modify_unique_file(fileName, d->m_recentFiles);
        emit recentFilesChanged();
    }

    void DocumentSystem::removeRecentFile(const QString &fileName) {
        Q_D(DocumentSystem);

        if (fileName.isEmpty())
            return;
        _modify_unique_file(fileName, d->m_recentFiles, true);
        emit recentFilesChanged();
    }

    void DocumentSystem::clearRecentFiles() {
        Q_D(DocumentSystem);
        d->m_recentFiles.clear();
        emit recentFilesChanged();
    }

    QStringList DocumentSystem::recentFiles() const {
        Q_D(const DocumentSystem);
        return d->m_recentFiles;
    }

    void DocumentSystem::addRecentDir(const QString &fileName) {
        Q_D(DocumentSystem);

        if (fileName.isEmpty())
            return;
        _modify_unique_file(fileName, d->m_recentDirs);
        emit recentDirsChanged();
    }

    void DocumentSystem::clearRecentDirs() {
        Q_D(DocumentSystem);
        d->m_recentDirs.clear();
        emit recentDirsChanged();
    }

    QStringList DocumentSystem::recentDirs() const {
        Q_D(const DocumentSystem);
        return d->m_recentDirs;
    }

    bool DocumentSystem::openFileBrowse(QWidget *parent, DocumentSpec *spec,
                                        const QString &path) const {
        auto filter =
            spec->filter() + ";;" +
            QString("%1(%2)").arg(QApplication::translate("Core::DocumentSystem", "All Files"),
                                  QMOs::allFilesFilter());
        auto paths = getOpenFileNames(parent, {}, filter, path);
        if (paths.isEmpty()) {
            return false;
        }

        int cnt = 0;
        for (const auto &item : qAsConst(paths)) {
            if (spec->open(item, parent))
                cnt++;
        }

        return cnt > 0;
    }

    bool DocumentSystem::saveFileBrowse(QWidget *parent, IDocument *doc,
                                        const QString &path) const {
        Q_D(const DocumentSystem);
        const QString &saveFileName = getSaveAsFileName(doc, path, parent);
        if (!saveFileName.isEmpty()) {
            if (d->m_documentsWithWatch.contains(doc) || d->m_documentsWithoutWatch.contains(doc)) {
                auto doc2 = searchDocument(saveFileName);
                if (doc2) {
                    if (doc2 == doc) {
                        goto lab_save;
                    }
                    DocumentSystemPrivate::errorOnOverwrite(saveFileName, parent);
                    return false;
                }
                return const_cast<DocumentSystemPrivate *>(d)->saveDocument_helper(
                    doc, saveFileName, nullptr);
            } else
                goto lab_save;
        }
        return false;

    lab_save:
        return doc->save(saveFileName);
    }

    QString DocumentSystem::getOpenFileName(QWidget *parent, const QString &title,
                                            const QString &filters, const QString &path,
                                            QString *selectedFilter) const {
        Q_D(const DocumentSystem);
        auto res = QFileDialog::getOpenFileName(
            parent,
            title.isEmpty() ? QApplication::translate("Core::DocumentSystem", "Open File") : title,
            path.isEmpty() ? d->openFileLastVisitDir : path, filters, selectedFilter);
        if (!res.isEmpty()) {
            d->openFileLastVisitDir = QMFs::PathFindDirPath(res);
        }
        return res;
    }

    QStringList DocumentSystem::getOpenFileNames(QWidget *parent, const QString &title,
                                                 const QString &filters, const QString &path,
                                                 QString *selectedFilter) const {
        Q_D(const DocumentSystem);
        auto res = QFileDialog::getOpenFileNames(
            parent,
            title.isEmpty() ? QApplication::translate("Core::DocumentSystem", "Open Files") : title,
            path.isEmpty() ? d->openFileLastVisitDir : path, filters, selectedFilter);
        if (!res.isEmpty()) {
            d->openFileLastVisitDir = QMFs::PathFindDirPath(res.first());
        }
        return res;
    }

    QString DocumentSystem::getExistingDirectory(QWidget *parent, const QString &title,
                                                 const QString &path) const {
        Q_D(const DocumentSystem);
        auto res = QFileDialog::getExistingDirectory(
            parent,
            title.isEmpty() ? QApplication::translate("Core::DocumentSystem", "Open Directory")
                            : title,
            path.isEmpty() ? d->openDirLastVisitDir : path);
        if (!res.isEmpty()) {
            d->openDirLastVisitDir = QMFs::PathFindDirPath(res);
        }
        return res;
    }

    QString DocumentSystem::getSaveFileName(QWidget *parent, const QString &title,
                                            const QString &path, const QString &filter,
                                            QString *selectedFilter) const {
        Q_D(const DocumentSystem);
        auto res = QFileDialog::getSaveFileName(
            parent,
            title.isEmpty() ? QApplication::translate("Core::DocumentSystem", "Save File") : title,
            path.isEmpty() ? d->saveFileLastVisitDir : path, filter, selectedFilter);
        if (!res.isEmpty()) {
            d->saveFileLastVisitDir = QMFs::PathFindDirPath(res);
        }
        return res;
    }

    //    int DocumentSystem::checkRemainingLogs(QWidget *parent) const {
    //        Q_D(const DocumentSystem);

    //        QDir baseDir(DocumentSystemPrivate::logBaseDir());
    //        QFileInfoList fileInfos = baseDir.entryInfoList(QDir::Dirs | QDir::NoSymLinks |
    //        QDir::NoDotAndDotDot);

    //        struct Remaining {
    //            DocumentSpec *spec;
    //            QString file;
    //            QString logDir;
    //        };

    //        QList<Remaining> remaining;
    //        for (const auto &info : qAsConst(fileInfos)) {
    //            if (info.birthTime() > ILoader::atime()) {
    //                continue;
    //            }

    //            QDir dir(info.absoluteFilePath());
    //            IDocumentSettings settings(dir.path());

    //            const QString &id = settings.docType();
    //            const QString &path = settings.fileName();

    //            auto spec = docType(id);
    //            if (spec && spec->canRecover() && !path.isEmpty()) {
    //                remaining.append({spec, path, dir.path()});
    //                continue;
    //            }
    //            dir.removeRecursively();
    //        }

    //        if (remaining.isEmpty())
    //            return 0;

    //        auto listWidget = new QListWidget();
    //        for (const auto &rem : qAsConst(remaining)) {
    //            auto item = new QListWidgetItem();
    //            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    //            item->setText(QDir::toNativeSeparators(rem.file));
    //            item->setCheckState(Qt::Checked);
    //            listWidget->addItem(item);
    //        }

    //        auto allCheckbox = new QCheckBox(QApplication::translate("Core::DocumentSystem",
    //        "Restore all")); connect(allCheckbox, &QCheckBox::toggled, listWidget,
    //        &QListWidget::setDisabled);

    //        QMessageBox msgBox(parent);
    //        // msgBox.setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, false);
    //        msgBox.setIcon(QMessageBox::Question);
    //        msgBox.setText(QApplication::translate(
    //            "Core::DocumentSystem",
    //            "The following files have been detected that closed unexpectedly, would you like
    //            to restore them?"));
    //        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    //        msgBox.setCheckBox(allCheckbox);

    //        auto layout = qobject_cast<QGridLayout *>(msgBox.layout());
    //        int index = layout->indexOf(allCheckbox);
    //        int row, column, rowSpan, columnSpan;
    //        layout->getItemPosition(index, &row, &column, &rowSpan, &columnSpan);
    //        layout->addWidget(listWidget, row + 1, column, rowSpan, columnSpan);

    //        double ratio = (msgBox.screen()->logicalDotsPerInch() / QMOs::unitDpi());
    //        auto horizontalSpacer = new QSpacerItem(qMax<int>(ratio * 500,
    //        listWidget->sizeHint().width() + 100), 0,
    //                                                QSizePolicy::Minimum, QSizePolicy::Expanding);
    //        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    //        allCheckbox->setChecked(d->selectAllWhenRecover);
    //        int result = msgBox.exec();
    //        d->selectAllWhenRecover = allCheckbox->isChecked();

    //        if (result == QMessageBox::No) {
    //            for (const auto &rem : qAsConst(remaining)) {
    //                QDir(rem.logDir).removeRecursively();
    //            }
    //            return 0;
    //        }

    //        int cnt = 0;
    //        for (int i = 0; i < listWidget->count(); ++i) {
    //            auto item = listWidget->item(i);
    //            auto rem = remaining.at(i);
    //            if (!allCheckbox->isChecked() && item->checkState() == Qt::Unchecked) {
    //                QDir(rem.logDir).removeRecursively();
    //                continue;
    //            }

    //            if (rem.spec->recover(rem.logDir, rem.file)) {
    //                cnt++;
    //            }
    //        }

    //        return cnt;
    //    }

    //    QList<IDocumentSettings> DocumentSystem::remainingLogs() {
    //        if (!ILoader::instance())
    //            return {};

    //        static QList<IDocumentSettings> _logs = []() {
    //            QDir baseDir(DocumentSystemPrivate::logBaseDir());
    //            QFileInfoList fileInfos = baseDir.entryInfoList(QDir::Dirs | QDir::NoSymLinks |
    //            QDir::NoDotAndDotDot);

    //            QList<IDocumentSettings> remaining;
    //            for (const auto &info : qAsConst(fileInfos)) {
    //                if (info.birthTime() >= ILoader::atime()) {
    //                    continue;
    //                }
    //                remaining.append(IDocumentSettings(info.absoluteFilePath()));
    //            }

    //            return remaining;
    //        }();

    //        return _logs;
    //    }

    QString DocumentSystem::getSaveAsFileName(const IDocument *document, const QString &pathIn,
                                              QWidget *parent) const {
        Q_D(const DocumentSystem);
        auto spec = document->d_func()->spec;
        if (!spec) {
            return {};
        }

        auto filter =
            spec->saveFilter() + ";;" +
            QString("%1(%2)").arg(QApplication::translate("Core::DocumentSystem", "All Files"),
                                  QMOs::allFilesFilter());

        QString absoluteFilePath = document->filePath();
        const QFileInfo fi(absoluteFilePath);
        QString path =
            QMFs::isPathRelative(pathIn) ? d->saveFileLastVisitDir : QMFs::PathFindDirPath(pathIn);
        QString fileName;
        if (absoluteFilePath.isEmpty()) {
            fileName = document->suggestedFileName();
            const QString defaultPath = document->defaultPath();
            if (!defaultPath.isEmpty())
                path = defaultPath;
            if (fileName.isEmpty() && !pathIn.isEmpty())
                fileName = QMFs::PathFindFileName(pathIn);
        } else {
            path = fi.absolutePath();
            fileName = fi.fileName();
        }

        return getSaveFileName(parent,
                               QApplication::translate("Core::DocumentSystem", "Save As File"),
                               (path + QLatin1Char('/') + fileName), filter);
    }

    void DocumentSystem::documentAdded(IDocument *document, bool addWatch) {
        Q_UNUSED(addWatch);

        Q_D(DocumentSystem);
        connect(document, &IDocument::docInfoChanged, d, &DocumentSystemPrivate::_q_docInfoChanged);
        connect(document, &QObject::destroyed, d, &DocumentSystemPrivate::_q_documentDestroyed);
        d->docInfos.append(document, document->docInfo());
        d->postSaveOpenFilesTask();
    }

    void DocumentSystem::documentChanged(IDocument *document) {
        Q_UNUSED(document);
    }

    void DocumentSystem::documentRemoved(IDocument *document) {
        Q_D(DocumentSystem);
        disconnect(document, &IDocument::docInfoChanged, d,
                   &DocumentSystemPrivate::_q_docInfoChanged);
        disconnect(document, &QObject::destroyed, d, &DocumentSystemPrivate::_q_documentDestroyed);
        d->docInfos.remove(document);
        d->postSaveOpenFilesTask();
    }

    DocumentSystem::DocumentSystem(DocumentSystemPrivate &d, QObject *parent)
        : DocumentWatcher(d, parent) {
        m_instance = this;

        d.init();
    }

} // namespace Core
