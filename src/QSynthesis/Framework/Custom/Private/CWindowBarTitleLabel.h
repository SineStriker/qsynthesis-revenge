#ifndef CWINDOWBARTITLELABEL_H
#define CWINDOWBARTITLELABEL_H

#include <QLabel>

class CWindowBarTitleLabel : public QLabel {
    Q_OBJECT
public:
    explicit CWindowBarTitleLabel(QWidget *parent = nullptr);
    explicit CWindowBarTitleLabel(const QString &text, QWidget *parent = nullptr);
    ~CWindowBarTitleLabel();

    int textDirty;

public:
    void setText(const QString &text);

    bool validHints() const;

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // CWINDOWBARTITLELABEL_H
