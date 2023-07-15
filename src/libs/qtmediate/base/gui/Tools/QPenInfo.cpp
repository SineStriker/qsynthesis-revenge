#include "QPenInfo.h"
#include "private/QMetaTypeUtils.h"

#include <QDebug>

#include "QMCss.h"
#include "QPixelSize.h"

static const char QCssCustomValue_Pen_Line_None[] = "none";
static const char QCssCustomValue_Pen_Line_Solid[] = "solid";
static const char QCssCustomValue_Pen_Line_Dash[] = "dash";
static const char QCssCustomValue_Pen_Line_Dot[] = "dot";
static const char QCssCustomValue_Pen_Line_DashDot[] = "dashdot";
static const char QCssCustomValue_Pen_Line_DashDotDot[] = "dashdotdot";
static const char QCssCustomValue_Pen_Cap_Flat[] = "flat";
static const char QCssCustomValue_Pen_Cap_Square[] = "square";
static const char QCssCustomValue_Pen_Cap_Round[] = "round";
static const char QCssCustomValue_Pen_Join_Miter[] = "miter";
static const char QCssCustomValue_Pen_Join_Bevel[] = "bevel";
static const char QCssCustomValue_Pen_Join_Round[] = "round";

static Qt::PenStyle StringToLineStyle(const QString &str, Qt::PenStyle defaultValue) {
    Qt::PenStyle style = defaultValue;
    if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_None), Qt::CaseInsensitive)) {
        style = Qt::NoPen;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_Solid), Qt::CaseInsensitive)) {
        style = Qt::SolidLine;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_Dash), Qt::CaseInsensitive)) {
        style = Qt::DashLine;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_Dot), Qt::CaseInsensitive)) {
        style = Qt::DotLine;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_DashDot), Qt::CaseInsensitive)) {
        style = Qt::DashDotLine;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Line_DashDotDot), Qt::CaseInsensitive)) {
        style = Qt::DashDotDotLine;
    }
    return style;
}

static Qt::PenCapStyle StringToCapStyle(const QString &str, Qt::PenCapStyle defaultValue) {
    Qt::PenCapStyle style = defaultValue;
    if (!str.compare(QLatin1String(QCssCustomValue_Pen_Cap_Flat), Qt::CaseInsensitive)) {
        style = Qt::FlatCap;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Cap_Square), Qt::CaseInsensitive)) {
        style = Qt::SquareCap;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Cap_Round), Qt::CaseInsensitive)) {
        style = Qt::RoundCap;
    }
    return style;
}

static Qt::PenJoinStyle StringToJoinStyle(const QString &str, Qt::PenJoinStyle defaultValue) {
    Qt::PenJoinStyle style = defaultValue;
    if (!str.compare(QLatin1String(QCssCustomValue_Pen_Join_Miter), Qt::CaseInsensitive)) {
        style = Qt::MiterJoin;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Join_Bevel), Qt::CaseInsensitive)) {
        style = Qt::BevelJoin;
    } else if (!str.compare(QLatin1String(QCssCustomValue_Pen_Join_Round), Qt::CaseInsensitive)) {
        style = Qt::RoundJoin;
    }
    return style;
}

class QPenInfoData : public QSharedData {
public:
    QBrush brush[8];
    int brushIndexes[8];

    QPenInfoData() {
        QMetaTypeUtils::InitializeStateIndexes(brushIndexes);
    }
};

QPenInfo::QPenInfo() : d(new QPenInfoData()) {
}

QPenInfo::QPenInfo(Qt::PenStyle style) : QPenInfo() {
    setStyle(style);
}

QPenInfo::QPenInfo(const QColor &color) : QPenInfo() {
    setBrush(color);
}

QPenInfo::QPenInfo(const QBrush &brush, qreal width, Qt::PenStyle s, Qt::PenCapStyle c, Qt::PenJoinStyle j)
    : QPenInfo() {
    setBrush(brush);
    setWidthF(width);
    setStyle(s);
    setCapStyle(c);
    setJoinStyle(j);
}

QPenInfo::~QPenInfo() {
}

QPenInfo::QPenInfo(const QPenInfo &other) : QPen(other), d(other.d) {
}

