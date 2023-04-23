#ifndef FULLICONBUTTON_H
#define FULLICONBUTTON_H

#include "Widgets/CPushButton.h"

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class QSFRAMEWORK_API FullIconButton : public CPushButton {
        Q_OBJECT
        Q_PROPERTY_DECLARE(QSize, iconMargin, IconMargin)
    public:
        explicit FullIconButton(QWidget *parent = nullptr);
        ~FullIconButton();

    protected:
        void paintEvent(QPaintEvent *event) override;
    };

}

#endif // FULLICONBUTTON_H
