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

        connect(scene, &CanvasScene::viewMoved, this, &SectionWidget::_q_viewMoved);
        connect(scene, &CanvasScene::viewResized, this, &SectionWidget::_q_viewResized);

        auto timeMgr = iWin->timeManager();
        connect(timeMgr, &MusicTimeManager::currentWidthChanged, this, &SectionWidget::_q_currentWidthChanged);
        connect(timeMgr, &MusicTimeManager::currentSnapChanged, this, &SectionWidget::_q_currentSnapChanged);

        setCurrentWidth(timeMgr->currentWidth());
        setCurrentSnap(timeMgr->currentSnap());
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

    void SectionWidget::_q_viewMoved(const QPointF &pos, const QPointF &oldPos) {
        setStartPos(double(view->valueX()) / currentWidth() * 480);
        update();
    }

    void SectionWidget::_q_viewResized(const QSizeF &size, const QSizeF &newSize) {
        setStartPos(double(view->valueX()) / currentWidth() * 480);
        update();
    }

    void SectionWidget::_q_currentWidthChanged(int w) {
        setCurrentWidth(w);
    }

    void SectionWidget::_q_currentSnapChanged(int s) {
        setCurrentSnap(s);
    }

}