#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qmessagebox.h>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialp=new QSerialPort;
    scene = new QGraphicsScene;
    mytime=new QTimer();
    sendtimer=new QTimer();

    QObject::connect(serialp,&QSerialPort::readyRead,this,&MainWindow::ReadData);
    QObject::connect(mytime,&QTimer::timeout,this,&MainWindow::timeon);
    QObject::connect(sendtimer,&QTimer::timeout,this,&MainWindow::sendtime);

    scene->setSceneRect(0, 0, 600, 500);
//        scene->addLine(0, 0, 30, 30);
//        scene->addLine(40, 40, 70, 70);
    //wai kuang
    scene->addRect(0,0,600,500);
    //zhong xian
    scene->addLine(0,250,600,250);

    ui->graphicsView->setScene(scene);// important!!!
}

MainWindow::~MainWindow()
{
    delete ui;
}
//refresh button
void MainWindow::on_pushButton_4_clicked()
{
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            int isHavaIteminlist=0;
            for(int i=0;i<ui->comboBox->count();i++)
            {
                isHavaIteminlist=1;
            }
            if(isHavaIteminlist==0)
            {
                ui->comboBox->addItem(serial.portName());
            }
            serial.close();
        }

    }
   // ui->comboBox->addItem();
}
//open button
void MainWindow::on_pushButton_2_clicked()
{   if(serialp->isOpen()==0)
    {

        //baudrate 9600
        serialp->setBaudRate(QSerialPort::Baud9600);
        //databit 8
        serialp->setDataBits(QSerialPort::Data8);
        //noFlowControl
        serialp->setFlowControl(QSerialPort::NoFlowControl);
        //parity no
        serialp->setParity(QSerialPort::NoParity);
        //stopbit 1
        serialp->setStopBits(QSerialPort::OneStop);

        //serial name
        serialp->setPortName(ui->comboBox->currentText());

        //readdata signal connect slot
        //QObject::connect(serialp,&QSerialPort::readyRead,this,&MainWindow::ReadData);

        //open serial
        serialp->open(QIODevice::ReadWrite);
        //label edit
        ui->label_5->setText(tr("ON"));

        //QObject::connect(serialp,&QSerialPort::readyRead,this,&MainWindow::ReadData);
    }
    else
    {
        QMessageBox::warning(this,tr("警告"),tr("已经打开一个串口"),QMessageBox::Ok);
    }

}
//close button
void MainWindow::on_pushButton_3_clicked()
{   if(serialp->isOpen())
    {
        serialp->clear();
        serialp->close();
        serialp->deleteLater();
        ui->label_5->setText(tr("OFF"));

    }
    else
    {
        QMessageBox::warning(this,tr("警告"),tr("串口已经关闭"),QMessageBox::Ok);
    }
}
//sned button
void MainWindow::on_pushButton_clicked()
{
    float kp=ui->lineEdit->text().toFloat();
    float ki=ui->lineEdit_2->text().toFloat();
    float kd=ui->lineEdit_3->text().toFloat();
    char temp[3]={0,0,0};
    temp[0]=(int)(kp*10)+192;
    temp[1]=(int)(ki*10)+128;
    temp[2]=(int)(kd*10)+64;
    serialp->write(temp,3);
//    //P
//    serialp->write(ui->lineEdit->text().toLatin1());
//    //I
//    serialp->write(ui->lineEdit_2->text().toLatin1());
//    //D
//    serialp->write(ui->lineEdit_3->text().toLatin1());
}
//readdata slot
//void MainWindow::ReadData()
//{
//    QByteArray buf;
//    char *d;
//    static int state=0;
//    unsigned int nowdata=0;
//    buf = serialp->readAll();
//    if(!buf.isEmpty())
//    {
//        d=buf.data();
//        //d[0]=d[0]&0xff;
//        qDebug("%x",d[0]);
//        switch(state)
//        {
//        case 0:if(d[0]==0xffffffff) {state=1;qDebug()<<"state1";}else {qDebug()<<"state0";}break;
//        case 1:if(d[0]==0xfffffffe) {state=2;qDebug()<<"state2";}else state=0;break;
//        case 2:{qDebug()<<"oko";nowdata=(d[0]&0xff)<<8;qDebug()<<nowdata;state=0;break;}
//        }


////        if(d[0]==0xffffffff)
////        {
////            qDebug()<<"ok";
////        }
////        qDebug("%x",d[0]);
////    QString str = ui->textEdit->toPlainText();
////    str+=tr(buf);
////    ui->textEdit->clear();
////   ui->textEdit->append(str);

//    }
//    buf.clear();

//}
//void MainWindow::ReadData()
//{
//    static int dataCount=0;
//    static int lastData=0;
//    QByteArray buf;
//    buf = serialp->readAll();
//    if(!buf.isEmpty())
//    {
//        int nowData=(int)recData+200;

//        if(dataCount==0)
//        {
//            lastData=nowData;
//        }
//        else
//        {
//            scene->addLine(9+dataCount,lastData,10+dataCount,nowData);
//        }

//        dataCount++;
//        if(dataCount>580)
//        {
//            dataCount=0;
//            scene->clear();
//        }
//    }
//    buf.clear();

