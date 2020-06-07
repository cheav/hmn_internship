#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <QThread>
#include <QImage>

class MainWindow;
class QString;

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(MainWindow *pWindow);
    ~WorkerThread();
public:
    const QImage *getResultImage() const;
    void startLoadFile(const QString &rcFilePath);
    void startBlurImage(const QImage &rcImage);
    void startFindCircles(const QImage &rcImage);
    void stop();
signals:
    void canceled();
    void percentChanged(int);
private:
    enum Operation
    {
        LoadFile,
        BlurImage,
        FindCircles
    };
    //
    MainWindow *m_pWindow;
    Operation m_nOperation;
    QString m_FilePath;
    QScopedPointer <QImage> m_ptrInput, m_ptrResult;
    //
    virtual void run();
};

#endif // WORKER_THREAD_H
