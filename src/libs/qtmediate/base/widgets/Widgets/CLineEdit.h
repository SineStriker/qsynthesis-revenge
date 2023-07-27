#ifndef CHORUSKIT_CLINEEDIT_H
#define CHORUSKIT_CLINEEDIT_H

// QLineEdit has a bug that the placeholder color would be black (wrong color) if
// a polish event is handled when the window is not active
// This override class is a temporary solution

#include <QLineEdit>

#include "QMWidgetsGlobal.h"

class QMWIDGETS_EXPORT CLineEdit : public QLineEdit {
    Q_OBJECT
public:
    explicit CLineEdit(QWidget *parent = nullptr);
    explicit CLineEdit(const QString &text, QWidget *parent = nullptr);
    ~CLineEdit();

signals:
    void specialKeyPressed(int key);

protected:
    bool event(QEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
};



#endif // CHORUSKIT_CLINEEDIT_H
