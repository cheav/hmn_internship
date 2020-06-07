#ifndef MAIN_WIN_H__
#define MAIN_WIN_H__
#include <QMainWindow>
#include "ui_main_window.h"

//class DocWindowTable;
class DocWindowTree;

class MainWin : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
    MainWin(QWidget *pParent = nullptr);
protected:
    void closeEvent(QCloseEvent *pEvent) override;
private slots:
    void New();
    void Open();
    void OpenRecentFile();
    void CloseAllWindows();
    void About();
private:
    //
    enum { enMaxRecentFiles = 5 };
    //
    QAction *m_apActionsRecent[enMaxRecentFiles];
    QAction *m_pActionSeparator;
    //
    QString m_fileName;
    QStringList m_listRecentFiles;
    //
    //DocWindowTable* createNewTable();
    DocWindowTree* createNewTree();
    void loadFile(const QString &rcFileName);
    void updateRecentFileActions();
    void updateStatusBar();
};

#endif // MAIN_WIN_H__
