#ifndef CHORUSKIT_CONFIGMODEL_H
#define CHORUSKIT_CONFIGMODEL_H
#include <QCheckBox>
#include <QComboBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <QTreeView>
#include <QVariant>

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QVariant>

namespace TemplatePlg {
    class ConfigModel : public QStandardItemModel {
        Q_OBJECT
    public:
        explicit ConfigModel(QObject *parent = nullptr);
        void loadJsonFile(const QString &filePath);

    private:
        void populateModel(QStandardItem *parentItem, const QJsonObject &jsonObject);
    };

}
#endif // CHORUSKIT_CONFIGMODEL_H
