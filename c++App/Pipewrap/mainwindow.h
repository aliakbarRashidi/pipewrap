#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_browserwidget.h>

class WorkflowTreeView;
class Assistant;
class Browser;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    //virtual ~MainWindow();
    QString getName() const;
    void clearName();

protected:
    void closeEvent(QCloseEvent *event);

signals:

private slots:
    void openSlot();
    bool saveSlot();
    bool saveAsSlot();
    void aboutSlot();
    void openRecentFileSlot();
    void documentModifiedSlot();
    void ShowHelpSlot();
    void newSlot();
    void sqlSlot();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();

    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName) const;
    void updateWindowsTitle();
    void init();

    QString curFile;
    //QString pipeName;
    WorkflowTreeView* tree_view;
    Assistant *assistant;

    QMenu *fileMenu;
    QMenu *recentFilesMenu;
    QMenu *actionsMenu;
    QMenu *helpMenu;
    QToolBar* fileToolBar;
    QToolBar* editToolBar;
    QToolBar* actionsToolBar;
    QToolBar* helpToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    QAction *separatorAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *executeAct;

    QAction *addAct;
    QAction *upAct;
    QAction *downAct;
    QAction *deleteAct;

    QAction *aboutAct;
    QAction *assistantAct;
    QAction *sqlAct;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];

    QMainWindow* sqlWin;
    Browser* sqlBrowser;
};

#endif // MAINWINDOW_H













