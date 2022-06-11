#ifndef QCOMMANDPALETTE_H
#define QCOMMANDPALETTE_H

#include <QListWidgetItem>
#include <QWidget>

#include "QCommandPaletteImpl/QCommandPaletteItem.h"

class QCommandPalettePrivate;

class QCommandPalette : public QWidget {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QCommandPalette)
public:
    explicit QCommandPalette(QWidget *parent = nullptr);
    ~QCommandPalette();

public:
    int addWidget(QCommandPaletteItemWidget *w);
    int insertWidget(int index, QCommandPaletteItemWidget *w);
    QCommandPaletteItemWidget *takeWidget(int index);
    QCommandPaletteItemWidget *widget(int index) const;
    int count() const;

signals:
    void activated(int index);
    void abandoned();

protected:
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    QCommandPalette(QCommandPalettePrivate &d, QWidget *parent = nullptr);

    QScopedPointer<QCommandPalettePrivate> d_ptr;

private:
    void _q_textChanged(const QString &text);
    void _q_currentRowChanged(int row);
    void _q_itemClicked(QListWidgetItem *item);

signals:
};

#endif // QCOMMANDPALETTE_H
