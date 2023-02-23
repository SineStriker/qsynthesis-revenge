#include "QMrqFile.h"

#include <QFile>

#include <mrq.h>

/*
 *
 *  Private Structure
 *
 */

static int getMrqEntry(const wchar_t *wfilename, mrq_entry *entry, void *param) {
    QMrqFile::Entry e;
    e.name = QString::fromStdWString(wfilename);
    e.f0.reserve(entry->nf0);
    for (int i = 0; i < entry->nf0; ++i) {
        e.f0.append(entry->f0[i]);
    }

    e.nhop = entry->nhop;
    e.fs = entry->fs;

    e.timestamp = entry->timestamp;
    e.modified = !entry->modified;

    QMrqFile *p = (QMrqFile *) param;
    p->entries.append(e);
    return 1;
}

/*
 *
 *  Public Class
 *
 */

QMrqFile::QMrqFile() {
    reset();
}

QMrqFile::~QMrqFile() {
}

bool QMrqFile::load(const QString &filename) {
    // Open file
    FILE *f = mrq_open(filename.toStdWString().data(), L"r");
    if (!f) {
        return false;
    }

    // Get version
    int ver = mrq_get_version(f);
    if (ver < 0) {
        goto failed;
    }
    version = ver;

    // Enumerate all entries
    if (mrq_enumerate(f, getMrqEntry, this) != 1) {
        goto failed;
    }

    // Close file
    fclose(f);
    return true;

failed:
    fclose(f);
    reset();
    return false;
}

bool QMrqFile::save(const QString &filename) {
    // Remove first
    if (!QFile::remove(filename)) {
        return false;
    }

    FILE *f = mrq_open(filename.toStdWString().data(), L"w");
    if (!f) {
        return false;
    }

    /*
     *  Use current version of mrq library.
     */

    bool success = true;
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        mrq_entry e;
        e.nhop = it->nhop;
        e.fs = it->fs;

        e.f0 = new double[it->f0.size()];
        std::copy(it->f0.begin(), it->f0.end(), e.f0);
        e.nf0 = it->f0.size();
        e.timestamp = it->timestamp;
        e.modified = !it->modified;

        success = mrq_write_entry(f, it->name.toStdWString().data(), &e) == 1;
        delete e.f0;

        if (!success) {
            break;
        }
    }

    fclose(f);

    if (!success) {
        return false;
    }

    // Defrag
    if (mrq_defragment(filename.toStdWString().data()) != 1) {
        return false;
    }
    return true;
}

void QMrqFile::reset() {
    version = 0;
    entries.clear();
}
