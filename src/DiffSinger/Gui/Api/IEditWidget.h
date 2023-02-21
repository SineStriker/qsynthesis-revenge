#ifndef IEDITWIDGET_H
#define IEDITWIDGET_H

#include "DsCoreGlobal.h"
#include "DsGuiGlobal.h"
#include "QMNamePlugin.h"

DSAPI_BEGIN_NAMESPACE

class DSGUI_API IEditWidget : public QMNamePlugin {
    Q_OBJECT
public:
    explicit IEditWidget(QObject *parent = nullptr);
    ~IEditWidget();

public:
    virtual QWidget *createWidget() = 0;
};

DSAPI_END_NAMESPACE

#define DsApi_IEditWidget_IID "DiffScope.Plugin.EditWidget"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(DsApi::IEditWidget, DsApi_IEditWidget_IID)
QT_END_NAMESPACE


#endif // IEDITWIDGET_H
