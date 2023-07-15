#include "QRectInfo.h"
#include "private/QMetaTypeUtils.h"

#include <QColor>
#include <QDebug>

#include "QMCss.h"
#include "QPixelSize.h"

class QRectInfoData : public QSharedData {
public:
    QRectInfoData() {
        for (auto &item : nums)
            item = -1;
        for (auto &item : color)
            item = Qt::transparent;
        numSize = 0;
        radius = 0;

        QMetaTypeUtils::InitializeStateIndexes(colorIndexes);
    }

    int nums[4];
    int numSize;

    QColor color[8];
    int colorIndexes[8];

    int radius;
};

QRectInfo::QRectInfo() : d(new QRectInfoData()) {
}

QRectInfo::~QRectInfo() {
}

QRectInfo::QRectInfo(const QRectInfo &other) : d(other.d) {
}

QRectInfo::QRectInfo(QRectInfo &&other) noexcept : d(other.d) {
    other.d = nullptr;
}

QRectInfo &QRectInfo::operator=(const QRectInfo &other) {
    if (this == &other)
        return *this;
    d = other.d;
    return *this;
}

QRectInfo &QRectInfo::operator=(QRectInfo &&other) noexcept {
    qSwap(d, other.d);
    return *this;
}

QRect QRectInfo::rect() const {
    switch (d->numSize) {
        case 1:
            return {0, 0, d->nums[0], d->nums[0]};
            break;
        case 2:
            return {0, 0, d->nums[0], d->nums[1]};
            break;
        case 3:
            return {d->nums[0], d->nums[1], d->nums[2], d->nums[2]};
            break;
        case 4:
            return {d->nums[0], d->nums[1], d->nums[2], d->nums[3]};
            break;
        default:
            break;
    }
    return {};
}

void QRectInfo::setRect(const QRect &rect) {
    d->nums[0] = rect.x();
    d->nums[1] = rect.y();
    d->nums[2] = rect.width();
    d->nums[3] = rect.height();
    d->numSize = 4;
}

QMargins QRectInfo::margins() const {
    switch (d->numSize) {
        case 1:
            return {d->nums[0], d->nums[0], d->nums[0], d->nums[0]};
            break;
        case 2:
        case 3:
            return {d->nums[1], d->nums[0], d->nums[1], d->nums[0]};
            break;
        case 4:
            return {d->nums[0], d->nums[1], d->nums[2], d->nums[3]};
            break;
        default:
            break;
    }
    return {};
}

void QRectInfo::setMargins(const QMargins &margins) {
    d->nums[0] = margins.left();
    d->nums[1] = margins.top();
    d->nums[2] = margins.right();
    d->nums[3] = margins.bottom();
    d->numSize = 4;
}

QColor QRectInfo::color(QM::ClickState state) const {
    return d->color[d->colorIndexes[state]];
}

void QRectInfo::setColor(const QColor &color, QM::ClickState state) {
    d->color[state] = color;
    d->colorIndexes[state] = state;
    QMetaTypeUtils::UpdateStateIndexes(d->colorIndexes);
}

void QRectInfo::setColors(const QList<QColor> &colors) {
    int sz = qMin(colors.size(), 8);
    for (int i = 0; i < sz; ++i) {
        auto state = static_cast<QM::ClickState>(i);
        d->color[state] = colors.at(state);
        d->colorIndexes[state] = state;
    }
    QMetaTypeUtils::UpdateStateIndexes(d->colorIndexes);
}

int QRectInfo::radius() const {
    return d->radius;
}

void QRectInfo::setRadius(int r) {
    d->radius = r;
}

QRectInfo QRectInfo::fromStringList(const QStringList &stringList) {
    QMETATYPE_CHECK_FUNC(stringList, strData);

    auto args = QMetaTypeUtils::ParseFuncArgList(strData.trimmed(),
                                                 {
                                                     "color",
                                                     "numbers",
                                                     "radius",
                                                 },
                                                 {}, true);

    auto it = args.find("color");
    if (it == args.end())
        return {};

    QRectInfo res;

    QString colors[8];
    QMetaTypeUtils::ParseClickStateArgList(it.value().trimmed(), colors);
    for (int i = 0; i < 8; ++i) {
        res.d->color[i] = QMCss::CssStringToColor(colors[i]);
        res.d->colorIndexes[i] = i;
    }

    it = args.find("numbers");
    if (it != args.end()) {
        auto list = QMetaTypeUtils::SplitStringToIntList(it.value());
        auto sz = qMin(4, list.size());
        for (int i = 0; i < sz; ++i) {
            res.d->nums[i] = list.at(i);
        }
        res.d->numSize = sz;
    }

    it = args.find("radius");
    if (it != args.end()) {
        res.setRadius(QPixelSize::fromString(it.value()));
    }

    return res;
}

const char *QRectInfo::metaFunctionName() {
    return "qrect";
}

QDebug operator<<(QDebug debug, const QRectInfo &info) {
    QDebugStateSaver saver(debug);

    debug.nospace().noquote() << "QRectInfo(" << info.color().name() << ", " << info.rect() << ", " << info.radius()
                              << ")";
    return debug;
}