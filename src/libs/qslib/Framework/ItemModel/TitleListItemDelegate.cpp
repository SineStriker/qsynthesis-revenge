#include "TitleListItemDelegate.h"
#include "TitleListItemDelegate_p.h"

#include <QDir>
#include <QFileInfo>
#include <QMouseEvent>
#include <QPainter>

#include "QsFrameworkNamespace.h"

namespace QsApi {

    TitleListItemDelegatePrivate::TitleListItemDelegatePrivate() {
        m_backgroundType = QTypeFace(Qt::transparent, 1.0);
        m_underline = QTypeFace(Qt::lightGray, 1.0);

        m_dateBackType = QTypeFace(Qt::transparent, 0.0);
        m_dateBackType.setWeight(0);

        m_fileMargins = QMargins(5, 5, 5, 5);
        m_locMargins = QMargins(5, 5, 5, 5);
        m_dateMargins = QMargins(5, 5, 5, 5);

        m_iconMargins = QMargins(5, 5, 5, 5);
    }

    TitleListItemDelegatePrivate::~TitleListItemDelegatePrivate() {
    }

    void TitleListItemDelegatePrivate::init() {
    }

    TitleListItemDelegate::TitleListItemDelegate(QObject *parent)
        : TitleListItemDelegate(*new TitleListItemDelegatePrivate(), parent) {
    }

    TitleListItemDelegate::~TitleListItemDelegate() {
    }

    QSize TitleListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
        Q_D(const TitleListItemDelegate);

