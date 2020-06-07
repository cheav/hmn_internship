#include "main_win.h"
#include "tree_model.h"
//#include "table_model.h"
//#include "doc_window_table.h"
#include "doc_window.h"
#include "combo_delegate.h"

#include <QtWidgets>
#include <QFile>
#include <QTreeView>

namespace
{
QString strippedName(const QString &rcFullFileName)
{
    return QFileInfo(rcFullFileName).fileName();
}
}

MainWin::MainWin(QWidget *pParent) : QMainWindow(pParent)
{
    setupUi(this);
    setWindowTitle(tr("Treeview editor"));
    //---------------------------------------------------------------------------------------------
    connect(actionNew, SIGNAL(triggered(bool)), this, SLOT(New()));
    connect(actionOpen, SIGNAL(triggered(bool)), this, SLOT(Open()));
    connect(actionAbout, SIGNAL(triggered(bool)), this, SLOT(About()));
    connect(actionClose_all_windows, SIGNAL(triggered(bool)), this, SLOT(CloseAllWindows()));

    m_pActionSeparator = menuFile->addSeparator();
    m_pActionSeparator->setVisible(false);
    for (int i = 0; i < enMaxRecentFiles; ++i)
    {
        m_apActionsRecent[i] = new QAction(this);
        m_apActionsRecent[i]->setVisible(false);
        connect(m_apActionsRecent[i], SIGNAL(triggered()), this, SLOT(OpenRecentFile()));
        menuFile->addAction(m_apActionsRecent[i]);
    }
    menuFile->addSeparator();
    menuFile->addAction(tr("Quit"), this, SLOT(close()), QKeySequence::Quit);

    updateStatusBar();
    //
    setWindowIcon(QIcon(":/resources/icon.png"));
    //
    QSettings settings("cheav", "mvc-04");
    restoreGeometry(settings.value("geometry").toByteArray());
    m_listRecentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
}
//-------------------------------------------------------------------------------------------------
void MainWin::closeEvent(QCloseEvent *pEvent)
{
    pEvent->accept(); // ignore()
    //
    QSettings settings("cheav", "mvc-04");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", m_listRecentFiles);
}
//-------------------------------------------------------------------------------------------------
/*
DocWindowTable *MainWin::createNewTable()
{
    DocWindowTable *pDoc = new DocWindowTable;
    mdiArea->addSubWindow(pDoc);
    pDoc->setAttribute(Qt::WA_DeleteOnClose);
    pDoc->setWindowTitle(tr("Table Editor"));
    pDoc->setWindowIcon(QPixmap(":/resources/filenew.png"));
    return pDoc;
}
*/
DocWindowTree *MainWin::createNewTree()
{
    DocWindowTree *pDoc = new DocWindowTree;
    mdiArea->addSubWindow(pDoc);
    pDoc->setAttribute(Qt::WA_DeleteOnClose);
    pDoc->setWindowTitle(tr("Treeview Editor"));
    pDoc->setWindowIcon(QPixmap(":/resources/filenew.png"));
    return pDoc;
}
void MainWin::loadFile(const QString &rcFileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::restoreOverrideCursor();
    if(!rcFileName.isEmpty())
    {
        m_fileName = rcFileName;
        m_listRecentFiles.removeAll(m_fileName);
        m_listRecentFiles.prepend(m_fileName);
        updateRecentFileActions();
        updateStatusBar();
        statusBar()->showMessage(tr("File loaded"), 2000);
        createNewTree()->show();
    }
}
void MainWin::updateRecentFileActions()
{
    QMutableStringListIterator i(m_listRecentFiles);
    while (i.hasNext())
        if (!QFile::exists(i.next())) i.remove();
    //
    for (int j = 0; j < enMaxRecentFiles; ++ j)
        if (j < m_listRecentFiles.count())
        {
            QString text = QString("&%1 %2")
                    .arg(j + 1)
                    .arg(strippedName(m_listRecentFiles[j]));
            m_apActionsRecent[j]->setText(text);
            m_apActionsRecent[j]->setData(m_listRecentFiles[j]);
            m_apActionsRecent[j]->setVisible(true);
        }
        else
            m_apActionsRecent[j]->setVisible(false);
    m_pActionSeparator->setVisible(!m_listRecentFiles.empty());
}
void MainWin::updateStatusBar()
{

}
//-------------------------------------------------------------------------------------------------
void MainWin::New()
{
    createNewTree()->show();
}
void MainWin::Open()
{
    // same as New()
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}
void MainWin::OpenRecentFile()
{
    QAction *pAction = qobject_cast <QAction*> (sender());
    if (pAction)
        loadFile(pAction->data().toString());
}
void MainWin::CloseAllWindows()
{
    QList<QMdiSubWindow*> list = mdiArea->subWindowList();
    for(auto p : list) p->close();
}
void MainWin::About(){ QMessageBox::about(this, tr("MVC-04"), tr("Treeview editor")); }
//-------------------------------------------------------------------------------------------------
