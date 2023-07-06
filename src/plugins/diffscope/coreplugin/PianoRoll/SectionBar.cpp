#include "SectionBar.h"
#include "SectionBar_p.h"

namespace Core {

    SectionBarPrivate::SectionBarPrivate() {
        curWidth = 100;
    }

    SectionBarPrivate::~SectionBarPrivate() {
    }

    void SectionBarPrivate::init() {

    }

    void SectionBarPrivate::updateLayout() {
        Q_Q(SectionBar);

        //

        q->update();
    }

    SectionBar::SectionBar(IProjectWindow *iWin, QWidget *parent) : SectionBar(*new SectionBarPrivate(), iWin, parent) {
    }

    SectionBar::~SectionBar() {
    }

    void SectionBar::initialize() {
        Q_D(SectionBar);
    }

    void SectionBar::extensionInitialized() {
    }

    int SectionBar::currentWidth() const {
        Q_D(const SectionBar);
        return d->curWidth;
    }

    void SectionBar::setCurrentWidth(int width) {
        Q_D(SectionBar);
        d->curWidth = width;
        d->updateLayout();
    }

    void SectionBar::paintEvent(QPaintEvent *event) {
        QFrame::paintEvent(event);
    }

    SectionBar::SectionBar(SectionBarPrivate &d, IProjectWindow *iWin, QWidget *parent)
        : QFrame(parent), IPianoRollComponent(iWin), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

} // namespace Core
