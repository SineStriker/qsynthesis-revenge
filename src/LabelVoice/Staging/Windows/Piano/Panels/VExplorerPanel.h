#ifndef SPEAKERSPANEL_H
#define SPEAKERSPANEL_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QSplitter>
#include <QVBoxLayout>

#include "VSliceListWidget.h"
#include "VSpkTreeWidget.h"

#include "Commands/LVCommand.h"

#include "../PianoSpec.h"

class VExplorerPanel : public QFrame {
    Q_OBJECT
public:
    explicit VExplorerPanel(QWidget *parent = nullptr);
    ~VExplorerPanel();

    void reloadStrings();

    void setSpeakers(const QList<PianoSpec::SpeakerDesc> &speakers);
    void setItems(const QList<QPair<QString, PianoSpec::ItemDesc>> &items);

    // Items
    QFrame *itemsWidget;

    QLabel *itemsLabel;
    QLineEdit *itemsSearchBox;

    VSpkTreeWidget *itemsTree;

    // Sections
    QFrame *slicesWidget;

    QLabel *slicesLabel;
    QLineEdit *slicesSearchBox;

    VSliceListWidget *slicesList;

    // Main
    QSplitter *splitter;

protected:
    QVBoxLayout *itemsLayout;
    QVBoxLayout *slicesLayout;

    QVBoxLayout *mainLayout;

    // Data Structure
    enum LVNodeType {
        RootSpeaker = QTreeWidgetItem::UserType + 1,
        VirtualDir,
        LVItem,
    };

    enum LVItemDataType {
        LVItem_ID = Qt::UserRole + 1,
        LVItem_Name,
        LVItem_Speaker,
    };

    enum LVSpeakerDataType {
        LVSpeaker_ID = Qt::UserRole + 1,
        LVSpeaker_Name,
    };

    struct TreeNode {
        typedef QSharedPointer<TreeNode> Ref;

        QString name;
        QTreeWidgetItem *item;

        // Key: dir
        QHash<QString, Ref> subdirs;
        QHash<QString, QTreeWidgetItem *> files;
    };
    // Key: speaker.id
    QHash<QString, TreeNode::Ref> speakerNodes;

private:
    void _q_itemDropped(QTreeWidgetItem *item, QTreeWidgetItem *oldParent);

signals:
};

#endif // SPEAKERSPANEL_H
