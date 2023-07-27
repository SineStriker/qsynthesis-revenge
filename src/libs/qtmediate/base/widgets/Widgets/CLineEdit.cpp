#include "CLineEdit.h"

#include "CMenu.h"
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QTimer>


CLineEdit::CLineEdit(QWidget *parent) : QLineEdit(parent) {
}

CLineEdit::CLineEdit(const QString &text, QWidget *parent) : QLineEdit(text, parent) {
}

CLineEdit::~CLineEdit() {
}

static bool isBlack(const QColor &color) {
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    return red == 0 && green == 0 && blue == 0;
}

bool CLineEdit::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::StyleChange: {
            auto palette = this->palette();
            if (isBlack(palette.brush(QPalette::Active, QPalette::PlaceholderText).color()) &&
                !isBlack(palette.brush(QPalette::Active, QPalette::Text).color())) {
                palette.setBrush(QPalette::Active, QPalette::PlaceholderText, {}); // Set colors twice
                setPalette(palette);
            }
            break;
        }
        case QEvent::KeyPress:
        case QEvent::ShortcutOverride: {
            auto keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();
            switch (key) {
                case Qt::Key_Return:
                case Qt::Key_Enter:
                case Qt::Key_Tab:
                case Qt::Key_Escape:
                    emit specialKeyPressed(key);
                    return true;
                    break;
                default: {
                    break;
                }
            }
        }
        default:
            break;
    }
    return QLineEdit::event(event);
}

void CLineEdit::contextMenuEvent(QContextMenuEvent *event) {
    // HACK: Hijack with our implementation
    if (QMenu *m = createStandardContextMenu()) {
        CMenu *menu = new CMenu(this);
        foreach (QAction *i, m->actions()) {
            i->setParent(menu);
            menu->addAction(i);
        }
        delete m;
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->popup(event->globalPos());
    }
}
