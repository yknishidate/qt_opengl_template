#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set dark gui
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(77,77,77));
    a.setPalette(darkPalette);

    MainWindow w;
    w.show();

    return a.exec();
}
