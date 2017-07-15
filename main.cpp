#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setMinimumSize(500, 380);
    w.setWindowTitle("快递查询");
    w.show();

    return a.exec();
}
