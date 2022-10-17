#ifndef CCOUPLETABDRAGLABEL_H
#define CCOUPLETABDRAGLABEL_H

#include <QLabel>

#include "qsframework_global.h"

class CCoupleTabBarCard;
class CCoupleTabDoubleBar;

class QSFRAMEWORK_API CCoupleTabDragLabel : public QLabel {
    Q_OBJECT
public:
    explicit CCoupleTabDragLabel(QWidget *parent = nullptr);
    ~CCoupleTabDragLabel();

public:
    CCoupleTabBarCard *currentCard;
    QPoint currentPos;
    CCoupleTabDoubleBar *originBar, *targetBar;
};

#endif // CCOUPLETABDRAGLABEL_H
