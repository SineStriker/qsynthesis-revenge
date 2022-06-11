#include "CWindowBarTitleLabel.h"
#include "private/qlabel_p.h"

CWindowBarTitleLabel::CWindowBarTitleLabel(QWidget *parent) : QLabel(parent) {
    textDirty = 0;
}

CWindowBarTitleLabel::CWindowBarTitleLabel(const QString &text, QWidget *parent) : QLabel(parent) {
    textDirty = 0;
    setText(text);
}

CWindowBarTitleLabel::~CWindowBarTitleLabel() {
}

void CWindowBarTitleLabel::setText(const QString &text) {
    QLabel::setText(text);
    textDirty = 1;
}

bool CWindowBarTitleLabel::validHints() const {
    auto d = static_cast<QLabelPrivate *>(d_ptr.data());
    return d->valid_hints;
}

void CWindowBarTitleLabel::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    if (textDirty != 0) {
        if (validHints()) {
            textDirty = 0;
            auto w = parentWidget();
            if (w) {
                w->update();
            }
        }
    }
}
