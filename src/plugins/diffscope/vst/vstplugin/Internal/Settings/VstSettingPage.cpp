//
// Created by Crs_1 on 2023/8/13.
//

#include "VstSettingPage.h"
#include "CoreApi/ILoader.h"
#include "VstHelper.h"

#include <QApplication>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QTabWidget>

namespace Vst::Internal {

    VstSettingPage::VstSettingPage(QObject *parent) : Core::ISettingPage("vstclient.Vst", parent) {
        setTitle([]{return "VST";});
        setDescription([]{return tr("Connect %1 to VST host.").arg(QApplication::applicationName());});
    }
    VstSettingPage::~VstSettingPage() {
    }

    QString VstSettingPage::sortKeyword() const {
        return "VST";
    }
    bool VstSettingPage::matches(const QString &word) const {
        return ISettingPage::matches(word);
    }
    QWidget *VstSettingPage::widget() {
        if(m_widget) return m_widget;
        auto mainWidget = new QWidget;
        auto mainLayout = new QVBoxLayout;
        mainWidget->setLayout(mainLayout);

        // section: connection and stats
        auto infoTabWidget = new QTabWidget;

        m_connectionInfoList = new QTreeWidget;
        m_connectionInfoList->setColumnCount(2);
        m_connectionInfoList->setHeaderHidden(true);
        m_connectionInfoList->addTopLevelItems({
            m_connEnabled = new QTreeWidgetItem({tr("Enabled")}),
            m_connConnected = new QTreeWidgetItem({tr("Connected")}),
            m_connHost = new QTreeWidgetItem({tr("Host")}),
            m_connIsProcessing = new QTreeWidgetItem({tr("Processing")}),
            m_connSampleRate = new QTreeWidgetItem({tr("Sample rate")}),
            m_connChannelCount = new QTreeWidgetItem({tr("Channel count")}),
            m_connBufferSize = new QTreeWidgetItem({tr("Buffer size")}),
        });
        m_connectionInfoList->resizeColumnToContents(0);
        m_connEnabled->setFlags(m_connEnabled->flags() & ~Qt::ItemIsSelectable);
        m_connConnected->setFlags(m_connEnabled->flags());
        m_connHost->setFlags(m_connEnabled->flags());
        m_connIsProcessing->setFlags(m_connEnabled->flags());
        m_connSampleRate->setFlags(m_connEnabled->flags());
        m_connChannelCount->setFlags(m_connEnabled->flags());
        m_connBufferSize->setFlags(m_connEnabled->flags());

        m_statisticsInfoList = new QTreeWidget;
        auto statisticsLayout = new QVBoxLayout;
        statisticsLayout->addWidget(m_statisticsInfoList);
        m_statisticsInfoList->setColumnCount(2);
        m_statisticsInfoList->setHeaderHidden(true);
        m_statisticsInfoList->addTopLevelItems({
            new QTreeWidgetItem({tr("Current")}),
            new QTreeWidgetItem({tr("Total")}),
        });
        m_statisticsInfoList->topLevelItem(0)->setFlags(m_connEnabled->flags());
        m_statisticsInfoList->topLevelItem(0)->addChildren({
            m_statBufferCount = new QTreeWidgetItem({tr("Buffers processed")}),
            m_statSampleCount = new QTreeWidgetItem({tr("Samples processed")}),
        });
        m_statisticsInfoList->topLevelItem(1)->setFlags(m_connEnabled->flags());
        m_statisticsInfoList->topLevelItem(1)->addChildren({
            m_statTotalBufferCount = new QTreeWidgetItem({tr("Buffers processed")}),
            m_statTotalSampleCount = new QTreeWidgetItem({tr("Samples processed")}),
        });
        m_statBufferCount->setFlags(m_connEnabled->flags());
        m_statSampleCount->setFlags(m_connEnabled->flags());
        m_statTotalBufferCount->setFlags(m_connEnabled->flags());
        m_statTotalSampleCount->setFlags(m_connEnabled->flags());
        m_statisticsInfoList->expandAll();
        m_statisticsInfoList->resizeColumnToContents(0);

        infoTabWidget->addTab(m_connectionInfoList, "Connection Status");
        infoTabWidget->addTab(m_statisticsInfoList, "Statistics");
        mainLayout->addWidget(infoTabWidget);


        // section: advanced settings
        auto advancedSettingsGroupBox =  new QGroupBox(tr("Advanced"));
        auto formLayout = new QFormLayout;
        advancedSettingsGroupBox->setLayout(formLayout);
        formLayout->addRow(new QLabel(tr("These settings will not take effect until %1 and VSTi are all restarted.").arg(QApplication::applicationName())));
        mainLayout->addWidget(advancedSettingsGroupBox);

        m_ipcTimeoutSpinBox = new QSpinBox;
        m_ipcTimeoutSpinBox->setRange(0, std::numeric_limits<int>::max());
        m_ipcTimeoutSpinBox->setValue(ipcTimeout());
        formLayout->addRow(tr("IPC Timeout"), m_ipcTimeoutSpinBox);

        m_mainKeyLineEdit = new QLineEdit;
        m_mainKeyLineEdit->setText(mainKey());
        formLayout->addRow(tr("Main Key"), m_mainKeyLineEdit);

        connect(VstHelper::instance(), &VstHelper::connectionStatusChanged, this, &VstSettingPage::updateConnectionInfo);
        connect(VstHelper::instance(), &VstHelper::statisticsChanged, this, &VstSettingPage::updateStatisticsInfo);

        m_widget = mainWidget;

        updateConnectionInfo();
        updateStatisticsInfo();

        return m_widget;

    }

    void VstSettingPage::updateConnectionInfo() {
        if(!m_widget) return;
        QString yesText = tr("Yes");
        QString noText = tr("No");
        QString naText = tr("<N/A>");
        auto &cs = VstHelper::instance()->connectionStatus;
        m_connEnabled->setText(1, cs.isRemoting ? yesText : noText);
        m_connConnected->setText(1, cs.isConnected ? yesText: noText);
        m_connHost->setText(1, cs.hostName);
        m_connIsProcessing->setText(1, cs.isProcessing ? yesText : noText);
        m_connSampleRate->setText(1, cs.isProcessing ? QString::number(cs.sampleRate) : naText);
        m_connBufferSize->setText(1, cs.isProcessing ? QString::number(cs.bufferSize) : naText);
        m_connChannelCount->setText(1, cs.isProcessing ? QString::number(cs.channelCount) : naText);

    }
    void VstSettingPage::updateStatisticsInfo() {
    }

    bool VstSettingPage::accept() {
        if(m_mainKeyLineEdit->text() == mainKey() && m_ipcTimeoutSpinBox->value() == ipcTimeout()) {
            return true;
        }
        auto settingObj = Core::ILoader::instance()->settings()->value("VstClient").toObject();
        settingObj.insert("mainKey", m_mainKeyLineEdit->text());
        settingObj.insert("ipcTimeout", m_ipcTimeoutSpinBox->value());
        VstHelper::generateVstConfig();
        return true;
    }
    void VstSettingPage::finish() {
        delete m_widget;
        m_widget = nullptr;
    }

    QString VstSettingPage::mainKey() {
        return Core::ILoader::instance()->settings()->value("VstClient").toObject().value("mainKey").toString("77F6E993-671E-4283-99BE-C1CD1FF5C09E");
    }
    int VstSettingPage::ipcTimeout() {
        return Core::ILoader::instance()->settings()->value("VstClient").toObject().value("ipcTimeout").toInt(5000);
    }
} // Internal