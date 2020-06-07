#include "main_window.h"

#include <QApplication>

int main(int nArgC, char *apszArgV[])
{
    QApplication app(nArgC, apszArgV);
    //
    MainWindow mainWindow;
    mainWindow.show();
    //
    return app.exec();
}
