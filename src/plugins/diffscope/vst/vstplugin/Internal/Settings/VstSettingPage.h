//
// Created by Crs_1 on 2023/8/13.
//

#ifndef CHORUSKIT_VSTSETTINGPAGE_H
#define CHORUSKIT_VSTSETTINGPAGE_H

#include "CoreApi/ISettingPage.h"

class QSpinBox;
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;

namespace Vst::Internal {

    class VstSettingPage: public Core::ISettingPage {
        Q_OBJECT
    public:
        explicit VstSettingPage(QObject *parent = nullptr);
        ~VstSettingPage();

        QString sortKeyword() const override;
        bool matches(const QString &word) const override;
        QWidget *widget() override;
        bool accept() override;
        void finish() override;

        static QString mainKey();
        static int ipcTimeout();

        void updateConnectionInfo();
        void updateStatisticsInfo();

    private:
        QWidget *m_widget = nullptr;
        QSpinBox *m_ipcTimeoutSpinBox = nullptr;
        QLineEdit *m_mainKeyLineEdit = nullptr;

        QTreeWidget *m_connectionInfoList = nullptr;
        QTreeWidgetItem *m_connEnabled = nullptr;
        QTreeWidgetItem *m_connConnected = nullptr;
        QTreeWidgetItem *m_connHost = nullptr;
        QTreeWidgetItem *m_connIsProcessing = nullptr;
        QTreeWidgetItem *m_connSampleRate = nullptr;
        QTreeWidgetItem *m_connChannelCount = nullptr;
        QTreeWidgetItem *m_connBufferSize = nullptr;

        QTreeWidget *m_statisticsInfoList = nullptr;
        QTreeWidgetItem *m_statBufferCount = nullptr;
        QTreeWidgetItem *m_statSampleCount = nullptr;
        QTreeWidgetItem *m_statTotalBufferCount = nullptr;
        QTreeWidgetItem *m_statTotalSampleCount = nullptr;
    };

} // Internal

#endif // CHORUSKIT_VSTSETTINGPAGE_H
