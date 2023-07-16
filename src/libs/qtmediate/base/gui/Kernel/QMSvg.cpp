#include "QMSvg.h"

#include <QIconEngine>
#include <QMap>

#include <private/qicon_p.h>

#include "private/QMSvg_p.h"

namespace QMSvg {

    static inline QIconEngine *get_engine(QIcon &icon) {
        auto d = icon.data_ptr();
        if (!d)
            return nullptr;

        auto engine = d->engine;
        if (!engine || engine->key() != "svgx") {
            return nullptr;
        }

        return engine;
    }

    QIcon create(const QMap<QM::ClickState, QString> &fileMap, const QMap<QM::ClickState, QString> &colorMap) {
        QIcon icon(".svgx");
        auto engine = get_engine(icon);

        const void *a[2] = {
            &fileMap,
            &colorMap,
        };
        engine->virtual_hook(QMSvgPrivate::Create, a);
        return icon;
    }

    QIcon create(const QString &file, const QString &checkedFile, const QString &color) {
        if (checkedFile.isEmpty()) {
            return create(
                {
                    {
                     QM::CS_Normal,
                     file, //
                    },
            },
                {
                    {
                        QM::CS_Normal,
                        color,
                    },
                });
        }
        return create(
            {
                {
                 QM::CS_Normal,
                 file,                  //
                },
                {
                 QM::CS_Normal_Checked,
                 checkedFile,                      //
                },
        },
            {
                {
                    QM::CS_Normal,
                    color,
                },
            });
    }

    bool update(QIcon *icon, QM::ClickState state, const QString &salt) {
        auto engine = get_engine(*icon);
        if (!engine)
            return false;

        const void *a[2] = {
            &state,
            &salt,
        };
        engine->virtual_hook(QMSvgPrivate::Update, a);
        return true;
    }

    QString getColor(QIcon *icon, QM::ClickState state) {
        auto engine = get_engine(*icon);
        if (!engine)
            return {};

        QString res;
        const void *a[2] = {
            &state,
            &res,
        };
        engine->virtual_hook(QMSvgPrivate::GetColor, a);
        return res;
    }

    void setColor(QIcon *icon, QM::ClickState state, const QString &color) {
        auto engine = get_engine(*icon);
        if (!engine)
            return;

        const void *a[2] = {
            &state,
            &color,
        };
        engine->virtual_hook(QMSvgPrivate::SetColor, a);
    }

}