        QSize size = QStyledItemDelegate::sizeHint(option, index);
        int iconHeight =
            index.data(QsApi::IconSizeRole).toSize().height() + d->m_iconMargins.top() + d->m_iconMargins.bottom();
        int midHeight = QFontMetrics(d->m_fileType.font()).height() + QFontMetrics(d->m_locType.font()).height() +
                        d->m_fileMargins.top() + d->m_fileMargins.bottom() + d->m_locMargins.top() +
                        d->m_locMargins.bottom();
        int dateHeight =
            QFontMetrics(d->m_dateType.font()).height() + d->m_dateMargins.top() + d->m_dateMargins.bottom();
        int h = qMax(iconHeight, qMax(midHeight, dateHeight));
        h += d->m_margins.bottom() + d->m_margins.top();
        if (size.height() < h) {
            size.setHeight(h);
        }
        return size;
    }

    void TitleListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const {
        Q_D(const TitleListItemDelegate);

        QRect rect = option.rect;
        rect.adjust(d->m_margins.left(), d->m_margins.top(), -d->m_margins.right(), -d->m_margins.bottom());

        // Color
        QColor backgroundColor;
        QColor underlineColor;
        QColor fileColor;
        QColor locColor;
        QColor dateColor;
        QColor dateBackColor;
        if (option.state & QStyle::State_Selected) {
            backgroundColor = d->m_backgroundType.color3();
            underlineColor = d->m_underline.color3();
            fileColor = d->m_fileType.color3();
            locColor = d->m_locType.color3();
            dateColor = d->m_dateType.color3();
            dateBackColor = d->m_dateBackType.color3();
        } else if (option.state & QStyle::State_MouseOver) {
            backgroundColor = d->m_backgroundType.color2();
            underlineColor = d->m_underline.color2();
            fileColor = d->m_fileType.color2();
            locColor = d->m_locType.color2();
            dateColor = d->m_dateType.color2();
            dateBackColor = d->m_dateBackType.color2();
        } else {
            backgroundColor = d->m_backgroundType.color();
            underlineColor = d->m_underline.color();
            fileColor = d->m_fileType.color();
            locColor = d->m_locType.color();
            dateColor = d->m_dateType.color();
            dateBackColor = d->m_dateBackType.color();
        }

        // Fetch data
        QString filename = index.data(Qt::DisplayRole).toString();
        QString location = index.data(QsApi::SubtitleRole).toString();
        QString date = index.data(QsApi::DescriptionRole).toString();

        auto icon = index.data(Qt::DecorationRole).value<QIcon>();
        QSize iconSize = index.data(QsApi::IconSizeRole).toSize();

        // Calculate size
        QFont fileFont = d->m_fileType.font();
        QFont locFont = d->m_locType.font();
        QFont dateFont = d->m_dateType.font();

        //    fileFont.setStyleStrategy(QFont::PreferAntialias);
        //    locFont.setStyleStrategy(QFont::PreferAntialias);
        //    dateFont.setStyleStrategy(QFont::PreferAntialias);

        QFontMetrics fileFontM(fileFont);
        QFontMetrics locFontM(locFont);
        QFontMetrics dateFontM(dateFont);

        int fileFontHeight = fileFontM.height();
        int locFontHeight = locFontM.height();
        int dateFontHeight = dateFontM.height();

        int dateWidth = dateFontM.horizontalAdvance(date);

        int iconHeight =
            index.data(QsApi::IconSizeRole).toSize().height() + d->m_iconMargins.top() + d->m_iconMargins.bottom();
        int midHeight = fileFontHeight + d->m_fileMargins.top() + d->m_fileMargins.bottom() + locFontHeight +
                        d->m_locMargins.top() + d->m_locMargins.bottom();
        int dateHeight = dateFontHeight + d->m_dateMargins.top() + d->m_dateMargins.bottom();

        int realHeight = rect.height();

        QRect iconRect;
        iconRect.setTop(rect.top() + (realHeight - iconHeight) / 2 + d->m_iconMargins.top());
        iconRect.setLeft(d->m_iconMargins.left());
        iconRect.setSize(iconSize);

        int iconRight = iconRect.right() + d->m_iconMargins.right();
        int wordsWidth = rect.width() - iconRight;

        QRect dateRect;
        dateRect.setTop(rect.top() + (realHeight - dateHeight) / 2 + d->m_dateMargins.top());
        dateRect.setRight(rect.width() - d->m_dateMargins.right());
        dateRect.setLeft(
            qMax(iconRect.right() + wordsWidth / 2 + d->m_dateMargins.left(), dateRect.right() - dateWidth));
        dateRect.setHeight(dateFontHeight);

        int dateLeft = dateRect.left() - d->m_dateMargins.left();

        QRect fileRect;
        fileRect.setTop(rect.top() + (realHeight - midHeight) / 2 + d->m_fileMargins.top());
        fileRect.setLeft(iconRight + d->m_fileMargins.left());
        fileRect.setRight(dateLeft - d->m_fileMargins.right());
        fileRect.setHeight(fileFontHeight);

        int fileBottom = fileRect.bottom() + d->m_fileMargins.bottom();

        QRect locRect;
        locRect.setTop(fileBottom + d->m_locMargins.top());
        locRect.setLeft(iconRight + d->m_locMargins.left());
        locRect.setRight(dateLeft - d->m_locMargins.right());
        locRect.setHeight(locFontHeight);

        painter->setRenderHint(QPainter::Antialiasing);

        // Status
        painter->setPen(Qt::NoPen);
        painter->setBrush(backgroundColor);
        painter->drawRect(rect);

        // Icon
        if (!icon.isNull()) {
            painter->drawPixmap(iconRect, icon.pixmap(iconSize));
        }

        painter->setFont(dateFont);
        painter->setBrush(dateBackColor);

        // Last open time background
        {
            QRect dst = dateRect;
            auto r = d->m_dateBackType.weight();
            const QFontMetrics &fm = dateFontM;
            int adjust = (d->m_dateBackType.pointSize() - 1.0) * fm.height();
            dst.adjust(-adjust, -adjust, adjust, adjust);
            painter->drawRoundedRect(dst, r, r);
        }

        painter->setPen(dateColor);

        // Last open time
        {
            QString text = date;
            const QRect &dst = dateRect;
            const QFontMetrics &fm = dateFontM;

            int width = fm.horizontalAdvance(text);
            if (width > dst.width()) {
                text = fm.elidedText(text, Qt::ElideRight, dst.width());
            }
            painter->drawText(dst, Qt::AlignLeft | Qt::AlignVCenter, text);
        }

        painter->setFont(fileFont);
        painter->setPen(fileColor);

        // Filename
        {
            QString text = filename;
            const QRect &dst = fileRect;
            const QFontMetrics &fm = fileFontM;

            int width = fm.horizontalAdvance(text);
            if (width > dst.width()) {
                text = fm.elidedText(text, Qt::ElideRight, dst.width());
            }
            painter->drawText(dst, Qt::AlignLeft | Qt::AlignVCenter, text);
        }

        painter->setFont(locFont);
        painter->setPen(locColor);

        // Address
        {
            QString text = location;
            const QRect &dst = locRect;
            const QFontMetrics &fm = locFontM;

            int width = fm.horizontalAdvance(text);
            if (width > dst.width()) {
                text = fm.elidedText(text, Qt::ElideRight, dst.width());
            }
            painter->drawText(dst, Qt::AlignLeft | Qt::AlignVCenter, text);
        }

        painter->setPen(QPen(underlineColor, d->m_underline.pointSize()));
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    }

    QTypeFace TitleListItemDelegate::backgroundShape() const {
        Q_D(const TitleListItemDelegate);
        return d->m_backgroundType;
    }

    QTypeFace TitleListItemDelegate::underlineShape() const {
        Q_D(const TitleListItemDelegate);
        return d->m_underline;
    }

    QTypeFace TitleListItemDelegate::titleShape() const {
        Q_D(const TitleListItemDelegate);
        return d->m_fileType;
    }

    QTypeFace TitleListItemDelegate::subtitleShape() const {
        Q_D(const TitleListItemDelegate);
        return d->m_locType;
    }

    QTypeFace TitleListItemDelegate::descriptionShape() const {
        Q_D(const TitleListItemDelegate);
        return d->m_dateType;
    }

    QTypeFace TitleListItemDelegate::descriptionBackShape() const {
        Q_D(const TitleListItemDelegate);
        return d->m_dateBackType;
    }

    QMargins TitleListItemDelegate::titleMargins() const {
        Q_D(const TitleListItemDelegate);
        return d->m_fileMargins;
    }

    QMargins TitleListItemDelegate::subtitleMargins() const {
        Q_D(const TitleListItemDelegate);
        return d->m_locMargins;
    }

    QMargins TitleListItemDelegate::descriptionMargins() const {
        Q_D(const TitleListItemDelegate);
        return d->m_locMargins;
    }

    QMargins TitleListItemDelegate::iconMargins() const {
        Q_D(const TitleListItemDelegate);
        return d->m_iconMargins;
    }

    QMargins TitleListItemDelegate::margins() const {
        Q_D(const TitleListItemDelegate);
        return d->m_margins;
    }

    void TitleListItemDelegate::setBackgroundShape(const QTypeFace &shape) {
        Q_D(TitleListItemDelegate);
        d->m_backgroundType = shape;
    }

    void TitleListItemDelegate::setUnderlineShape(const QTypeFace &shape) {
        Q_D(TitleListItemDelegate);
        d->m_underline = shape;
    }

    void TitleListItemDelegate::setTitleShape(const QTypeFace &shape) {
        Q_D(TitleListItemDelegate);
        d->m_fileType = shape;
    }

    void TitleListItemDelegate::setSubtitleShape(const QTypeFace &shape) {
        Q_D(TitleListItemDelegate);
        d->m_locType = shape;
    }

    void TitleListItemDelegate::setDescriptionShape(const QTypeFace &shape) {
        Q_D(TitleListItemDelegate);
        d->m_dateType = shape;
    }

    void TitleListItemDelegate::setDescriptionBackShape(const QTypeFace &shape) {
        Q_D(TitleListItemDelegate);
        d->m_dateBackType = shape;
    }

    void TitleListItemDelegate::setTitleMargins(const QMargins &margins) {
        Q_D(TitleListItemDelegate);
        d->m_fileMargins = margins;
    }

    void TitleListItemDelegate::setSubtitleMargins(const QMargins &margins) {
        Q_D(TitleListItemDelegate);
        d->m_locMargins = margins;
    }

    void TitleListItemDelegate::setDescriptionMargins(const QMargins &margins) {
        Q_D(TitleListItemDelegate);
        d->m_dateMargins = margins;
    }

    void TitleListItemDelegate::setIconMargins(const QMargins &margins) {
        Q_D(TitleListItemDelegate);
        d->m_iconMargins = margins;
    }

    void TitleListItemDelegate::setMargins(const QMargins &margins) {
        Q_D(TitleListItemDelegate);
        d->m_margins = margins;
    }

    bool TitleListItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                            const QStyleOptionViewItem &option, const QModelIndex &index) {
        if (event->type() == QEvent::MouseButtonRelease) {
            auto e = static_cast<QMouseEvent *>(event);

            if (e->button() == Qt::RightButton) {
                emit clicked(index, Qt::RightButton);
            } else if (e->button() == Qt::LeftButton) {
                emit clicked(index, Qt::LeftButton);
            }
        }

        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    TitleListItemDelegate::TitleListItemDelegate(TitleListItemDelegatePrivate &d, QObject *parent)
        : QStyledItemDelegate(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
