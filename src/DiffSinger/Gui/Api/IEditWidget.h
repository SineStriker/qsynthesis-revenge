#ifndef IEDITWIDGET_H
#define IEDITWIDGET_H

#include "DsGuiGlobal.h"
#include "INamePlugin.h"

class DSGUI_API IEditWidget : public INamePlugin {
    Q_OBJECT
public:
    explicit IEditWidget(QObject *parent = nullptr);
    ~IEditWidget();

public:
    virtual QWidget *createWidget() = 0;
};

#define IEditWidget_IID "DiffScope.Plugin.EditWidget"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IEditWidget, IEditWidget_IID)
QT_END_NAMESPACE


#endif // IEDITWIDGET_H