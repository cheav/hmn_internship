#include "main_win.h"

#include <QApplication>
#include <QScopedPointer>
#include <QMessageBox>
#include <QTranslator>

int main(int nArgC, char *apszArgV[])
{
    QApplication app(nArgC, apszArgV);
    //
    bool bTranslationLoaded = false;
    QTranslator translator;
    const char *aszTranslationDirs[] =
    {
        //".", "./mvc", "../translations", ":/translations", 0
        ".", "./mvc", "../translations", ":/translations", 0
        //".", "../translations", 0
    };
    for(const char **ppszDir = aszTranslationDirs; *ppszDir != 0; ++ppszDir)
    {
        bTranslationLoaded
                = translator.load(
                    QLocale::system(),
                    "mvc-04", "_", *ppszDir);
        if (bTranslationLoaded)
            break;
    }
    //
    if (bTranslationLoaded)  app.installTranslator(&translator);
    //else  QMessageBox::warning(0, "Translation", "Not loaded");
    //
    QScopedPointer <MainWin> mainWindow(new MainWin);
    mainWindow->show();
    //
    return app.exec();
}
