#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPluginLoader>
#include <QTime>

#include "Common/CodecArguments.h"
#include "Common/SampleFormat.h"

#include "MathHelper.h"
#include "SystemHelper.h"

// https://iconduck.com/icons

static QString audioFileToLabFile(const QString &filename) {
    QFileInfo info(filename);
    QString suffix = info.suffix().toLower();
    return info.absolutePath() + "/" + info.baseName() + (suffix != "wav" ? "_" + suffix : "") +
           ".lab";
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    notifyTimerId = 0;
    playing = false;

    setAcceptDrops(true);

    initStyleSheet();

    // Init menus
    browseAction = new QAction("Open Folder", this);
    browseAction->setShortcut(QKeySequence("Ctrl+O"));

    fileMenu = new QMenu("File(&F)", this);
    fileMenu->addAction(browseAction);

    nextAction = new QAction("Next file", this);
    nextAction->setShortcut(QKeySequence::MoveToNextPage);

    prevAction = new QAction("Previous file", this);
    prevAction->setShortcut(QKeySequence::MoveToPreviousPage);

    editMenu = new QMenu("Edit(&E)", this);
    editMenu->addAction(nextAction);
    editMenu->addAction(prevAction);

    playAction = new QAction("Play/Stop", this);
    playAction->setShortcut(QKeySequence("F5"));

    playMenu = new QMenu("Playback(&P)", this);
    playMenu->addAction(playAction);

    aboutAppAction = new QAction(QString("About %1").arg(qAppName()), this);
    aboutQtAction = new QAction("About Qt", this);

    helpMenu = new QMenu("Help(&H)", this);
    helpMenu->addAction(aboutAppAction);
    helpMenu->addAction(aboutQtAction);

    auto bar = menuBar();
    bar->addMenu(fileMenu);
    bar->addMenu(editMenu);
    bar->addMenu(playMenu);
    bar->addMenu(helpMenu);

    // Init widgets
    playerWidget = new PlayWidget();
    playerWidget->setObjectName("play-widget");
    playerWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    textWidget = new TextWidget();
    textWidget->setObjectName("text-widget");

    fsModel = new QFileSystemModel();
    fsModel->setParent(this);
    fsModel->setNameFilters({"*.wav", "*.mp3", "*.m4a", "*.flac"});
    fsModel->setNameFilterDisables(false);

    treeView = new QTreeView();
    treeView->setModel(fsModel);

    {
        QSet<QString> names{"Name", "Size"};
        for (int i = 0; i < fsModel->columnCount(); ++i) {
            if (!names.contains(fsModel->headerData(i, Qt::Horizontal).toString())) {
                treeView->hideColumn(i);
            }
        }
    }

    rightLayout = new QVBoxLayout();
    rightLayout->addWidget(playerWidget);
    rightLayout->addWidget(textWidget);

    rightWidget = new QWidget();
    rightWidget->setLayout(rightLayout);

    mainSplitter = new QSplitter();
    mainSplitter->setObjectName("central-widget");
    mainSplitter->addWidget(treeView);
    mainSplitter->addWidget(rightWidget);
    setCentralWidget(mainSplitter);

    mainSplitter->setStretchFactor(0, 0);
    mainSplitter->setStretchFactor(1, 1);

    // Init signals
    connect(fileMenu, &QMenu::triggered, this, &MainWindow::_q_fileMenuTriggered);
    connect(editMenu, &QMenu::triggered, this, &MainWindow::_q_editMenuTriggered);
    connect(playMenu, &QMenu::triggered, this, &MainWindow::_q_playMenuTriggered);
    connect(helpMenu, &QMenu::triggered, this, &MainWindow::_q_helpMenuTriggered);

    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &MainWindow::_q_treeCurrentChanged);

    reloadWindowTitle();
    resize(1280, 720);
}

MainWindow::~MainWindow() {
    if (Sys::isFileExist(lastFile)) {
        saveFile(lastFile);
    }
}

