#include "QCommandPalette_p.h"
#include "QCommandPalette.h"

QCommandPalettePrivate::QCommandPalettePrivate() {
}

QCommandPalettePrivate::~QCommandPalettePrivate() {
}

void QCommandPalettePrivate::init() {
    Q_Q(QCommandPalette);

    lineEdit = new QLineEdit();
    listWidget = new QListWidget();

    layout = new QVBoxLayout();
    layout->addWidget(lineEdit);
    layout->addWidget(listWidget);
    q->setLayout(layout);

    q->setFocusPolicy(Qt::ClickFocus);
    q->setFocusProxy(lineEdit);

    lineEdit->setFocusPolicy(Qt::ClickFocus);
    listWidget->setFocusPolicy(Qt::ClickFocus);

    lineEdit->installEventFilter(q);
    listWidget->installEventFilter(q);

    QObject::connect(lineEdit, &QLineEdit::textChanged, q, &QCommandPalette::_q_textChanged);
    QObject::connect(listWidget, &QListWidget::currentRowChanged, q,
                     &QCommandPalette::_q_currentRowChanged);
    QObject::connect(listWidget, &QListWidget::itemClicked, q, &QCommandPalette::_q_itemClicked);
}
