#ifndef SPEAKERSPANEL_H
#define SPEAKERSPANEL_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QSplitter>
#include <QVBoxLayout>

#include "VSliceListWidget.h"
#include "VSpeakerTreeWidget.h"

#include "Commands/LVCommand.h"

class VExplorerPanel : public QFrame, public ICommandSubscriber {
    Q_OBJECT
public:
    explicit VExplorerPanel(QWidget *parent = nullptr);
    ~VExplorerPanel();

    void reloadStrings();

    void execute(const LVCommandList &cmds, bool isUndo) override;

    // Items
    QFrame *itemsWidget;

    QLabel *itemsLabel;
    QLineEdit *itemsSearchBox;

    VSpeakerTreeWidget *speakersTree;

    // Sections
    QFrame *slicesWidget;

    QLabel *slicesLabel;
    QLineEdit *secsSearchBox;

    VSliceListWidget *slicesList;

    // Main
    QSplitter *splitter;

protected:
    QVBoxLayout *itemsLayout;
    QVBoxLayout *slicesLayout;

    QVBoxLayout *mainLayout;

signals:
};

#endif // SPEAKERSPANEL_H