void MainWindow::openDirectory(const QString &dirname) {
    fsModel->setRootPath(dirname);
    treeView->setRootIndex(fsModel->index(dirname));
}

void MainWindow::openFile(const QString &filename) {
    QString content;

    QString labFile = audioFileToLabFile(filename);
    QFile file(labFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        content = QString::fromUtf8(file.readAll());
    }

    textWidget->setText(content);

    playerWidget->openFile(filename);
}

void MainWindow::saveFile(const QString &filename) {
    QString labFile = audioFileToLabFile(filename);

    QString content = textWidget->text();
    if (content.isEmpty()) {
        return;
    }

    QFile file(labFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(
            this, qAppName(),
            QString("Failed to write to file %1").arg(Sys::PathFindFileName(labFile)));
        ::exit(-1);
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    in << content;

    file.close();
}

void MainWindow::reloadWindowTitle() {
    setWindowTitle(dirname.isEmpty()
                       ? qAppName()
                       : QString("%1 - %2").arg(
                             qAppName(), QDir::toNativeSeparators(Sys::PathFindFileName(dirname))));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    auto e = static_cast<QDragEnterEvent *>(event);
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls()) {
        auto urls = mime->urls();
        QStringList filenames;
        for (auto it = urls.begin(); it != urls.end(); ++it) {
            if (it->isLocalFile()) {
                filenames.append(Sys::removeTailSlashes(it->toLocalFile()));
            }
        }
        bool ok = false;
        if (filenames.size() == 1) {
            QString filename = filenames.front();
            if (Sys::isDirExist(filename)) {
                ok = true;
            }
        }
        if (ok) {
            e->acceptProposedAction();
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    auto e = static_cast<QDropEvent *>(event);
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls()) {
        auto urls = mime->urls();
        QStringList filenames;
        for (auto it = urls.begin(); it != urls.end(); ++it) {
            if (it->isLocalFile()) {
                filenames.append(Sys::removeTailSlashes(it->toLocalFile()));
            }
        }
        bool ok = false;
        if (filenames.size() == 1) {
            QString filename = filenames.front();
            if (Sys::isDirExist(filename)) {
                ok = true;
                openDirectory(filename);
            }
        }
        if (ok) {
            e->acceptProposedAction();
        }
    }
}

void MainWindow::initStyleSheet() {
    QFile file(":/res/app.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(file.readAll());
    }
}

void MainWindow::_q_fileMenuTriggered(QAction *action) {
    if (action == browseAction) {
        playerWidget->setPlaying(false);

        QString path = QFileDialog::getExistingDirectory(this, "Open Folder",
                                                         QFileInfo(dirname).absolutePath());
        if (path.isEmpty()) {
            return;
        }
        openDirectory(path);

        dirname = path;
    }
    reloadWindowTitle();
}

void MainWindow::_q_editMenuTriggered(QAction *action) {
    if (action == prevAction) {
        QKeyEvent e(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
        QApplication::sendEvent(treeView, &e);
    } else if (action == nextAction) {
        QKeyEvent e(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
        QApplication::sendEvent(treeView, &e);
    }
}

void MainWindow::_q_playMenuTriggered(QAction *action) {
    if (action == playAction) {
        playerWidget->setPlaying(!playerWidget->isPlaying());
    }
}

void MainWindow::_q_helpMenuTriggered(QAction *action) {
    if (action == aboutAppAction) {
        QMessageBox::information(this, qAppName(),
                                 QString("%1 %2, Copyright OpenVPI.").arg(qAppName(), APP_VERSON));
    } else if (action == aboutQtAction) {
        QMessageBox::aboutQt(this);
    }
}

void MainWindow::_q_treeCurrentChanged(const QModelIndex &current, const QModelIndex &previous) {
    Q_UNUSED(previous);

    QFileInfo info = fsModel->fileInfo(current);
    if (info.isFile()) {
        if (Sys::isFileExist(lastFile)) {
            saveFile(lastFile);
        }
        lastFile = info.absoluteFilePath();
        openFile(lastFile);
    }
}
