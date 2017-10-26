#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbsingleton.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->receiver=new QUdpSocket(this);
    receiver->bind(8088,QUdpSocket::ShareAddress);
    connect(receiver,SIGNAL(readyRead()),this,SLOT(read_data()));

}
void MainWindow::read_data()
{
    datagram=new QByteArray;
    while(receiver->hasPendingDatagrams())
    {
        datagram->resize(receiver->pendingDatagramSize());
        receiver->readDatagram(datagram->data(),datagram->size());
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
      qDebug(datagram->data());
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    }
    this->send_dish_command();
}

void MainWindow::send_dish_command()
{
      QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    qDebug(datagram->data());
    dbsingle*db=dbsingle::getinstace("dish.db");
    char **result=new char*;
    int col,row;
    int id;
    char sql[100]={0};
    sprintf(sql,"select id from dish  where name=\'%s\' and state=0 limit 1 offset 0 ",datagram->data());
    qDebug(sql);
    db->gettable(sql,&result,col,row);
    if(row!=0)
   {
        for(int i=0;i<=row;i++)
    {
        for(int j=0;j<col;j++)
        {
            qDebug(result[j+i*col]);
        }
    }
    }
    else
    {
        QUdpSocket *send_succes=new QUdpSocket(this);
        send_succes->connectToHost(QHostAddress::Broadcast,8089);
        send_succes->write("no",2);
        return ;
    }
    id=QString(result[1]).toInt();
    /*获取第一位点餐的信息*/
    memset(sql,0,100);
    sprintf(sql,"update dish set state =1 where id=%d",id);
    db->gettable(sql,&result,col,row);
    qDebug("ss");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    qDebug("开始送餐");
    QUdpSocket *send_succes=new QUdpSocket(this);
    send_succes->connectToHost(QHostAddress::Broadcast,8089);
    send_succes->write("yes",3);
}
MainWindow::~MainWindow()
{
    delete ui;
}
