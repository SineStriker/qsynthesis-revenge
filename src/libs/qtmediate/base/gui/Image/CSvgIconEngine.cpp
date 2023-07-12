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

QAtomicInt CSvgIconEnginePrivate::lastSerialNum;

CSvgIconEnginePrivate::CSvgIconEnginePrivate() {
    stepSerialNum();
}

CSvgIconEnginePrivate::~CSvgIconEnginePrivate() {
}

QString CSvgIconEnginePrivate::pmcKey(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    return QLatin1String("$qt_svgicon_") + QString::number(serialNum, 16).append(QLatin1Char('_')) +
           QString::number((((((qint64(size.width()) << 11) | size.height()) << 11) | mode) << 4) | state, 16);
}

void CSvgIconEnginePrivate::stepSerialNum() {
    serialNum = lastSerialNum.fetchAndAddRelaxed(1);
}

bool CSvgIconEnginePrivate::tryLoad(QSvgRenderer *renderer, QIcon::Mode mode, QIcon::State state) {
    QString svgFile = svgFiles.value(hashKey(mode, state));
    if (!svgFile.isEmpty()) {
        QFile file(svgFile);
        QByteArray data;
        if (file.open(QIODevice::ReadOnly)) {
            data = file.readAll();
            if (!currentColor.isEmpty()) {
                data.replace("currentColor", currentColor.toUtf8());
            }
            renderer->load(data);
            return true;
        }
    }
    return false;
}

QIcon::Mode CSvgIconEnginePrivate::loadDataForModeAndState(QSvgRenderer *renderer, QIcon::Mode mode,
                                                           QIcon::State state) {
    if (tryLoad(renderer, mode, state))
        return mode;

    const QIcon::State oppositeState = (state == QIcon::On) ? QIcon::Off : QIcon::On;
    if (mode == QIcon::Disabled || mode == QIcon::Selected) {
        const QIcon::Mode oppositeMode = (mode == QIcon::Disabled) ? QIcon::Selected : QIcon::Disabled;
        if (tryLoad(renderer, QIcon::Normal, state))
            return QIcon::Normal;
        if (tryLoad(renderer, QIcon::Active, state))
            return QIcon::Active;
        if (tryLoad(renderer, mode, oppositeState))
            return mode;
        if (tryLoad(renderer, QIcon::Normal, oppositeState))
            return QIcon::Normal;
        if (tryLoad(renderer, QIcon::Active, oppositeState))
            return QIcon::Active;
        if (tryLoad(renderer, oppositeMode, state))
            return oppositeMode;
        if (tryLoad(renderer, oppositeMode, oppositeState))
            return oppositeMode;
    } else {
        const QIcon::Mode oppositeMode = (mode == QIcon::Normal) ? QIcon::Active : QIcon::Normal;
        if (tryLoad(renderer, oppositeMode, state))
            return oppositeMode;
        if (tryLoad(renderer, mode, oppositeState))
            return mode;
        if (tryLoad(renderer, oppositeMode, oppositeState))
            return oppositeMode;
        if (tryLoad(renderer, QIcon::Disabled, state))
            return QIcon::Disabled;
        if (tryLoad(renderer, QIcon::Selected, state))
            return QIcon::Selected;
        if (tryLoad(renderer, QIcon::Disabled, oppositeState))
            return QIcon::Disabled;
        if (tryLoad(renderer, QIcon::Selected, oppositeState))
            return QIcon::Selected;
    }
    return QIcon::Normal;
}

CSvgIconEngine::CSvgIconEngine() : d(new CSvgIconEnginePrivate) {
}

CSvgIconEngine::CSvgIconEngine(const CSvgIconEngine &other) : QIconEngine(other), d(new CSvgIconEnginePrivate()) {
    d->svgFiles = other.d->svgFiles;
    d->currentColor = other.d->currentColor;
}

