#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QHostAddress>
#include<QMessageBox>
#include<QTextCodec>
#include<QTableWidget>
#include<QStringList>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dish_num=0;
    this->receiver_return=new QUdpSocket(this);
    receiver_return->bind(8089);
    ui->dish_table->setColumnCount(2);
    ui->dish_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dish_table->setRowCount(6);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    ui->dish_table->setHorizontalHeaderLabels(QStringList()<<"菜品编号"<<"菜名");
    connect(receiver_return,SIGNAL(readyRead()),this,SLOT(send_return()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString dish=ui->dish_combo->itemText(ui->dish_combo->currentIndex());
    qDebug(dish.toLocal8Bit().data());
    this->sender=new QUdpSocket(this);
    sender->connectToHost(QHostAddress::Broadcast,8088);
    sender->write(dish.toLocal8Bit().data(),strlen(dish.toLocal8Bit().data()));
}


void MainWindow::send_return()
{
    QByteArray *datagram=new QByteArray;
    while(receiver_return->hasPendingDatagrams())
    {
        datagram->resize(receiver_return->pendingDatagramSize());
        receiver_return->readDatagram(datagram->data(),datagram->size());
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
        qDebug(datagram->data());
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    }
    if(strcmp("yes",datagram->data())==0)
    {
            QMessageBox::information(this,"senddish","送餐成功");
    }
    else
    QMessageBox::information(this,"senddish","送餐失败");
}

void MainWindow::on_dish_combo_activated(int index)
{
    if(dish_num>=6)
    {
        ui->dish_table->setRowCount(dish_num+1);
    }
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    qDebug(ui->dish_combo->itemText(index).toLocal8Bit().data());
    ui->dish_table->setItem(dish_num,0,new QTableWidgetItem(QString::number(index)) );
    ui->dish_table->setItem(dish_num++,1,new QTableWidgetItem(ui->dish_combo->itemText(index).toLocal8Bit().data()) );
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}
