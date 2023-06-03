#include "WindowCloseFilter_p.h"

#include "IWindow_p.h"

#include <QDragEnterEvent>
#include <QEvent>
#include <QMimeData>
#include <QTimer>

#include <QMSystem.h>

namespace Core {

    static void qtimer_single_shot_impl(QObject *obj, const char *member, const QString &s) {
        QMetaObject::invokeMethod(obj, member, Qt::DirectConnection, Q_ARG(QString, s));
    }

    WindowCloseFilter::WindowCloseFilter(IWindowPrivate *d, QWidget *w) : QObject(d), d(d), w(w) {
        w->installEventFilter(this);
    }

    WindowCloseFilter::~WindowCloseFilter() {
    }

    bool WindowCloseFilter::eventFilter(QObject *obj, QEvent *event) {
        if (obj == w) {
            switch (event->type()) {
                case QEvent::DragEnter:
                case QEvent::Drop: {
                    auto e = static_cast<QDropEvent *>(event);
                    const QMimeData *mime = e->mimeData();
                    if (mime->hasUrls()) {
                        QFileInfoList dirs;
                        QHash<QString, QFileInfoList> files;
                        for (const auto &url : mime->urls()) {
                            if (url.isLocalFile()) {
                                auto info = QFileInfo(url.toLocalFile());
                                if (info.isDir()) {
                                    dirs.append(info);
                                    continue;
                                }
                                if (info.isFile()) {
                                    auto suffix = info.completeSuffix().toLower();
                                    files[suffix].append(info);
                                    continue;
                                }
                            }
                        }

                        if (event->type() == QEvent::DragEnter) {
                            if ((!dirs.isEmpty() && d->dragFileHandlerMap.contains("/")) ||
                                (!files.isEmpty() && d->dragFileHandlerMap.contains("*"))) {
                                e->acceptProposedAction();
                            } else {
                                for (auto it = files.begin(); it != files.end(); ++it) {
                                    auto it2 = d->dragFileHandlerMap.find(it.key());
                                    if (it2 == d->dragFileHandlerMap.end()) {
                                        continue;
                                    }
                                    if (it2->max == 0 || it2->max >= it->size()) {
                                        e->acceptProposedAction();
                                        break;
                                    }
                                }
                            }
                        } else {
                            bool accept = false;

                            // Handle directories
                            if (!dirs.isEmpty()) {
                                auto it = d->dragFileHandlerMap.find("/");
                                if (it != d->dragFileHandlerMap.end()) {
                                    for (const auto &dir : qAsConst(dirs)) {
                                        qtimer_single_shot_impl(it->obj, it->member, dir.absoluteFilePath());
                                    }
                                    accept = true;
                                }
                            }

                            // Handle files
                            for (auto it = files.begin(); it != files.end();) {
                                auto it2 = d->dragFileHandlerMap.find(it.key());
                                if (it2 != d->dragFileHandlerMap.end()) {
                                    const auto &handler = *it2;
                                    if (handler.max == 0 || handler.max >= it->size()) {
                                        for (const auto &file : qAsConst(it.value())) {
                                            qtimer_single_shot_impl(handler.obj, handler.member,
                                                                    file.absoluteFilePath());
                                        }
                                        accept = true;
                                    }
                                    it = files.erase(it);
                                    continue;
                                }
                                ++it;
                            }

                            // Handle unhandled files
                            if (!files.isEmpty()) {
                                auto it = d->dragFileHandlerMap.find("*");
                                if (it != d->dragFileHandlerMap.end()) {
                                    for (const auto &fileList : qAsConst(files)) {
                                        for (const auto &file : fileList) {
                                            qtimer_single_shot_impl(it->obj, it->member, file.absoluteFilePath());
                                        }
                                    }
                                    accept = true;
                                }
                            }

                            if (accept) {
                                e->acceptProposedAction();
                            }
                        }
                    }

                    if (!e->isAccepted()) {
                        return true;
                    }
                    break;
                }

                case QEvent::Close:
                    if (event->isAccepted()) {
                        emit windowClosed(w);
                    }
                    break;

                default:
                    break;
            }
        }
        return QObject::eventFilter(obj, event);
    }

}