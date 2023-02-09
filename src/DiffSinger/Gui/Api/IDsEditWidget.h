#ifndef IDSEDITWIDGET_H
#define IDSEDITWIDGET_H

#include "DsGuiGlobal.h"
#include "INamePlugin.h"

class DSGUI_API IDsEditWidget : public INamePlugin {
    Q_OBJECT
public:
    explicit IDsEditWidget(QObject *parent = nullptr);
    ~IDsEditWidget();

public:
    virtual QWidget *createWidget() = 0;
};

#define IDsEditWidget_IID "DiffSinger.Plugin.DsEditWidget"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IDsEditWidget, IDsEditWidget_IID)
QT_END_NAMESPACE

#endif // IDSEDITWIDGET_H
