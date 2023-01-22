#ifndef ANATIVEBOXLAYOUT_H
#define ANATIVEBOXLAYOUT_H

#include <QBoxLayout>
#include <QWidget>

#define A_LAYOUT_CREATE_INDEXER(Getter, Setter, Taker, Index)                                      \
    QWidget *Getter() const {                                                                      \
        return widgetAt(Index);                                                                    \
    }                                                                                              \
                                                                                                   \
    void Setter(QWidget *Getter) {                                                                 \
        setWidgetAt(Index, Getter);                                                                \
    }                                                                                              \
                                                                                                   \
    QWidget *Taker() {                                                                             \
        return takeWidgetAt(Index);                                                                \
    }

#define A_LAYOUT_INIT(Start, Count)                                                                \
    for (int i = Start; i < Start + Count; ++i) {                                                  \
        insertDefaultSpace(i);                                                                     \
    }

class Q_DECL_EXPORT ANativeBoxLayout : public QBoxLayout {
    Q_OBJECT
public:
    explicit ANativeBoxLayout(Direction dir, QWidget *parent = nullptr);
    ~ANativeBoxLayout();

public:
    QWidget *widgetAt(int index) const;
    void setWidgetAt(int index, QWidget *widget);
    QWidget *takeWidgetAt(int index);

    void insertDefaultSpace(int index);

signals:
};

#endif // ANATIVEBOXLAYOUT_H
