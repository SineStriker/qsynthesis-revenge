#ifndef CHORUSKIT_TREEJSONUTIL_H
#define CHORUSKIT_TREEJSONUTIL_H

#include "../Widgets/TreeConfigWidget.h"
#include "../Widgets/TreeDevWidget.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTreeWidget>

namespace TemplatePlg {
    namespace Internal {
        class TreeJsonUtil {
            friend class TreeDevWidget;
            friend class TreeConfigWidget;
        private:
            static QString getLocalLanguage();
            static QWidget *qFileWidget(QString text = NULL);
            static QMessageBox *messageBox(QString title, QString text);
            static QJsonArray JsonArrayFromFile(const QString filePath);
            static QJsonObject JsonObjectFromFile(const QString filePath);
            static bool JsonArrayToFile(const QString filePath, const QJsonArray configJson);
            static bool TreeToFile(const QString filePath, QTreeWidget *treeWidget);
            static QJsonObject JsonArrayToJsonObject(const QJsonArray &jsonArray);
            static QJsonArray JsonArrayFromTree(QTreeWidget *treeWidget, QTreeWidgetItem *item = nullptr);
            static QJsonObject JsonObjectFromTree(QTreeWidget *treeWidget, QTreeWidgetItem *item = nullptr);
            static void TreeFromJsonArray(const QJsonArray &config, int insertIndex, bool configGen,
                                          QTreeWidget *treeWidget, QTreeWidgetItem *parent = nullptr);
            static void TreeFromFile(const QString filePath, bool configGen, QTreeWidget *treeWidget);
        };
    }
} // TemplatePlg

#endif // CHORUSKIT_TREEJSONUTIL_H
