// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qbytearray.h>
#include <qfile.h>
#include <qiconengineplugin.h>
#include <qiodevice.h>
#include <qstringlist.h>

#include <QMCodec.h>

#include "Common.h"

class SvgxIconPlugin : public QIconEnginePlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QIconEngineFactoryInterface" FILE "plugin.json")
public:
    QIconEngine *create(const QString &fileName) override;
};

QIconEngine *SvgxIconPlugin::create(const QString &fileName) {
    auto engine = new SvgxIconEngine();
    if (fileName == ".svgx") {
        return engine;
    }

    // Decode
    QStringList valueList = QMetaTypeUtils::SplitStringByComma(fileName);
    if (!valueList.isEmpty() && valueList.back().trimmed().endsWith(".svgx"))
        valueList.pop_back();

    if (valueList.isEmpty())
        return engine;

    // Avoid copy file data because it's a waste of memory
    QString files[8];
    if (!QMetaTypeUtils::ParseClickStateArgList(valueList.front().trimmed(), files, false)) {
        return engine;
    }

    // Handle file contents
    QByteArray svgContents[8];
    for (int i = 0; i < 8; ++i) {
        const auto &item = QMCodec::unescape(files[i]);
        if (item.isEmpty())
            continue;

        if (item.startsWith("data:")) {
            svgContents[i] = parseDataUrl(fromDataUrl(item));
        } else {
            QFile file(item);
            if (file.open(QIODevice::ReadOnly)) {
                svgContents[i] = file.readAll();
            }
        }
    }

    // Handle colors
    QString colors[8];
    if (valueList.size() > 1) {
        QMetaTypeUtils::ParseClickStateArgList(valueList.at(1).trimmed(), colors);
    }
    //    else {
    //        for (auto &c : colors)
    //            c = "transparent";
    //    }

    engine->setValues(svgContents, colors);

    return engine;
}

#include "main.moc"
