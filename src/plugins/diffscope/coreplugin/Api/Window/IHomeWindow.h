#ifndef IHOMEWINDOW_H
#define IHOMEWINDOW_H

#include "ICoreWindow.h"

namespace Core {

    namespace Internal {
        class IHomeWindowFactory;
    }

    class CORE_EXPORT IHomeWindow : public ICoreWindow {
        Q_OBJECT
    protected:
        explicit IHomeWindow(QObject *parent = nullptr);
        ~IHomeWindow();

    public:
    private:
        friend class Internal::IHomeWindowFactory;
    };

}


#endif // IHOMEWINDOW_H
