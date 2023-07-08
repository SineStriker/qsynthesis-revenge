#include "SectionWidget.h"

#include "PianoRoll/CanvasScene.h"
#include "PianoRoll/SectionBar_p.h"

namespace Core::Internal {

    SectionWidget::SectionWidget(IProjectWindow *iWin, QWidget *parent) : SectionBar(iWin, parent) {
        m_preferredHeight = 30;
        view = nullptr;
        scene = nullptr;
    }

    SectionWidget::~SectionWidget() {
    }

    void SectionWidget::initialize() {
        SectionBar::initialize();

        view = iWin->pianoRoll()->canvasView();
        scene = view->scene();
        scene->installEventFilter(this);
    }

    void SectionWidget::extensionInitialized() {
        SectionBar::extensionInitialized();
    }

    QPixelSize SectionWidget::preferredHeight() const {
        return m_preferredHeight;
    }

    void SectionWidget::setPreferredHeight(const QPixelSize &height) {
        m_preferredHeight = height;
        updateGeometry();
    }
    QSize SectionWidget::sizeHint() const {
        return {QFrame::sizeHint().width(), m_preferredHeight};
    }

    void SectionWidget::paintEvent(QPaintEvent *event) {
        SectionBar::paintEvent(event);

        // Paint
    }

    bool SectionWidget::eventFilter(QObject *obj, QEvent *event) {
        if (obj == scene) {
            switch (event->type()) {
                case QEvent::GraphicsSceneMove: {
                    auto e = static_cast<QGraphicsSceneMoveEvent *>(event);
                    if (e->newPos().x() != e->oldPos().x())
                        setStartPos(double(static_cast<CanvasView *>(view)->valueX()) / currentWidth() * 480);
                    break;
                }
                default:
                    break;
            }
        }
        return QObject::eventFilter(obj, event);
    }

}