CSvgIconEngine::CSvgIconEngine(const QString &currentColor) : d(new CSvgIconEnginePrivate) {
    setCurrentColor(currentColor);
}

CSvgIconEngine::~CSvgIconEngine() {
}

QSize CSvgIconEngine::actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    QPixmap pm = pixmap(size, mode, state);
    if (pm.isNull())
        return QSize();
    return pm.size();
}


QPixmap CSvgIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    QPixmap pm;

    QString pmckey(d->pmcKey(size, mode, state));
    if (QPixmapCache::find(pmckey, &pm))
        return pm;

    QSvgRenderer renderer;
    const QIcon::Mode loadmode = d->loadDataForModeAndState(&renderer, mode, state);
    if (!renderer.isValid())
        return pm;

    QSize actualSize = renderer.defaultSize();
    if (!actualSize.isNull())
        actualSize.scale(size, Qt::KeepAspectRatio);

    if (actualSize.isEmpty())
        return QPixmap();

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

    if (!pm.isNull())
        QPixmapCache::insert(pmckey, pm);

    return pm;
}

enum FileType { OtherFile, SvgFile, CompressedSvgFile };

static FileType fileType(const QFileInfo &fi) {
    const QString &abs = fi.absoluteFilePath();
    if (abs.endsWith(QLatin1String(".svg"), Qt::CaseInsensitive))
        return SvgFile;
    if (abs.endsWith(QLatin1String(".svgz"), Qt::CaseInsensitive) ||
        abs.endsWith(QLatin1String(".svg.gz"), Qt::CaseInsensitive)) {
        return CompressedSvgFile;
    }
#ifndef QT_NO_MIMETYPE
    const QString &mimeTypeName = QMimeDatabase().mimeTypeForFile(fi).name();
    if (mimeTypeName == QLatin1String("image/svg+xml"))
        return SvgFile;
    if (mimeTypeName == QLatin1String("image/svg+xml-compressed"))
        return CompressedSvgFile;
#endif // !QT_NO_MIMETYPE
    return OtherFile;
}

void CSvgIconEngine::addFile(const QString &fileName, const QSize &, QIcon::Mode mode, QIcon::State state) {
    if (!fileName.isEmpty()) {
        const QFileInfo fi(fileName);
        const QString &abs = fi.absoluteFilePath();
        const FileType &type = fileType(fi);
#ifndef QT_NO_COMPRESS
        if (type == SvgFile || type == CompressedSvgFile) {
#else
        if (type == SvgFile) {
#endif
            QSvgRenderer renderer(abs);
            if (renderer.isValid()) {
                d->stepSerialNum();
                d->svgFiles.insert(d->hashKey(mode, state), abs);
            }
        } else if (type == OtherFile) {
            QPixmap pm(abs);
            if (!pm.isNull())
                addPixmap(pm, mode, state);
        }
    }
}

void CSvgIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) {
    QSize pixmapSize = rect.size();
    if (painter->device())
        pixmapSize *= painter->device()->devicePixelRatioF();
    painter->drawPixmap(rect, pixmap(pixmapSize, mode, state));
}

QString CSvgIconEngine::key() const {
    return QLatin1String("svg");
}

QIconEngine *CSvgIconEngine::clone() const {
    return new CSvgIconEngine(*this);
}


bool CSvgIconEngine::read(QDataStream &in) {
    d = new CSvgIconEnginePrivate;
    in >> d->svgFiles;
    return true;
}


bool CSvgIconEngine::write(QDataStream &out) const {
    out << d->svgFiles;
    return true;
}

void CSvgIconEngine::virtual_hook(int id, void *data) {
    if (id == QIconEngine::IsNullHook) {
        *reinterpret_cast<bool *>(data) = false;
        return;
    }
    QIconEngine::virtual_hook(id, data);
}

QString CSvgIconEngine::currentColor() const {
    return d->currentColor;
}

void CSvgIconEngine::setCurrentColor(const QString &currentColor) {
    d->currentColor = currentColor;
}