//}
void MainWindow::ReadData()
{
    static int dataCount=0;
    static int lastData=0;
    static int stateRec=0;
    static unsigned int Hnum=0,Lnum=0;
    QByteArray buf;
    char *dataByte=(char*)malloc(1*sizeof(char));
    //char rec=0;
    int recData=0;
    static int disData=0;
    buf = serialp->readAll();
    if(!buf.isEmpty())
    {
        dataByte=buf.data();
        //rec=
        switch(stateRec)
        {
        case 0:if(dataByte[0]==0xffffffff)
                {
                    stateRec=1;
                  //  qDebug()<<"oxff1";
                }
                else if(dataByte[0]==0xfffffffe)
                {
                    stateRec=4;
                  //  qDebug()<<"oxfe1";
                }break;
        case 1:if(dataByte[0]==0xffffffff)
                {
                    stateRec=2;
                 //   qDebug()<<"oxff2";
                }
                else
                {
                    stateRec=0;
                 //   qDebug()<<"oxff1return";
                };break;
        case 2:{Hnum=dataByte[0]&(0xff);stateRec=3;break;}
        case 3:
            {
                Lnum=dataByte[0]&(0xff);
                recData=(Hnum<<8)+Lnum;
                disData=recData;
              //  qDebug()<<"zhengshu";
              //  qDebug()<<recData;
                stateRec=0;
                int nowData=250-(int)(recData/20.0);
                if(dataCount==0)
                {
                    lastData=nowData;
                }
                else
                {
                    if(nowData<1) nowData=lastData;
                //    qDebug()<<"huitu************************************";
                    scene->addLine(9+dataCount,lastData,10+dataCount,nowData);
                    ui->graphicsView->setScene(scene);
                    lastData=nowData;
                }

                dataCount++;
                if(dataCount>580)
                {
                    dataCount=0;
                    scene->clear();
                    scene->addRect(0,0,600,500);
                    //zhong xian
                    scene->addLine(0,250,600,250);
                    ui->graphicsView->setScene(scene);
                }
                break;
            }
        case 4:if(dataByte[0]==0xfffffffe)
                {
                    stateRec=5;
                //    qDebug()<<"oxfe2";
                }
                else
                {
                    stateRec=0;
               //     qDebug()<<"oxfe1return";
                }break;
        case 5:{Hnum=dataByte[0]&(0xff);stateRec=6;break;}
        case 6:
            {
                Lnum=dataByte[0]&(0xff);
                recData=(Hnum<<8)+Lnum;
                disData=-recData;
             //   qDebug()<<"fushu";
             //   qDebug()<<recData;
                stateRec=0;
                int nowData=250+(int)(recData/20.0);
                if(dataCount==0)
                {
                    lastData=nowData;
                }
                else
                {
                    if(nowData>499) nowData=lastData;
               //     qDebug()<<"huitu************************************";
                    scene->addLine(9+dataCount,lastData,10+dataCount,nowData);
                    ui->graphicsView->setScene(scene);
                    lastData=nowData;
                }

                dataCount++;
                if(dataCount>580)
                {
                    dataCount=0;
                    scene->clear();
                    scene->addRect(0,0,600,500);
                    //zhong xian
                    scene->addLine(0,250,600,250);
                    ui->graphicsView->setScene(scene);
                }
                break;
            }

        }
        if(dataCount%3==0)
               ui->lineEdit_4->setText(QString::number(disData/100.0));



    }
    buf.clear();
}


// timer
void MainWindow::on_pushButton_5_clicked()
{
    static int keyflag=0;
    if(keyflag==0)
    {
        mytime->start(50);
        ui->pushButton_5->setText(tr("Time:end"));
        keyflag=1;
    }
    else
    {
        mytime->stop();
        ui->pushButton_5->setText(tr("Time:start"));
        keyflag=0;
    }


}
void MainWindow::timeon()
{
    static float timedis=0;
    mytime->stop();
    timedis+=0.05;
    ui->lineEdit_5->setText(QString::number(timedis));
    mytime->start(50);
}

//camera button
void MainWindow::on_pushButton_6_clicked()
{
    static int cameraState=0;
    char temp[1]={0x3f};
    serialp->write(temp,1);
    switch(cameraState)
    {
    case 0:{
           ui->pushButton_6->setText(tr("camera:on"));
           cameraState=1;
           break;
    }
    case 1:{
            ui->pushButton_6->setText(tr("camera:off"));
            cameraState=0;
            break;
    }
    default:break;
    }
}

void MainWindow::on_dial_valueChanged(int value)
{
    if(serialp->isOpen())
    {
        char temp[1]={0x00};
        temp[0]=(char)value/2;
        serialp->write(temp,1);
    }
}

void MainWindow::on_dial_sliderPressed()
{
 //   dialFlag=0;
//    if(serialp->isOpen())
//    {
// //       while(dialFlag!=1)
//        {
//            char temp[1]={0x00};
//            temp[0]=char(ui->dial->value());
//            serialp->write(temp,1);
//        }
//    }
    sendtimer->start(50);
}

void MainWindow::on_dial_sliderReleased()
{
//    dialFlag=1;
    sendtimer->stop();
    char temp[1]={0x3e};
    serialp->write(temp,1);
}
void MainWindow::sendtime()
{
    char temp[1]={0x00};
    temp[0]=(char)(ui->dial->value())/2;
    serialp->write(temp,1);
}
