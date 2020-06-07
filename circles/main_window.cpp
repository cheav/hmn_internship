#include "main_window.h"
#include "worker_thread.h"

#include <QtWidgets>
#include <QFile>
#include <QPushButton>

MainWindow::MainWindow(QWidget *pParent) : QMainWindow(pParent)
{
    setupUi(this);
    setWindowTitle(tr("Circle search"));
    setGeometry(200, 200, 500, 500);
    showMaximized();
    //---------------------------------------------------------------------------------------------
    connect(actionOpen, SIGNAL(triggered(bool)), this, SLOT(onActionOpen_triggered()));
    connect(actionBlur, SIGNAL(triggered(bool)), this, SLOT(onActionBlur_triggered()));
    connect(actionFind_circles, SIGNAL(triggered(bool)), this, SLOT(onActionFind_circles_triggered()));
    connect(actionAbout, SIGNAL(triggered(bool)), this, SLOT(onActionAbout_triggered()));

    menuFile->addSeparator();
    menuFile->addAction(tr("Quit"), this, SLOT(close()), QKeySequence::Quit);
    //---------------------------------------------------------------------------------------------
    m_pLabelImage = new QLabel;
    m_pLabelImage->setAlignment(Qt::AlignCenter);
    //
    m_pAreaImage = new QScrollArea;
    m_pAreaImage->setBackgroundRole(QPalette::Dark);
    m_pAreaImage->setWidget(m_pLabelImage);
    m_pAreaImage->setWidgetResizable(true);
    setCentralWidget(m_pAreaImage);
    //
    m_pProgressBar = new QProgressBar;
    m_pButtonCancel = new QPushButton("X");
    connect(m_pButtonCancel, SIGNAL(clicked(bool)), this, SLOT(sActionCancel()));
    //
    m_pProgressBar->hide();
    m_pButtonCancel->hide();
    statusBar()->addWidget(m_pProgressBar);
    statusBar()->addWidget(m_pButtonCancel);
    //
    m_pThreadWork = new WorkerThread(this);
    connect(m_pThreadWork, SIGNAL(started()), this, SLOT(onThreadStarted()));
    connect(m_pThreadWork, SIGNAL(finished()), this, SLOT(onThreadFinished()));
    connect(m_pThreadWork, SIGNAL(canceled()), this, SLOT(onThreadCanceled()));
}
//-------------------------------------------------------------------------------------------------
void MainWindow::updateProgress(int nPercent)
{
    m_pProgressBar->setValue(nPercent);
}
void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    pEvent->accept();
    m_pThreadWork->stop();
}
//-------------------------------------------------------------------------------------------------
void MainWindow::onActionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        m_pThreadWork->startLoadFile(fileName);
}
void MainWindow::onActionBlur_triggered()
{
    m_pThreadWork->startBlurImage(m_Image);
}
void MainWindow::onActionFind_circles_triggered()
{
    m_pThreadWork->startFindCircles(m_Image);
}
void MainWindow::sActionCancel()
{
    m_pThreadWork->stop();
}
void MainWindow::onThreadStarted()
{
    actionOpen->setEnabled(false);
    actionBlur->setEnabled(false);
    m_pProgressBar->setValue(0);
    m_pProgressBar->show();
    m_pButtonCancel->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //
    m_bCanceled = false;
}
void MainWindow::onThreadFinished()
{
    actionOpen->setEnabled(true);
    actionBlur->setEnabled(true);
    m_pProgressBar->hide();
    m_pButtonCancel->hide();
    QApplication::restoreOverrideCursor();
    //
    const QImage *pcImage = m_pThreadWork->getResultImage();
    if (pcImage)
    {
        m_Image = *pcImage;
        m_pLabelImage->setPixmap(QPixmap::fromImage(m_Image));
    }
    //
    statusbar->showMessage(m_bCanceled ? "Canceled" : "Finished");
} // MainWindow::onThreadFinished()
void MainWindow::onThreadCanceled()
{
    m_bCanceled = true;
}
//-------------------------------------------------------------------------------------------------
void MainWindow::onActionAbout_triggered()
{
    QMessageBox::about(this, tr("Circles-04"), tr("Circle search"));
}
//-------------------------------------------------------------------------------------------------
