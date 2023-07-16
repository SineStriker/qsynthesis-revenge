#include "CSvgIconEngine.h"
#include "CSvgIconEngine_p.h"

#include <QAtomicInt>
#include <QDebug>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>

#include "private/QMetaTypeUtils.h"

QAtomicInt CSvgIconEnginePrivate::lastSerialNum;

CSvgIconEnginePrivate::CSvgIconEnginePrivate() {
    stepSerialNum();

    for (int i = 0; i < 8; ++i)
        svgContentIndexes[i] = i;

    currentState = QM::CS_Normal;
    noSaveCache = false;
    salt = "Dummy";
}

CSvgIconEnginePrivate::~CSvgIconEnginePrivate() {
}

QString CSvgIconEnginePrivate::pmcKey(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    //    return QLatin1String("$qtm_svgicon_") + QString::number(serialNum, 16).append(QLatin1Char('_')) +
    //           QString::number((((((qint64(size.width()) << 11) | size.height()) << 11) | mode) << 4) | state, 16);

    return QLatin1String("$qtm_svgicon_") + QString::number(serialNum, 16).append(QLatin1Char('_')) +
           QString::number(
               (((((qint64(size.width()) << 11) | size.height()) << 11) | mode) << 4) | state | (currentState << 1), 16)
               .append("_") +
           salt;
}

void CSvgIconEnginePrivate::stepSerialNum() {
    serialNum = lastSerialNum.fetchAndAddRelaxed(1);
}

bool CSvgIconEnginePrivate::tryLoad(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state) {
    QByteArray bytes = svgContents[svgContentIndexes[currentState]];
    if (bytes.isEmpty())
        return false;

    auto color = colorHints[currentState];
    if (color.isEmpty()) {
        color = currentColors[currentState];
        if (color.isEmpty()) {
            // ...
        } else if (color == "auto") {
            noSaveCache = true;
        } else {
            colorHints[currentState] = color;
            bytes.replace("currentColor", color.toUtf8());
        }
    } else {
        bytes.replace("currentColor", color.toUtf8());
    }
    renderer->load(bytes);

    return true;
}

QIcon::Mode CSvgIconEnginePrivate::loadDataForModeAndState(QSvgRenderer *renderer, QIcon::Mode mode,
                                                           QIcon::State state) {
    if (tryLoad(renderer, mode, state))
        return mode;

    return QIcon::Normal;
}

CSvgIconEngine::CSvgIconEngine() : d(new CSvgIconEnginePrivate) {
}

CSvgIconEngine::CSvgIconEngine(const CSvgIconEngine &other) : QIconEngine(other), d(new CSvgIconEnginePrivate()) {
    for (int i = 0; i < sizeof(d->svgContents) / sizeof(QString); ++i) {
        d->svgContents[i] = other.d->svgContents[i];
        d->svgContentIndexes[i] = other.d->svgContentIndexes[i];
    }

    for (int i = 0; i < sizeof(d->currentColors) / sizeof(QString); ++i) {
        d->currentColors[i] = other.d->currentColors[i];
    }
}

CSvgIconEngine::~CSvgIconEngine() {
}

QSize CSvgIconEngine::actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    QPixmap pm = pixmap(size, mode, state);
    if (pm.isNull())
        return {};

    return pm.size();
}

QPixmap CSvgIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    QPixmap pm;

    QString pmckey(d->pmcKey(size, mode, state));
    if (QPixmapCache::find(pmckey, &pm)) {
        return pm;
    }

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

    if (!pm.isNull() && !d->noSaveCache)
        QPixmapCache::insert(pmckey, pm);

    d->noSaveCache = false;

    return pm;
}

void CSvgIconEngine::addFile(const QString &fileName, const QSize &, QIcon::Mode mode, QIcon::State state) {
    // Do nothing
}

void CSvgIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) {
    QSize pixmapSize = rect.size();
    if (painter->device())
        pixmapSize *= painter->device()->devicePixelRatioF();
    painter->drawPixmap(rect, pixmap(pixmapSize, mode, state));
}

QString CSvgIconEngine::key() const {
    return "svg-ex";
}

QIconEngine *CSvgIconEngine::clone() const {
    return new CSvgIconEngine(*this);
}

bool CSvgIconEngine::read(QDataStream &in) {
    d.reset(new CSvgIconEnginePrivate());
    for (auto &item : d->svgContents) {
        in >> item;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
    }
    for (auto &item : d->svgContentIndexes) {
        in >> item;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
    }
    for (auto &item : d->currentColors) {
        in >> item;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
    }
    return true;
}

bool CSvgIconEngine::write(QDataStream &out) const {
    for (auto &item : d->svgContents) {
        out << item;
    }
    for (auto &item : d->svgContentIndexes) {
        out << item;
    }
    for (auto &item : d->currentColors) {
        out << item;
    }
    return true;
}

void CSvgIconEngine::virtual_hook(int id, void *data) {
    if (id == QIconEngine::IsNullHook) {
        *reinterpret_cast<bool *>(data) = false;
        return;
    }
    QIconEngine::virtual_hook(id, data);
}

QM::ClickState CSvgIconEngine::currentState() const {
    return d->currentState;
}

void CSvgIconEngine::setCurrentState(QM::ClickState state) {
    d->currentState = state;
}

QString CSvgIconEngine::salt() const {
    return d->salt;
}

void CSvgIconEngine::setSalt(const QString &salt) {
    d->salt = salt;
}

bool CSvgIconEngine::needColorHint() const {
    auto idx = d->currentState;
    return d->colorHints[idx].isEmpty() && d->currentColors[idx] == "auto";
}

void CSvgIconEngine::setColorHint(const QString &color) {
    auto idx = d->currentState;
    d->colorHints[idx] = color;
}

void CSvgIconEngine::setValues(QByteArray *dataList, QString *colorList) {
    for (int i = 0; i < 8; ++i) {
        const auto &data = dataList[i];
        d->svgContents[i] = data;
        if (data.isEmpty()) {
            QMetaTypeUtils::UpdateStateIndex(i, d->svgContentIndexes);
        } else {
            d->svgContentIndexes[i] = i;
        }
    }

    for (int i = 0; i < 8; ++i) {
        const auto &data = colorList[i];
        d->currentColors[i] = data;
    }

    d->stepSerialNum();
}
