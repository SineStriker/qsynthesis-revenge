/*
 *  Copyright (C) 2009 Aleksey Palazhchenko
 *  Copyright (C) 2014 Sergey Shambir
 *  Copyright (C) 2016 Alexander Makarov
 *
 * This file is a part of Breakpad-qt library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

#ifndef QBREAKPADHANDLER_H
#define QBREAKPADHANDLER_H

#include "singletone/singleton.h"
#include <QString>
#include <QUrl>


#include "QBreakpadGlobal.h"

namespace google_breakpad {
    class ExceptionHandler;
    class MinidumpDescriptor;
}

class QBreakpadHandlerPrivate;

class QBREAKPAD_API QBreakpadHandler : public QObject {
    Q_OBJECT
public:
    static QString version();

    QBreakpadHandler();
    ~QBreakpadHandler();

    QString uploadUrl() const;
    QString dumpPath() const;
    QStringList dumpFileList() const;

    void setDumpPath(const QString &path);
    void setUploadUrl(const QUrl &url);

public slots:
    void sendDumps();

public:
    typedef void (*HandlerFunc)();

    static HandlerFunc UniqueExtraHandler;

private:
    QBreakpadHandlerPrivate *d;
};
#define QBreakpadInstance Singleton<QBreakpadHandler>::instance()

#endif // QBREAKPADHANDLER_H
