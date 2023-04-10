#ifndef IMPORTDIALOG_P_H
#define IMPORTDIALOG_P_H

#include "../ImportDialog.h"

#include "Layout/QMAutoResizer.h"
#include "Layout/QMEqualBoxLayout.h"
#include "Widgets/CTabButton.h"
#include "Widgets/LinearScrollArea.h"

#include <QAction>
#include <QButtonGroup>
#include <QLabel>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QTableWidget>
#include <QVBoxLayout>

#include <list>

namespace QsApi {

    class ImportDialogPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ImportDialog)
    public:
        ImportDialogPrivate();
        ~ImportDialogPrivate();

        void init();

        ImportDialog *q_ptr;

        // Options
        ImportDialog::ImportOptions opt;

        // Results
        QList<int> trackIndexs;
        QTextCodec *codec;

        // Tab - Track
        QButtonGroup *boxGroup;
        QWidget *boxesWidget;
        QMAutoResizer *boxesWidgetResizer;
        QVBoxLayout *boxesLayout;
        QScrollArea *tracksScroll;

        // Tab - Codec
        QSplitter *codecWidget;
        QListWidget *codecListWidget;
        QPlainTextEdit *labelsWidget;
        QListWidget *nameListWidget;
        QPlainTextEdit *lyricsWidget;

        // Main
        QTabWidget *tabWidget;
        CTabButton *btnCancel, *btnOK;
        QMEqualBoxLayout *buttonsLayout;

        QVBoxLayout *layout;
        QAction *okAction;

        std::list<QAbstractButton *> queue;
        QHash<QAbstractButton *, std::list<QAbstractButton *>::iterator> queueMap;

        bool firstShow;
        int maxInitHeight;

        void updateEncoding();
        void updateNameList();

        bool codecVisible;
        void setCodecTabVisible(bool visible);

    private:
        void _q_boxToggled(bool checked);
        void _q_okButtonClicked();
        void _q_cancelButtonClicked();
        void _q_scrollRangeChanged(int min, int max);

        void _q_currentCodecChanged(QListWidgetItem *cur, QListWidgetItem *prev);
        void _q_currentNameChanged(QListWidgetItem *cur, QListWidgetItem *prev);
    };

}

#endif // IMPORTDIALOG_P_H
