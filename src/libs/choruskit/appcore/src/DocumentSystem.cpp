#include "DocumentSystem.h"
#include "DocumentSystem_p.h"

#include "IDocument_p.h"

#include "ILoader.h"

#include <QMMath.h>
#include <QMSystem.h>

#include <QApplication>
#include <QDir>
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

    bool DocumentSystem::addDocument(DocumentSpec *document) {
        Q_D(DocumentSystem);
        if (!document) {
            qWarning() << "Core::DocumentSystem::addDocument(): trying to add null document";
            return false;
        }
        if (d->documents.contains(document->id())) {
            qWarning() << "Core::DocumentSystem::addDocument(): trying to add duplicated document:" << document->id();
            return false;
        }
        document->setParent(this);
        d->documents.append(document->id(), document);

        return true;
    }

    bool DocumentSystem::removeDocument(DocumentSpec *document) {
        if (document == nullptr) {
            qWarning() << "Core::DocumentSystem::removeDocument(): trying to remove null document";
            return false;
        }
        return removeDocument(document->id());
    }

    bool DocumentSystem::removeDocument(const QString &id) {
        Q_D(DocumentSystem);
        auto it = d->documents.find(id);
        if (it == d->documents.end()) {
            qWarning() << "Core::DocumentSystem::removeDocument(): document does not exist:" << id;
            return false;
        }

        auto document = it.value();
        document->setParent(nullptr);
        d->documents.erase(it);

        return true;
    }

    DocumentSpec *DocumentSystem::document(const QString &id) const {
        Q_D(const DocumentSystem);
        return d->documents.value(id, nullptr);
    }

    QList<DocumentSpec *> DocumentSystem::documents() const {
        Q_D(const DocumentSystem);
        return d->documents.values();
    }

    QStringList DocumentSystem::documentIds() const {
        Q_D(const DocumentSystem);
        return d->documents.keys();
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
            QString("%1(%2)").arg(QApplication::translate("DocumentSystem", "All Files"), QMOs::allFilesFilter());
        auto path2 = getOpenFileName(parent, filter, path);
        if (path2.isEmpty()) {
            return false;
        }
        return spec->open(path2);
    }

    bool DocumentSystem::openDirBrowse(DocumentSpec *spec, const QString &path, QWidget *parent) const {
        Q_D(const DocumentSystem);
        auto path2 = getExistingDirectory(parent, path);
        if (path2.isEmpty()) {
            return false;
        }
        return spec->open(path2);
    }

    bool DocumentSystem::saveFileBrowse(IDocument *iDoc, const QString &path, QWidget *parent) const {
        Q_D(const DocumentSystem);
        auto spec = iDoc->d_ptr->spec;
        if (!spec) {
            return false;
        }

        auto filter =
            spec->filter() + ";;" +
            QString("%1(%2)").arg(QApplication::translate("DocumentSystem", "All Files"), QMOs::allFilesFilter());
        auto path2 = getSaveFileName(parent, filter, path);
        if (path2.isEmpty()) {
            return false;
        }

        return spec->open(path2);
    }

    QString DocumentSystem::getOpenFileName(QWidget *parent, const QString &filters, const QString &path,
                                            QString *selectedFilter) const {
        Q_D(const DocumentSystem);
        return QFileDialog::getOpenFileName(parent, QApplication::translate("DocumentSystem", "Open File"),
                                            path.isEmpty() ? d->openFileLastVisitDir : path, filters, selectedFilter);
    }

    QStringList DocumentSystem::getOpenFileNames(QWidget *parent, const QString &filters, const QString &path,
                                                 QString *selectedFilter) const {
        Q_D(const DocumentSystem);
        return QFileDialog::getOpenFileNames(parent, QApplication::translate("DocumentSystem", "Open Files"),
                                             path.isEmpty() ? d->openFileLastVisitDir : path, filters, selectedFilter);
    }

    QString DocumentSystem::getExistingDirectory(QWidget *parent, const QString &path) const {
        Q_D(const DocumentSystem);
        return QFileDialog::getExistingDirectory(parent, QApplication::translate("DocumentSystem", "Open Directory"),
                                                 path.isEmpty() ? d->openDirLastVisitDir : path);
    }

    QString DocumentSystem::getSaveFileName(QWidget *parent, const QString &path, const QString &filter,
                                            QString *selectedFilter) const {
        Q_D(const DocumentSystem);
        return QFileDialog::getSaveFileName(parent, QApplication::translate("DocumentSystem", "Save File"),
                                            path.isEmpty() ? d->saveFileLastVisitDir : path, filter, selectedFilter);
    }

    QString DocumentSystem::fixFileName(const QString &fileName, DocumentSystem::FixMode fixmode) {
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

    DocumentSystem::DocumentSystem(DocumentSystemPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }
}