QPenInfo::QPenInfo(QPenInfo &&other) noexcept : QPen(other), d(other.d) {
    other.d = nullptr;
}

QPenInfo &QPenInfo::operator=(const QPenInfo &other) {
    if (this == &other)
        return *this;
    QPen::operator=(other);
    d = other.d;
    return *this;
}

QPenInfo &QPenInfo::operator=(QPenInfo &&other) noexcept {
    QPen::operator=(other);
    qSwap(d, other.d);
    return *this;
}

QPen QPenInfo::toPen(QM::ClickState state) const {
    QPen pen = *this;
    if (state != QM::CS_Normal)
        pen.setBrush(d->brush[state]);
    return pen;
}

QBrush QPenInfo::brush(QM::ClickState state) const {
    int idx = d->brushIndexes[state];
    if (idx != QM::CS_Normal)
        return d->brush[idx];
    return QPen::brush();
}

void QPenInfo::setBrush(const QBrush &brush, QM::ClickState state) {
    if (state != QM::CS_Normal) {
        d->brush[state] = brush;
        d->brushIndexes[state] = state;
        QMetaTypeUtils::UpdateStateIndexes(d->brushIndexes);
        return;
    }
    QPen::setBrush(brush);
}

QColor QPenInfo::color(QM::ClickState state) const {
    return brush(state).color();
}

void QPenInfo::setColor(const QColor &color, QM::ClickState state) {
    setBrush(color, state);
}

void QPenInfo::setColors(const QList<QColor> &colors) {
    int sz = qMin(colors.size(), 8);
    for (int i = 0; i < sz; ++i) {
        auto state = static_cast<QM::ClickState>(i);
        if (state != QM::CS_Normal) {
            d->brush[state] = colors.at(state);
            d->brushIndexes[state] = state;
            continue;
        }
        QPen::setBrush(colors.at(state));
    }
    QMetaTypeUtils::UpdateStateIndexes(d->brushIndexes);
}

QPenInfo QPenInfo::fromStringList(const QStringList &stringList) {
    QMETATYPE_CHECK_FUNC(stringList, strData);

    auto args = QMetaTypeUtils::ParseFuncArgList(strData.trimmed(),
                                                 {
                                                     "color",
                                                     "width",
                                                     "style",
                                                     "cap",
                                                     "join",
                                                     "dashPattern",
                                                     "dashOffset",
                                                     "miterLimit",
                                                     "cosmetic",
                                                 },
                                                 {}, true);

    auto it = args.find("color");
    if (it == args.end())
        return {};

    QPenInfo res;

    QString colors[8];
    QMetaTypeUtils::ParseClickStateArgList(it.value().trimmed(), colors);
    for (int i = 0; i < 8; ++i) {
        res.d->brush[i] = QBrush(QMCss::CssStringToColor(colors[i]));
        res.d->brushIndexes[i] = i;
    }

    it = args.find("width");
    if (it != args.end()) {
        res.setWidthF(QPixelSize::fromString(it.value()));
    }

    it = args.find("style");
    if (it != args.end()) {
        res.setStyle(StringToLineStyle(it.value(), res.style()));
    }

    it = args.find("cap");
    if (it != args.end()) {
        res.setCapStyle(StringToCapStyle(it.value(), res.capStyle()));
    }

    it = args.find("join");
    if (it != args.end()) {
        res.setJoinStyle(StringToJoinStyle(it.value(), res.joinStyle()));
    }

    it = args.find("dashPattern");
    if (it != args.end()) {
        res.setDashPattern(QMetaTypeUtils::SplitStringToDoubleList(it.value()));
    }

    it = args.find("dashOffset");
    if (it != args.end()) {
        res.setDashOffset(QPixelSize::fromString(it.value()));
    }

    it = args.find("miterLimit");
    if (it != args.end()) {
        res.setMiterLimit(QPixelSize::fromString(it.value()));
    }

    it = args.find("cosmetic");
    if (it != args.end()) {
        res.setCosmetic(QMetaTypeUtils::StringToBool(it.value()));
    }

    return res;
}

const char *QPenInfo::metaFunctionName() {
    return "qpen";
}

QDebug operator<<(QDebug debug, const QPenInfo &info) {
    debug << info.toPen();
    return debug;
}