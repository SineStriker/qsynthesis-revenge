#ifndef CHORUSKIT_TREEJSONUTIL_H
#define CHORUSKIT_TREEJSONUTIL_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTreeWidget>

namespace TemplatePlg::Internal {
        class TreeJsonUtil {
        public:
            static QString getLocalLanguage();
            static QWidget *qFileWidget(const QString &text = "");
            static QJsonArray jsonArrayFromFile(const QString &filePath);
            static QJsonObject jsonObjectFromFile(const QString &filePath);
            static bool jsonArrayToFile(const QString &filePath, const QJsonArray &configJson);
            static bool TreeToFile(const QString &filePath, QTreeWidget *treeWidget);
            static QJsonObject jsonArrayToJsonObject(const QJsonArray &jsonArray);
            static QJsonArray jsonArrayFromTree(QTreeWidget *treeWidget, QTreeWidgetItem *item = nullptr);
            static QJsonObject jsonObjectFromTree(QTreeWidget *treeWidget, QTreeWidgetItem *item = nullptr);
            static void TreeFromJsonArray(const QJsonArray &config, int insertIndex, bool configGen,
                                          QTreeWidget *treeWidget, QTreeWidgetItem *parent = nullptr);
            static void TreeFromFile(const QString &filePath, bool configGen, QTreeWidget *treeWidget);
        };
    } // TemplatePlg

#endif // CHORUSKIT_TREEJSONUTIL_H
