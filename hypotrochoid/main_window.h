#ifndef MAIN_WINDOW_H__
#define MAIN_WINDOW_H__
#include <QMainWindow>
#include "input_win.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *pParent = nullptr);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *pEvent) override;
    void paintEvent(QPaintEvent *pEvent) override;
    void closeEvent(QCloseEvent *pEvent) override;
private slots:
    void sShowInputDataWin();
    void sUpdate();
    void sAbout();
private:
    void drawHypertrochoid(QPainter& painter, double R, double r, double d, double th_max);
    void drawInputData(QPainter& painter, double R, double r, double d, double th_max);
    void draw();
    //
    InputDialog *m_pInputWin;
    QPixmap m_pixmap;
};

#endif // MAIN_WINDOW_H__
