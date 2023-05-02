#include "HomeRecoveryWidget.h"

#include <QCheckBox>
#include <QDir>

#include <QMDecoratorV2.h>

#include "ICore.h"

namespace Core {

    HomeRecoveryWidget::HomeRecoveryWidget(QWidget *parent) : QFrame(parent) {
        listWidget = new QListWidget();
        for (const auto &rem : ICore::instance()->documentSystem()->remainingLogs()) {
            auto item = new QListWidgetItem();
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            item->setText(QDir::toNativeSeparators(rem.fileName()));
            item->setCheckState(Qt::Checked);
            listWidget->addItem(item);
        }

        emptyLabel = new QLabel();

        layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(listWidget);
        layout->addWidget(emptyLabel);
        layout->addStretch();

        setLayout(layout);

        if (listWidget->count() == 0) {
            listWidget->hide();
        } else {
            emptyLabel->hide();
        }

        qIDec->installLocale(this, _LOC(HomeRecoveryWidget, this));
    }

    HomeRecoveryWidget::~HomeRecoveryWidget() {
    }

    void HomeRecoveryWidget::reloadStrings() {
        emptyLabel->setText(tr("No data."));
    }

}