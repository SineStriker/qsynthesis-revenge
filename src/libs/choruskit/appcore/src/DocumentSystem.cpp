#include "DocumentSystem.h"
#include "DocumentSystem_p.h"

#include "IDocument_p.h"

#include "ILoader.h"

#include <QMMath.h>
#include <QMSystem.h>

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>

namespace Core {

    static const char settingCategoryC[] = "DocumentSystem";

    static const char recentGroupC[] = "RecentFiles";
    static const char filesKeyC[] = "Files";
    static const char dirKeyC[] = "Directories";

    static const char lastVisitGroupC[] = "LastVisitDirs";
    static const char openFileLastVisitDirC[] = "OpenFile";
    static const char openDirLastVisitDirC[] = "OpenDir";
    static const char saveFileLastVisitDirC[] = "SaveFile";

    DocumentSystemPrivate::DocumentSystemPrivate() {
    }

    DocumentSystemPrivate::~DocumentSystemPrivate() {
    }

    void DocumentSystemPrivate::init() {
        readSettings();
    }

    void DocumentSystemPrivate::readSettings() {
        auto s = ILoader::instance()->settings();

        auto obj = s->value(settingCategoryC).toObject();

        auto recentObj = obj.value(recentGroupC).toObject();
        QStringList recentFiles = QMMath::arrayToStringList(recentObj.value(QLatin1String(filesKeyC)).toArray());
        QStringList recentDirs = QMMath::arrayToStringList(recentObj.value(QLatin1String(dirKeyC)).toArray());

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

        s->insert(settingCategoryC, obj);
    }

    static DocumentSystem *m_instance = nullptr;

    DocumentSystem::DocumentSystem(QObject *parent) : DocumentSystem(*new DocumentSystemPrivate(), parent) {
    }

    DocumentSystem::~DocumentSystem() {
        m_instance = nullptr;
        Q_D(DocumentSystem);
        d->saveSettings();
    }

    bool DocumentSystem::addDocType(DocumentSpec *doc) {
        Q_D(DocumentSystem);
        if (!doc) {
            qWarning() << "Core::DocumentSystem::addDocType(): trying to add null document";
            return false;
        }
        if (d->documents.contains(doc->id())) {
            qWarning() << "Core::DocumentSystem::addDocType(): trying to add duplicated document:" << doc->id();
            return false;
        }
        doc->setParent(this);
        d->documents.append(doc->id(), doc);

        for (const auto &ext : doc->supportedExtensions()) {
            d->extensionsMap[ext].append(doc);
        }

        return true;
    }

    bool DocumentSystem::removeDocType(DocumentSpec *doc) {
        if (doc == nullptr) {
            qWarning() << "Core::DocumentSystem::removeDocType(): trying to remove null document";
            return false;
        }
        return removeDocType(doc->id());
    }

    bool DocumentSystem::removeDocType(const QString &id) {
        Q_D(DocumentSystem);
        auto it = d->documents.find(id);
        if (it == d->documents.end()) {
            qWarning() << "Core::DocumentSystem::removeDocType(): document does not exist:" << id;
            return false;
        }

        auto doc = it.value();
        doc->setParent(nullptr);
        d->documents.erase(it);

        for (const auto &ext : doc->supportedExtensions()) {
            auto it2 = d->extensionsMap.find(ext);
            if (it2 == d->extensionsMap.end())
                continue;
            auto &set = it2.value();
            it2->remove(doc);
            if (it2->isEmpty())
                d->extensionsMap.erase(it2);
        }

        return true;
    }

    DocumentSpec *DocumentSystem::docType(const QString &id) const {
        Q_D(const DocumentSystem);
        return d->documents.value(id, nullptr);
    }

    QList<DocumentSpec *> DocumentSystem::docTypes() const {
        Q_D(const DocumentSystem);
        return d->documents.values();
    }

    QStringList DocumentSystem::docTypeIds() const {
        Q_D(const DocumentSystem);
        return d->documents.keys();
    }

    QList<DocumentSpec *> DocumentSystem::supportedDocTypes(const QString &suffix) const {
        Q_D(const DocumentSystem);
        return d->extensionsMap.value(suffix, {}).values();
    }

    void _check_unique_file(const QString &name, QStringList &files) {
        QString unifiedForm(DocumentSystem::fixFileName(name, DocumentSystem::KeepLinks));
        QMutableListIterator<QString> it(files);
        while (it.hasNext()) {
            auto file = it.next();
            QString recentUnifiedForm(DocumentSystem::fixFileName(file, DocumentSystem::KeepLinks));
            if (unifiedForm == recentUnifiedForm)
                it.remove();
        }
        files.prepend(name);
    }

