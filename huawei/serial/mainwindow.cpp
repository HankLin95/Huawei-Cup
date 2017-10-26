#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include<QDate>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->app=NULL;
    this->robot=NULL;
    this->temp=NULL;
    table_id=0;
    dish_name=new QStringList;
  //  dish_name<< "拌面" << "米饭" << "拉面" << "鱼香肉丝";
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    dish_name->append(QString("拌面"));
    dish_name->append(QString("米饭"));
    dish_name->append(QString("拉面"));
    dish_name->append(QString("鱼香肉丝"));
    server =new QTcpServer(this);
    server->listen(QHostAddress::Any,8088);
    server->setMaxPendingConnections(2);
    connect(server,SIGNAL(newConnection()),this,SLOT(newSocketConnection()));
    connect(ui->pushButton_send,SIGNAL(clicked()),this,SLOT(send_slot()));
    connect(ui->pushButton_send1,SIGNAL(clicked()),this,SLOT(send_dish_slot()));
    connect(ui->pushButton_send2,SIGNAL(clicked()),this,SLOT(send_dish_slot()));
    connect(ui->pushButton_send3,SIGNAL(clicked()),this,SLOT(send_dish_slot()));
    connect(ui->pushButton_send4,SIGNAL(clicked()),this,SLOT(send_dish_slot()));

    qDebug("init ok");

}
MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

void MainWindow::read_com() //读串口函数
{
    QByteArray temp = serial->readAll();
    //读取串口缓冲区的所有数据给临时变量temp
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    qDebug("length is %d",temp.length());
    if(temp.length()==13)
    {
        ui->textEdit->append(temp.toHex());
        char *temp_c=new char[27];
        temp_c=temp.toHex().data();
        char dev[3]={0};
        int j=0;
        for(int i=24;i<=25;i++)//7-10是设备号
        {
            dev[j++]=temp_c[i];
        }
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        qDebug("节点号是：%s",dev);
        table_id=atoi(dev);
        qDebug("%d",table_id);
        QString mes =QString(dev).append("号桌请求服务");
        QMessageBox::information(0,"服务请求",mes);
    }
    //将串口的数据显示在窗口的文本浏览器中
}

