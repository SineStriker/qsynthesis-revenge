#ifndef __CDOCKDRAGLABEL_H__
#define __CDOCKDRAGLABEL_H__

#include <QLabel>

class CDockCard;
class CDockSideBar;

class CDockDragLabel : public QLabel {
    Q_OBJECT
public:
    explicit CDockDragLabel(QWidget *parent = nullptr);
    ~CDockDragLabel();

public:
    CDockCard *currentCard;
    QPoint currentPos;
    CDockSideBar *originBar, *targetBar;
};

#endif // __CDOCKDRAGLABEL_H__