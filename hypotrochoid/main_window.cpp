#include "main_window.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QResizeEvent>
#include <QPainter>
#include <cmath>
const double pi = 3.14;

MainWindow::MainWindow(QWidget *pParent) : QMainWindow(pParent), m_pInputWin(nullptr)
{
    setWindowTitle("Гипотрохоида");
    setAttribute(Qt::WA_NoSystemBackground, true);
    resize(500, 500);

    QMenu *pMenuFile = menuBar()->addMenu(tr("&Файл"));
    pMenuFile->addSeparator();
    pMenuFile->addAction(tr("Выход"), this, SLOT(close()), QKeySequence::Quit);

    m_pInputWin = new InputDialog;
    connect(m_pInputWin, SIGNAL(dataChanged()), this, SLOT(sUpdate()));

    QMenu *pEdit = menuBar()->addMenu(tr("&Правка"));
    pEdit->addAction(tr("&Входные данные"), this, SLOT(sShowInputDataWin()));

    QMenu *pHelp = menuBar()->addMenu(tr("&Справка"));
    pHelp->addSeparator();
    pHelp->addAction(tr("&О программе"), this, SLOT(sAbout()));
}

MainWindow::~MainWindow(){}
//-------------------------------------------------------------------------------------------------
void MainWindow::resizeEvent(QResizeEvent *pEvent)
{
    m_pixmap = QPixmap(pEvent->size());
    m_pixmap.fill(Qt::white);
    draw();
}
void MainWindow::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QPainter painter(this);
    painter.drawPixmap(0, 0, m_pixmap);
}
void MainWindow::closeEvent(QCloseEvent *pEvent) {}
//-------------------------------------------------------------------------------------------------
void MainWindow::drawHypertrochoid(QPainter &painter, double R, double r, double d, double th_max)
{
    QPen pen(QColor(50, 200, 50));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    painter.setPen(pen);

    double x0 = (double) m_pixmap.width()/2;
    double y0 = (double) m_pixmap.height()/2;

    QPainterPath path(QPointF(x0, y0));

//    R = 100.0; r = 60.0; d = 120.0; th_max = 8.0 * pi;

    double factor = 0.003;
    double scale = factor * sqrt(x0*x0 + y0*y0);
    R *= scale;
    r *= scale;
    d *= scale;

    double c1 = R - r;
    double c2 = (R - r) / r;
    double x, y;


    double th = 0.0, dth = 0.1;
    x = x0 + c1 * cos(th) + d * cos(c2 * th);
    y = y0 + c1 * sin(th) - d * sin(c2 * th);
    path.moveTo(x, y);
    th += dth;

    for(;th < th_max; th += dth)
    {
        x = x0 + c1 * cos(th) + d * cos(c2 * th);
        y = y0 + c1 * sin(th) - d * sin(c2 * th);
        path.lineTo(x, y);
    }
    painter.drawPath(path);
}
void MainWindow::drawInputData(QPainter& painter, double R, double r, double d, double th_max)
{
    QPen pen(QColor(0, 0, 0));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter.setPen(pen);

    QFont font("Arial", 12);
    painter.setFont(font);
    const QString cstrInputData("R = " + QString::number(R) + "\nr = " + QString::number(r) +
                                "\nd = " + QString::number(d) + "\nth = " + QString::number(th_max));
    const int cnFlags = Qt::AlignLeft;
    QRect rectText;
    QRect pixmap_rect = m_pixmap.rect();
    pixmap_rect.adjust(10, 30, 0, 0);
    painter.drawText(pixmap_rect, cnFlags, cstrInputData, &rectText);
    rectText.adjust(-5, -5, 5, 5);
    painter.drawRect(rectText);
}
void MainWindow::draw()
{
    double R{}, r{}, d{}, th_max{};
    if(m_pInputWin)
    {
        R = m_pInputWin->Radius();
        r = m_pInputWin->radius();
        d = m_pInputWin->distance();
        th_max = m_pInputWin->thetaMax();
    }

    if(qIsInf(R) || qIsNaN(R) || qIsNull(R) || qIsInf(r) || qIsNaN(r) || qIsNull(r) ||
            qIsInf(d) || qIsNaN(d) || qIsInf(th_max) || qIsNaN(th_max) )
        return;

    QPainter painter(&m_pixmap);
    drawHypertrochoid(painter, R, r, d, th_max);
    drawInputData(painter, R, r, d, th_max);
}
//-------------------------------------------------------------------------------------------------
void MainWindow::sUpdate()
{
    m_pixmap.fill(Qt::white);
    draw();
    update();
}
void MainWindow::sShowInputDataWin()
{
    m_pInputWin->show();
}
void MainWindow::sAbout(){ QMessageBox::about(this, "Гипотрохоида", "Рисование"); }
//-------------------------------------------------------------------------------------------------
