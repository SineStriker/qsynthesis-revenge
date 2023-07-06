#ifndef SECTIONBAR_H
#define SECTIONBAR_H

#include <QFrame>

#include <MusicUtil/MusicTimeline.h>

#include "coreplugin/CoreGlobal.h"
#include "coreplugin/Interfaces/IPianoRollComponent.h"

namespace Core {

    class SectionBarPrivate;

    class CORE_EXPORT SectionBar : public QFrame, public IPianoRollComponent {
        Q_OBJECT
        Q_DECLARE_PRIVATE(SectionBar)
    public:
        explicit SectionBar(IProjectWindow *iWin, QWidget *parent = nullptr);
        ~SectionBar();

        void initialize() override;
        void extensionInitialized() override;

    public:
        const QsApi::MusicTimeline *timeline() const;

    protected:
        SectionBar(SectionBarPrivate &d, IProjectWindow *iWin, QWidget *parent = nullptr);

        QScopedPointer<SectionBarPrivate> d_ptr;
    };

} // namespace Core

#endif // SECTIONBAR_H
