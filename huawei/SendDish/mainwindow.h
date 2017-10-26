#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void send_return();



    void on_dish_combo_activated(int index);

private:
    int dish_num;
    Ui::MainWindow *ui;
    QUdpSocket *sender;
    QUdpSocket *receiver_return;
};

#endif // MAINWINDOW_H
