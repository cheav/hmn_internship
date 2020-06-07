#ifndef MAIN_WINDOW_H__
#define MAIN_WINDOW_H__
#include <QMainWindow>
#include "ui_main_window.h"

class QProgressBar;
class QPushButton;
class QLabel;
class QScrollArea;
class WorkerThread;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *pParent = nullptr);
public slots:
    void updateProgress(int nPercent);
protected:
    void closeEvent(QCloseEvent *pEvent) override;
private slots:
    void onActionOpen_triggered();
    void onActionBlur_triggered();
    void onActionFind_circles_triggered();
    void sActionCancel();
    void onThreadStarted();
    void onThreadFinished();
    void onThreadCanceled();
    void onActionAbout_triggered();
private:
    QString m_fileName;
    //
    QImage m_Image;
    QLabel *m_pLabelImage;
    QScrollArea *m_pAreaImage;
    WorkerThread *m_pThreadWork;
    bool m_bCanceled;
    //
    QProgressBar *m_pProgressBar;
    QPushButton *m_pButtonCancel;
};

#endif // MAIN_WINDOW_H__
