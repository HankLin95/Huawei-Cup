#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>
#include<QStringList>
#include"win_qextserialport.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QStringList *dish_name;

    Win_QextSerialPort *serial;
    char ConvertHexChar(char ch);
    void QstringToQbtyedata(QString str,QByteArray& senddata);
private:
    Ui::MainWindow *ui;
  //  QList <QTcpSocket *> socketList;
    QList<int> table_id_list;
    QTcpSocket *robot;
      QTcpSocket *app;
    QTcpServer *server;
    QTcpSocket*temp;
    int table_id;
    void add_dish(int table,QStringList *dish);
private slots:
    void read_com();//读取串口信息
    void on_pushButton_clicked();
    void  newSocketConnection();
    void read_info();
    void socket_disconnected();
    void send_slot();
    void send_dish_slot();
};

#endif // MAINWINDOW_H
