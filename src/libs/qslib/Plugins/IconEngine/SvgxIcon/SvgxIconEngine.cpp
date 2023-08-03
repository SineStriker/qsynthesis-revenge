#include "SvgxIconEngine.h"
#include "SvgxIconEngine_p.h"

#include <QAtomicInt>
#include <QDebug>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>

#include <private/qguiapplication_p.h>

#include <QMNamespace.h>
#include <private/QMGuiAppExtension_p.h>

#include <private/QMSvg_p.h>
#include <private/QMetaTypeUtils.h>

#include "Common.h"

QAtomicInt SvgxIconEnginePrivate::lastSerialNum;

SvgxIconEnginePrivate::SvgxIconEnginePrivate() {
    stepSerialNum();

    for (int i = 0; i < 8; ++i)
        contentIndexes[i] = i;

    currentState = QM::CS_Normal;

    salt = "0";
}

SvgxIconEnginePrivate::~SvgxIconEnginePrivate() {
}

QString SvgxIconEnginePrivate::pmcKey(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    //    return QLatin1String("$qtm_svgicon_") + QString::number(serialNum, 16).append(QLatin1Char('_')) +
    //           QString::number((((((qint64(size.width()) << 11) | size.height()) << 11) | mode) << 4) | state, 16);

    mode = QIcon::Normal;
    state = QIcon::Off;

    return QLatin1String("$qtm_svgxicon_") + QString::number(serialNum, 16).append(QLatin1Char('_')) +
           QString::number(
               (((((qint64(size.width()) << 11) | size.height()) << 11) | mode) << 4) | state | (currentState << 1), 16)
               .append("_") +
           QString::number(QMGuiAppExtensionPrivate::globalImageCacheSerialNum, 16).append(QLatin1Char('_')) +
           realColors[currentState];
}

void SvgxIconEnginePrivate::stepSerialNum() {
    serialNum = lastSerialNum.fetchAndAddRelaxed(1);
}

bool SvgxIconEnginePrivate::tryLoad(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state) {
    QByteArray bytes = svgContents[contentIndexes[currentState]];
    if (bytes.isEmpty())
        return false;

    auto color = realColors[currentState];
    if (!color.isEmpty()) {
        bytes.replace("currentColor", color.toUtf8());
    }
    renderer->load(bytes);

    return true;
}

QIcon::Mode SvgxIconEnginePrivate::loadDataForModeAndState(QSvgRenderer *renderer, QIcon::Mode mode,
                                                           QIcon::State state) {
    if (tryLoad(renderer, mode, state))
        return mode;

    return QIcon::Normal;
}

SvgxIconEngine::SvgxIconEngine() : d(new SvgxIconEnginePrivate) {
}

SvgxIconEngine::SvgxIconEngine(const SvgxIconEngine &other) : QIconEngine(other), d(new SvgxIconEnginePrivate()) {
    for (int i = 0; i < sizeof(d->svgContents) / sizeof(QString); ++i) {
        d->svgContents[i] = other.d->svgContents[i];
        d->contentIndexes[i] = other.d->contentIndexes[i];
    }

    for (int i = 0; i < sizeof(d->colors) / sizeof(QString); ++i) {
        d->colors[i] = other.d->colors[i];
    }
}

SvgxIconEngine::~SvgxIconEngine() {
}

QSize SvgxIconEngine::actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    QPixmap pm = pixmap(size, mode, state);
    if (pm.isNull())
        return {};

    return pm.size();
}

QPixmap SvgxIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    QPixmap pm;
    QString pmckey(d->pmcKey(size, mode, state));
    if (QPixmapCache::find(pmckey, &pm)) {
        return pm;
    }

    bool noSaveCache = d->realColors[d->currentState] == "auto";

    QSvgRenderer renderer;
    const QIcon::Mode loadmode = d->loadDataForModeAndState(&renderer, mode, state);
    if (!renderer.isValid())
        return pm;

    QSize actualSize = renderer.defaultSize();
    if (!actualSize.isNull())
        actualSize.scale(size, Qt::KeepAspectRatio);

    if (actualSize.isEmpty())
        return {};

    QImage img(actualSize, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x00000000);
    QPainter p(&img);
    renderer.render(&p);
    p.end();
    pm = QPixmap::fromImage(img);
    if (qobject_cast<QGuiApplication *>(QCoreApplication::instance())) {
        if (loadmode != mode && mode != QIcon::Normal) {
            const QPixmap generated = QGuiApplicationPrivate::instance()->applyQIconStyleHelper(mode, pm);
            if (!generated.isNull())
                pm = generated;
        }
    }

    if (!pm.isNull() && !noSaveCache)
        QPixmapCache::insert(pmckey, pm);

    return pm;
}

void SvgxIconEngine::addFile(const QString &fileName, const QSize &, QIcon::Mode mode, QIcon::State state) {
    // Do nothing
}

void SvgxIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) {
    QSize pixmapSize = rect.size();
    if (painter->device())
        pixmapSize *= painter->device()->devicePixelRatioF();
    painter->drawPixmap(rect, pixmap(pixmapSize, mode, state));
}

QString SvgxIconEngine::key() const {
    return "svgx";
}