    void DocumentSystem::addRecentFile(const QString &fileName) {
        Q_D(DocumentSystem);

        if (fileName.isEmpty())
            return;
        _check_unique_file(fileName, d->m_recentFiles);
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
        _check_unique_file(fileName, d->m_recentDirs);
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

    bool DocumentSystem::openFileBrowse(DocumentSpec *spec, const QString &path, QWidget *parent) const {
        Q_D(const DocumentSystem);
        auto filter =
            spec->filter() + ";;" +
            QString("%1(%2)").arg(QApplication::translate("Core::DocumentSystem", "All Files"), QMOs::allFilesFilter());
        auto path2 = getOpenFileName(parent, {}, filter, path);
        if (path2.isEmpty()) {
            return false;
        }
        return spec->open(path2);
    }

    QString DocumentSystem::getOpenFileName(QWidget *parent, const QString &title, const QString &filters,
                                            const QString &path, QString *selectedFilter) const {
        Q_D(const DocumentSystem);
        auto res = QFileDialog::getOpenFileName(
            parent, title.isEmpty() ? QApplication::translate("Core::DocumentSystem", "Open File") : title,
            path.isEmpty() ? d->openFileLastVisitDir : path, filters, selectedFilter);
        if (!res.isEmpty()) {
            d->openFileLastVisitDir = QMFs::PathFindDirPath(res);
        }
        return res;
    }

    QStringList DocumentSystem::getOpenFileNames(QWidget *parent, const QString &title, const QString &filters,
                                                 const QString &path, QString *selectedFilter) const {
        Q_D(const DocumentSystem);
        auto res = QFileDialog::getOpenFileNames(
            parent, title.isEmpty() ? QApplication::translate("Core::DocumentSystem", "Open Files") : title,
            path.isEmpty() ? d->openFileLastVisitDir : path, filters, selectedFilter);
        if (!res.isEmpty()) {
            d->openFileLastVisitDir = QMFs::PathFindDirPath(res.first());
        }
        return res;
    }

    QString DocumentSystem::getExistingDirectory(QWidget *parent, const QString &title, const QString &path) const {
        Q_D(const DocumentSystem);
        auto res = QFileDialog::getExistingDirectory(
            parent, title.isEmpty() ? QApplication::translate("Core::DocumentSystem", "Open Directory") : title,
            path.isEmpty() ? d->openDirLastVisitDir : path);
        if (!res.isEmpty()) {
            d->openDirLastVisitDir = QMFs::PathFindDirPath(res);
        }
        return res;
    }

    QString DocumentSystem::getSaveFileName(QWidget *parent, const QString &title, const QString &path,
                                            const QString &filter, QString *selectedFilter) const {
        Q_D(const DocumentSystem);
        auto res = QFileDialog::getSaveFileName(
            parent, title.isEmpty() ? QApplication::translate("Core::DocumentSystem", "Save File") : title,
            path.isEmpty() ? d->saveFileLastVisitDir : path, filter, selectedFilter);
        if (!res.isEmpty()) {
            d->saveFileLastVisitDir = QMFs::PathFindDirPath(res);
        }
        return res;
    }

    QString DocumentSystem::getSaveAsFileName(const IDocument *document, QWidget *parent) {
        Q_D(const DocumentSystem);
        auto spec = document->d_ptr->spec;
        if (!spec) {
            return {};
        }

        auto filter =
            spec->saveFilter() + ";;" +
            QString("%1(%2)").arg(QApplication::translate("Core::DocumentSystem", "All Files"), QMOs::allFilesFilter());

        QString absoluteFilePath = document->filePath();
        const QFileInfo fi(absoluteFilePath);
        QString path = d->saveFileLastVisitDir;
        QString fileName;
        if (absoluteFilePath.isEmpty()) {
            fileName = document->suggestedFileName();
            const QString defaultPath = document->defaultPath();
            if (!defaultPath.isEmpty())
                path = defaultPath;
        } else {
            path = fi.absolutePath();
            fileName = fi.fileName();
        }

        return getSaveFileName(parent, QApplication::translate("Core::DocumentSystem", "Save As File"),
                               (path + QLatin1Char('/') + fileName), filter);
    }

    DocumentSystem::DocumentSystem(DocumentSystemPrivate &d, QObject *parent) : DocumentWatcher(d, parent) {
        m_instance = this;

        d.init();
    }

}
