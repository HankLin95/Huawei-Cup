#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("饿了么餐厅机器人后台");
    w.show();
    
    return a.exec();
}