void MainWindow::on_pushButton_clicked()
{
    struct PortSettings myComSetting = {BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
    //定义一个结构体，用来存放串口各个参数
    serial = new Win_QextSerialPort("com4",myComSetting,QextSerialBase::EventDriven);
    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    serial ->open(QIODevice::ReadWrite);
    QString str("7E 42 C0 FF 00 FF 00 7E") ;
    QByteArray senddata;
    QstringToQbtyedata(str,senddata);
    serial->write(senddata);
    qDebug(senddata);
    //以可读写方式打开串口
    serial->readAll();
    connect(serial,SIGNAL(readyRead()),this,SLOT(read_com()));
    disconnect(ui->pushButton,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
}

void MainWindow::QstringToQbtyedata(QString str,QByteArray& senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toAscii();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toAscii();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}
char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void MainWindow::newSocketConnection()
{
    qDebug("new socket");
    temp=server->nextPendingConnection();
    if(temp==NULL)
    {
        qDebug("no ");
        return;
    }
   // disconnect(server,SIGNAL(newConnection()),this,SLOT(newSocketConnection()));
    qDebug("new client");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    qDebug("1");
    QString addr= temp->peerAddress().toString();
    qDebug("1");
    QStringList data_list=addr.split('.');
    qDebug("1");
    if(QString::compare(QString("1"),data_list.at(3))==0||QString::compare(QString("245"),data_list.at(3))==0)
    {
        if(robot)
        {
            qDebug("has robot");
            return ;
        }
        qDebug("is robot");
        robot=temp;
        ui->label_state->setText("已连接");
        qDebug("is route and robot");//是路由器的处理
        connect(robot,SIGNAL(disconnected()),this,SLOT(socket_disconnected()));
    }
    else//是安卓端的操作
    {
     /*   if(table_id==0)
        {
            qDebug("has no table need help");
            return;
        }*/
        if(app)
        {
            qDebug("has app");
            return ;
        }
        qDebug("is app");
        app=temp;
        ui->label_app->setText("已连接");
        connect(app,SIGNAL(readyRead()),this,SLOT(read_info()));
        connect(app,SIGNAL(disconnected()),this,SLOT(socket_disconnected()));
    }
    qDebug("the ip is %s\n",addr.toLatin1().data());
}
void MainWindow::read_info()
{
    qDebug("read_info");
    QByteArray info= app->readAll();
    QString data=QString(info.data());
    qDebug(data.toLatin1().data());
    if(QString::compare(data,QString("ok"))==0)
    {
        unsigned char data_back[3]={0};
        data_back[0]=48+table_id;
        data_back[1]='2';//回
        //robot->write((char*)data_back,2);
        QMessageBox::information(0,"error","send dish finish");
        return;
    }
    QStringList *data_list= new QStringList( data.split('/'));
    int data_num=data_list->count();
    qDebug(info.data());
    qDebug("%d",data_num);
    qDebug("%d桌需要的菜有:",table_id);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QString *data_dish=new QString();
    for(int i=0;i<data_num-1;i++)
    {
        data_dish->append(this->dish_name->at(data_list->at(i).toInt()-1));
        data_dish->append("\n");
    }
   /* if(robot==NULL)
    {
        QMessageBox::information(0,"error","has no robot");
        return ;
    }
    if(table_id==0)
    {
        QMessageBox::information(0,"error","has no table need help");
        return ;
    }*/
    QString *filename=new QString(QDate::currentDate().toString("yyyyMMdd")+QString(".log"));
    QFile *log=new QFile(*filename);
    log->open(QIODevice::Append|QIODevice::Text);//追加模式|文本模式（识别回车）

    QTextStream *out=new QTextStream(log);
    out->operator <<(*data_dish);
    log->close();
    unsigned char data_back[3]={0};
    data_back[0]=48+table_id;
    data_back[1]='2';//回。
    switch (table_id)
    {
        case 1:
        {
            ui->textEdit_table1->setText(*data_dish);
             break;
        }
        case 2:
        {
        ui->textEdit_table2->setText(*data_dish);
             break;
        }
        case 3:
        {
        ui->textEdit_table3->setText(*data_dish);
             break;
        }
        case 4:
        {
        ui->textEdit_table4->setText(*data_dish);
            break;
        }
        default:
            break;
    }
    this->table_id_list.append(table_id);
 //   robot->write((char*)data_back,2);
}

void MainWindow::socket_disconnected()
{
    qDebug("disconneted slot");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    if(((QTcpSocket*)sender())==this->robot)
    {
        qDebug("robot disconnect");
        robot=NULL;
        ui->label_state->setText("未连接");
    }
    else
    {
        app=NULL;
               qDebug("app disconnect");
        ui->label_app->setText("未连接");
    }
   // connect(server,SIGNAL(newConnection()),this,SLOT(newSocketConnection()));//重新接受连接
}

void MainWindow::send_slot()
{
    qDebug("send slot");
    if(robot==NULL)
    {
        QMessageBox::information(0,"error","has no robot");//路由器还未链接
        return ;
    }
    if(table_id==0)
    {
        QMessageBox::information(0,"error","has no table need help");//没有顾客按下ZIGBEE
        return ;
    }
    unsigned char data[3]={0};
    data[0]=48+table_id;
    data[1]='1';//去1桌。
    robot->write((char*)data,2);
    qDebug("send");
}

void MainWindow::send_dish_slot()
{
    qDebug("send_dish_slot");
    if(robot==NULL)
    {
        QMessageBox::information(0,"error","has no robot");
        return ;
    }
    if(table_id==0)
    {
        QMessageBox::information(0,"error","has no table need help");
        return ;
    }
    QPushButton *btn=(QPushButton*)sender();
    int table=0;
    if(btn==ui->pushButton_send1)
    {
        if(this->table_id_list.indexOf(1)!=-1)
        {
               table=1;
        }

    }
    else if(btn==ui->pushButton_send2)
    {
        if(this->table_id_list.indexOf(2)!=-1)
        {
               table=2;
        }
    }
    else if(btn==ui->pushButton_send3)
    {
        if(this->table_id_list.indexOf(3)!=-1)
        {
               table=3;
        }

    }
    else //第4桌
     {
        if(this->table_id_list.indexOf(4)!=-1)
        {
               table=4;
        }
    }
    if(table==0)
    {
        qDebug("该桌还未点餐");
        QMessageBox::information(0,"error","该桌还未点餐");
        return;
    }
    unsigned char data[3]={0};
    data[0]=48+table;
    data[1]='1';//去1桌。
    robot->write((char*)data,2);
    qDebug("send");
}
