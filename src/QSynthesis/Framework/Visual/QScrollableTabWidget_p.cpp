#include "QScrollableTabWidget_p.h"
#include "QScrollableTabWidget.h"

QScrollableTabWidgetPrivate::QScrollableTabWidgetPrivate() {
}

QScrollableTabWidgetPrivate::~QScrollableTabWidgetPrivate() {
}

void QScrollableTabWidgetPrivate::init() {
    Q_Q(QScrollableTabWidget);
    q->setAttribute(Qt::WA_StyledBackground);
    q->setAcceptDrops(true);

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    q->setLayout(mainLayout);

    barLayout = new QHBoxLayout();
    barLayout->setMargin(0);
    barLayout->setSpacing(0);
    mainLayout->addLayout(barLayout);

    // Init Stack
    stack = new QStackedWidget();
    stack->setObjectName(QLatin1String("stack"));
    stack->setLineWidth(0);
    stack->setSizePolicy(
        QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::TabWidget));
    QObject::connect(stack, &QStackedWidget::widgetRemoved, q, &QScrollableTabWidget::_q_removeTab);
    mainLayout->addWidget(stack);

    // Init Tab Bar
    tabBar = nullptr;
    auto bar = new QScrollableTabBar();
    bar->setObjectName("tab-bar");
    q->setTabBar(bar);
}
