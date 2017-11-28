#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
//    w.showMaximized();

//    printf("max height %d width %d \n",w.maximumHeight(),w.maximumWidth());

    w.show();

    return a.exec();
}
