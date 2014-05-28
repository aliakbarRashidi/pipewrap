#include <QSettings>
#include <QString>
#include <QAction>
#include <QTextStream>
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include "mainwindow.h"
#include "workflowtreeview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    Init();
}

MainWindow::~MainWindow()
{
}

void MainWindow::Init()
{
    setWindowTitle(tr("Shell command tool"));
    setAttribute(Qt::WA_DeleteOnClose);

    tree_view = new WorkflowTreeView;
    setCentralWidget(tree_view);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    readSettings();
    connect(tree_view, SIGNAL(contentsChanged()),
            this, SLOT(documentModifiedSlot()));

    setWindowFilePath(QString());
    resize(600, 400);
}

void MainWindow::newSlot()
{
    tree_view = new WorkflowTreeView;
    setCentralWidget(tree_view);;
    tree_view->show();
}
void MainWindow::openSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}

bool MainWindow::saveSlot()
{
    if (curFile.isEmpty())
        return saveAsSlot();
    else
        return saveFile(curFile);
}

bool MainWindow::saveAsSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::openRecentFileSlot()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

void MainWindow::aboutSlot()
{
   QMessageBox::about(this, tr("Test pSNP detection"),
            tr("This is piers' program."));
}

void MainWindow::documentModifiedSlot()
{
    setWindowModified(true);
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newSlot()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openSlot()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveSlot()));

    saveAsAct = new QAction(QIcon(":/images/saveas.xpm"), tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAsSlot()));


    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFileSlot()));
    }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(aboutSlot()));

    executeAct = new QAction(QIcon(":/images/star.png"),tr("Exe&cute"), this);
    //executeAct->setShortcuts("x");
    executeAct->setStatusTip(tr("Excecute"));
    connect(executeAct, SIGNAL(triggered()), tree_view, SLOT(executeSlot()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), tree_view, SLOT(cutSlot()));

    //copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    //copyAct->setShortcuts(QKeySequence::Copy);
    //copyAct->setStatusTip(tr("Copy the current selection's contents to the "
    //                         "clipboard"));
    //connect(copyAct, SIGNAL(triggered()), tree_view, SLOT(copySlot()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), tree_view, SLOT(pasteSlot()));

    addAct = new QAction(QIcon(":/images/edit_add.png"), tr("&Add"), this);
    //addAct->setShortcuts(QKeySequence::Paste);
    addAct->setStatusTip(tr("Add new tool to pipline"));
    connect(addAct, SIGNAL(triggered()), tree_view, SLOT(addSlot()));

    upAct = new QAction(QIcon(":/images/up.png"), tr("&Up"), this);
    //upAct->setShortcuts(QKeySequence::Paste);
    upAct->setStatusTip(tr("Move selected tool up one"));
    connect(upAct, SIGNAL(triggered()), tree_view, SLOT(upSlot()));

    downAct = new QAction(QIcon(":/images/down.png"), tr("&Down"), this);
    //downAct->setShortcuts(QKeySequence::Paste);
    downAct->setStatusTip(tr("Move selected tool down one"));
    connect(downAct, SIGNAL(triggered()), tree_view, SLOT(downSlot()));

    deleteAct = new QAction(QIcon(":/images/remove.png"), tr("&Delete"), this);
    //deleteAct->setShortcuts(QKeySequence::Paste);
    deleteAct->setStatusTip(tr("Remove selected tool"));
    connect(deleteAct, SIGNAL(triggered()), tree_view, SLOT(deleteSlot()));

    cutAct->setEnabled(false);
    upAct->setEnabled(false);
    downAct->setEnabled(false);
    deleteAct->setEnabled(false);
    //copyAct->setEnabled(false);
    pasteAct->setEnabled(false);
    connect(tree_view, SIGNAL(cutAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(upAct, SIGNAL(cutAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(downAct, SIGNAL(cutAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(deleteAct, SIGNAL(cutAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    //connect(tree_view, SIGNAL(copyAvailable(bool)),
    //        copyAct, SLOT(setEnabled(bool)));
    connect(tree_view, SIGNAL(pasteAvailable(bool)),
            pasteAct, SLOT(setEnabled(bool)));

}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsAct);
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    //editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->addAction(executeAct);
    editToolBar->addAction(addAct);
    editToolBar->addAction(upAct);
    editToolBar->addAction(downAct);
    editToolBar->addAction(deleteAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::maybeSave()
{
    if (tree_view->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("SDI"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard
                     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveSlot();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    updateRecentFileActions();
    menuBar()->addSeparator();

    actionsMenu = menuBar()->addMenu(tr("&Action"));
    actionsMenu->addAction(addAct);
    actionsMenu->addAction(cutAct);
    //actionsMenu->addAction(copyAct);
    actionsMenu->addAction(pasteAct);
    actionsMenu->addAction(executeAct);
    actionsMenu->addAction(upAct);
    actionsMenu->addAction(downAct);
    actionsMenu->addAction(deleteAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Recent Files"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QDataStream in(&file);
    newSlot();
    tree_view->read(in);
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Recent Files"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    tree_view->write(out);
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowFilePath(curFile);

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    //foreach (QWidget *widget, QApplication::topLevelWidgets())
    //{
    //    MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
    //    if (mainWin)
    //        mainWin->updateRecentFileActions();
    //}
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