QIconEngine *SvgxIconEngine::clone() const {
    return new SvgxIconEngine(*this);
}

bool SvgxIconEngine::read(QDataStream &in) {
    d.reset(new SvgxIconEnginePrivate());
    for (auto &item : d->svgContents) {
        in >> item;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
    }
    for (auto &item : d->contentIndexes) {
        in >> item;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
    }
    for (auto &item : d->colors) {
        in >> item;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
    }
    return true;
}

bool SvgxIconEngine::write(QDataStream &out) const {
    for (auto &item : d->svgContents) {
        out << item;
    }
    for (auto &item : d->contentIndexes) {
        out << item;
    }
    for (auto &item : d->colors) {
        out << item;
    }
    return true;
}

void SvgxIconEngine::virtual_hook(int id, void *data) {
    switch (id) {
        case QIconEngine::IsNullHook:
            *reinterpret_cast<bool *>(data) = false;
            return;

        case QMSvgPrivate::Create: {
            auto a = reinterpret_cast<void **>(data);
            auto &fileMap = *reinterpret_cast<const QMap<QM::ClickState, QString> *>(a[0]);
            auto &colorMap = *reinterpret_cast<const QMap<QM::ClickState, QString> *>(a[1]);

            // Handle file contents
            QByteArray svgContents[8];
            for (int i = 0; i < 8; ++i) {
                const auto &item = fileMap.value(static_cast<QM::ClickState>(i));
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
            for (int i = 0; i < 8; ++i) {
                auto it = colorMap.find(static_cast<QM::ClickState>(i));
                if (it == colorMap.end())
                    continue;
                colors[i] = it.value();
            }

            for (int i = 0; i < 8; ++i) {
                if (!colors[i].isEmpty())
                    continue;

                switch (i) {
                    case QM::CS_Hover:
                    case QM::CS_Disabled: {
                        colors[i] = colors[QM::CS_Normal];
                        break;
                    }
                    case QM::CS_Pressed: {
                        colors[i] = colors[QM::CS_Hover];
                        break;
                    }
                    case QM::CS_Normal_Checked: {
                        colors[i] = colors[QM::CS_Normal];
                        break;
                    }
                    case QM::CS_Hover_Checked:
                    case QM::CS_Disabled_Checked: {
                        colors[i] = colors[QM::CS_Normal_Checked];
                        break;
                    }
                    case QM::CS_Pressed_Checked: {
                        colors[i] = colors[QM::CS_Hover_Checked];
                        break;
                    }
                    default:
                        break;
                }
            }

            setValues(svgContents, colors);
            return;
        }

        case QMSvgPrivate::Update: {
            auto a = reinterpret_cast<void **>(data);
            auto &state = *reinterpret_cast<const QM::ClickState *>(a[0]);
            auto &salt = *reinterpret_cast<const QString *>(a[1]);

            setCurrentState(state);
            setSalt(salt);
            return;
        }

        case QMSvgPrivate::GetColor: {
            auto a = reinterpret_cast<void **>(data);
            auto &state = *reinterpret_cast<const QM::ClickState *>(a[0]);
            auto &res = *reinterpret_cast<QString *>(a[1]);

            res = stateColor(state);
            return;
        }

        case QMSvgPrivate::SetColor: {
            auto a = reinterpret_cast<void **>(data);
            auto &state = *reinterpret_cast<const QM::ClickState *>(a[0]);
            auto &color = *reinterpret_cast<const QString *>(a[1]);

            setStateColor(state, color);
            return;
        };

        default:
            break;
    }
    QIconEngine::virtual_hook(id, data);
}

QM::ClickState SvgxIconEngine::currentState() const {
    return d->currentState;
}

void SvgxIconEngine::setCurrentState(QM::ClickState state) {
    d->currentState = state;
}

QString SvgxIconEngine::stateColor(QM::ClickState state) const {
    return d->realColors[state];
}

void SvgxIconEngine::setStateColor(QM::ClickState state, const QString &color) {
    d->realColors[d->currentState] = color;
}

QString SvgxIconEngine::salt() const {
    return d->salt;
}

void SvgxIconEngine::setSalt(const QString &salt) {
    if (!salt.isEmpty() && salt != d->salt && d->colors[d->currentState] == "auto") {
        d->realColors[d->currentState] = "auto";
    }
    d->salt = salt;
}

void SvgxIconEngine::setValues(QByteArray *dataList, QString *colorList) {
    bool hasCurrentColor[8] = {};
    for (int i = 0; i < 8; ++i) {
        const auto &data = dataList[i];
        d->svgContents[i] = data;
        if (data.isEmpty()) {
            QMetaTypeUtils::UpdateStateIndex(i, d->contentIndexes);
        } else {
            d->contentIndexes[i] = i;
            hasCurrentColor[i] = data.contains("currentColor");
        }
    }

    for (int i = 0; i < 8; ++i) {
        auto data = colorList[i];
        if (hasCurrentColor[d->contentIndexes[i]]) {
            if (data.isEmpty()) {
                data = "auto";
            }
        } else {
            if (data == "auto") {
                data.clear();
            }
        }

        d->colors[i] = data;
        d->realColors[i] = data;
    }

    d->stepSerialNum();
}
