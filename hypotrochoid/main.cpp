#include <QApplication>
#include "main_window.h"

int main(int nArgC, char *apszArgV[])
{
    QApplication app(nArgC, apszArgV);

    MainWindow w;
    w.show();

    return app.exec();
}
