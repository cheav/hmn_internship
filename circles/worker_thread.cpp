#include <QImage>
#include <QString>

#include <algorithm>
#include <cassert>
#include <vector>

#include "worker_thread.h"
#include "main_window.h"
#include "asmOpenCV.h"

#include <opencv2/imgproc.hpp>
//-------------------------------------------------------------------------------------------------
WorkerThread::WorkerThread(MainWindow *pWindow) : QThread(pWindow), m_pWindow(pWindow) {}
WorkerThread::~WorkerThread() {}
//-------------------------------------------------------------------------------------------------
const QImage *WorkerThread::getResultImage() const
{
    assert(isFinished());
    //
    return m_ptrResult.data();
}
void WorkerThread::startLoadFile(const QString &rcFilePath)
{
    if (isRunning()) return;
    //
    m_nOperation = LoadFile;
    m_FilePath = rcFilePath;
    m_ptrInput.reset(new QImage);
    QThread::start();
}
void WorkerThread::startBlurImage(const QImage &rcImage)
{
    if (isRunning()) return;
    //
    m_nOperation = BlurImage;
    m_ptrInput.reset(new QImage(rcImage));
    QThread::start();
}
void WorkerThread::startFindCircles(const QImage &rcImage)
{
    if (isRunning()) return;
    //
    m_nOperation = FindCircles;
    m_ptrInput.reset(new QImage(rcImage));
    QThread::start();
}
void WorkerThread::stop()
{
    requestInterruption();
}
void WorkerThread::run()
{
    switch (m_nOperation)
    {
    case LoadFile:
        //
        if (m_ptrInput->load(m_FilePath))
            m_ptrResult.reset(new QImage(*m_ptrInput));
        break;
        //
    case BlurImage:
        //
        if (m_ptrInput)
        {
            m_ptrResult.reset(new QImage(*m_ptrInput));
            //
            const int cm = m_ptrInput->width();
            const int cn = m_ptrInput->height();
            const int cnTotal = cm * cn;
            const int cnWindow = 5;
            int nCurrent = 0, nPercent, nPrev = 0;
            for (int i = 0; i < cm; ++ i)
                for (int j = 0; j < cn; ++ j)
                {
                    std::vector <int> reds, greens, blues;
                    for (int k = i - cnWindow; k < i + cnWindow; ++ k)
                        if (k >= 0 && k < cm)
                            for (int l = j - cnWindow; l < j + cnWindow; ++ l)
                                if (l >= 0 && l < cn)
                                {
                                    QColor color = m_ptrInput->pixel(k, l);
                                    reds.push_back(color.red());
                                    greens.push_back(color.green());
                                    blues.push_back(color.blue());
                                }
                    //
                    QColor color(
                                std::accumulate(reds.begin(), reds.end(), 0) /
                                reds.size(),
                                std::accumulate(greens.begin(), greens.end(), 0) /
                                greens.size(),
                                std::accumulate(blues.begin(), blues.end(), 0) /
                                blues.size());
                    m_ptrResult->setPixel(i, j, color.rgb());
                    //
                    if (isInterruptionRequested())
                    {
                        m_ptrResult.reset();
                        emit canceled();
                        return;
                    }
                    //
                    ++ nCurrent;
                    nPercent = (100 * nCurrent) / cnTotal;
                    if (nPercent > nPrev)
                    {
                        nPrev = nPercent;
                        QMetaObject::invokeMethod(
                                    m_pWindow,
                                    "updateProgress", // Q_INVOKABLE void myMethod();
                                    Qt::QueuedConnection,
                                    // Q_RETURN_ARG(QString, retVal)
                                    Q_ARG(int, nPercent));
                        //
                        //emit percentChanged(nPercent);
                    }
                } // for (int j = 0; ...)
        } // if (m_ptrInput)
        //
        break;
    case FindCircles:
        //
        if (m_ptrInput)
        {
            m_ptrResult.reset(new QImage(*m_ptrInput));

            cv::Mat cvImage = ASM::QImageToCvMat(*m_ptrResult.data());
            cv::Mat cvImageGray;

            // transform to gray image
            if(!cvImage.empty())
                cv::cvtColor(cvImage, cvImageGray, cv::COLOR_BGR2GRAY);

            // smooth it, otherwise a lot of false circles may be detected
            cv::GaussianBlur(cvImageGray, cvImageGray, cv::Size(9, 9), 2, 2);

            std::vector<cv::Vec3f> circles; // x,y,radius
            cv::HoughCircles(cvImageGray, circles, CV_HOUGH_GRADIENT,
                             2, cvImageGray.rows/6, 200, 65);
            // circles draw
            //
            const int cm = m_ptrInput->width();
            const int cn = m_ptrInput->height();
            const int cnTotal = cm * cn;
            int nCurrent = 0, nPercent, nPrev = 0;
            //
            for(size_t i = 0; i < circles.size(); i++)
            {
                cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                // draw the circle center
                cv::circle(cvImage, center, 3, cv::Scalar(0,255,0), -1, 8, 0);
                // draw the circle outline
                cv::circle(cvImage, center, radius, cv::Scalar(0,0,255), 2, 8, 0);
                //
                if(isInterruptionRequested())
                {
                    m_ptrResult.reset(new QImage(*m_ptrInput));
                    emit canceled();
                    return;
                }
                //
                ++ nCurrent;
                nPercent = (100 * nCurrent) / cnTotal;
                if (nPercent > nPrev)
                {
                    nPrev = nPercent;
                    QMetaObject::invokeMethod(
                                m_pWindow,
                                "updateProgress", // Q_INVOKABLE void myMethod();
                                Qt::QueuedConnection,
                                // Q_RETURN_ARG(QString, retVal)
                                Q_ARG(int, nPercent));
                }
                //
            }// for(size_t i = 0; i < circles.size(); i++)

            QImage resultImage = ASM::cvMatToQImage(cvImage);

            if(!resultImage.isNull())
                m_ptrResult.reset(new QImage(resultImage));

        } // if (m_ptrInput)
        break;
    } // switch (m_nOperation)

} // WorkerThread::run()
