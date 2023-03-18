#ifndef IPROJECTWINDOW_H
#define IPROJECTWINDOW_H

#include "ICoreWindow.h"

namespace Core {

    namespace Internal {
        class IProjectWindowFactory;
    }

    class CORE_EXPORT IProjectWindow : public ICoreWindow {
        Q_OBJECT
    protected:
        explicit IProjectWindow(QObject *parent = nullptr);
        ~IProjectWindow();

    public:

    private:
        friend class Internal::IProjectWindowFactory;
    };

}

#endif // IPROJECTWINDOW_H
