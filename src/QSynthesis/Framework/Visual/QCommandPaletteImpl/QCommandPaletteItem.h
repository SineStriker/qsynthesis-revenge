#ifndef QCOMMANDPALETTEITEM_H
#define QCOMMANDPALETTEITEM_H

#include <QGridLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QPushButton>
#include <QWidget>

class QCommandPaletteItemWidget : public QWidget {
    Q_OBJECT
public:
    explicit QCommandPaletteItemWidget(QWidget *parent = nullptr);
    ~QCommandPaletteItemWidget();

public:
    QString leftTopText() const;
    void setLeftTopText(const QString &text) const;

    QString leftBottomText() const;
    void setLeftBottomText(const QString &text) const;

    QString rightTopText() const;
    void setRightTopText(const QString &text) const;

    QString rightBottomText() const;
    void setRightBottomText(const QString &text) const;

    QListWidgetItem *item;

protected:
    QLabel *leftTopLabel;
    QLabel *leftBottomLabel;
    QLabel *rightTopLabel;
    QLabel *rightBottomLabel;

    QPushButton *iconButton;
    QPushButton *rightButton;

    QGridLayout *layout;

    bool event(QEvent *event) override;
};

#endif // QCOMMANDPALETTEITEM_H
