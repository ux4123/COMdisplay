#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtSerialPort/QSerialPort"
#include "QtSerialPort/QSerialPortInfo"
#include "QGraphicsScene"
#include <QDebug>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void ReadData();
    void on_pushButton_5_clicked();
    void timeon();
    void sendtime();

    void buttonTime();

    void on_pushButton_6_clicked();

    void on_dial_valueChanged(int value);

    void on_dial_sliderPressed();

    void on_dial_sliderReleased();

    void on_pushButton_7_clicked();

private:
    QGraphicsScene *scene;
    QTimer *mytime;
    QTimer *sendtimer;
    QTimer *controlTimer;
//    int dialFlag=0;
private:
    Ui::MainWindow *ui;
    QSerialPort *serialp;
};

#endif // MAINWINDOW_H

