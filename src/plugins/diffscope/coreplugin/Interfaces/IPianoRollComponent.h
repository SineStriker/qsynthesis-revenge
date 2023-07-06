#ifndef IPIANOROLLCOMPONENT_H
#define IPIANOROLLCOMPONENT_H

namespace Core {

    class IProjectWindow;

    class IPianoRollComponent {
    public:
        explicit IPianoRollComponent(IProjectWindow *iWin) : iWin(iWin) {
        }
        virtual ~IPianoRollComponent() {
        }

        virtual void initialize() = 0;
        virtual void extensionInitialized() = 0;

        inline IProjectWindow *windowHandle() const {
            return iWin;
        }

    protected:
        IProjectWindow *iWin;
    };

} // namespace Core

#endif // IPIANOROLLCOMPONENT_H
