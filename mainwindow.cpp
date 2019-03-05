#include "mainwindow.h"
#include "loopuptable.h"
#include "g2d_thread.h"
#include <unistd.h>
#include <QThread>
#include <QTime>

#define topCameraWhich 0
#define bottomCameraWhich 1
#define leftCameraWhich 2
#define rightCameraWhich 3

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QTime time;
    time.start();

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QImage(":/背景3.png")));
    this->setPalette(palette);

    //初始化查找表
    if (initLoopupTable() == true)
    {
        qDebug() << "initLoopupTable is ok";
    }

    //计算并显示线程
    caleThread = new G2DThread();
    caleThread->setWhichCamera(topCameraWhich);
    caleThread->init();

    topButton = new QPushButton(this);
//    topButton->setGeometry(500,623,120,80);
    topButton->setGeometry(0,0,80,80);

    topButton->setText("前");
//    topButton->setAttribute(Qt::WA_TranslucentBackground);
    topButton->show();
    connect(topButton,SIGNAL(clicked(bool)),this,SLOT(TopFisheyeView()));

    bottomButton = new QPushButton(this);
//    bottomButton->setGeometry(650,623,120,80);
    bottomButton->setGeometry(0,120,80,80);
    bottomButton->setText("后");
//    bottomButton->setAttribute(Qt::WA_TranslucentBackground);
    bottomButton->show();
    connect(bottomButton,SIGNAL(clicked(bool)),this,SLOT(BottomFisheyeView()));

    leftButton = new QPushButton(this);
//    leftButton->setGeometry(800,623,120,80);
    leftButton->setGeometry(0,240,80,80);
    leftButton->setText("左");
//    leftButton->setAttribute(Qt::WA_TranslucentBackground);
    leftButton->show();
    connect(leftButton,SIGNAL(clicked(bool)),this,SLOT(LeftFisheyeView()));

    rightButton = new QPushButton(this);
//    rightButton->setGeometry(950,623,120,80);
    rightButton->setGeometry(0,360,80,80);
    rightButton->setText("右");
//    rightButton->setAttribute(Qt::WA_TranslucentBackground);
    rightButton->show();
    connect(rightButton,SIGNAL(clicked(bool)),this,SLOT(RightFisheyeView()));

    exitButton = new QPushButton(this);
//    exitButton->setGeometry(1100,623,180,80);
    exitButton->setGeometry(0,480,80,80);
    exitButton->setText("exit");
//    exitButton->setAttribute(Qt::WA_TranslucentBackground);
    exitButton->show();
    connect(exitButton,SIGNAL(clicked(bool)),this,SLOT(CloseWindows()));



    qDebug()<<"all function is ok"<<time.elapsed()/1000.0<<"s";
}

void MainWindow::TopFisheyeView()
{
    caleThread->setWhichCamera(topCameraWhich);
}

void MainWindow::BottomFisheyeView()
{
    caleThread->setWhichCamera(bottomCameraWhich);
}

void MainWindow::LeftFisheyeView()
{
    caleThread->setWhichCamera(leftCameraWhich);
}

void MainWindow::RightFisheyeView()
{
    caleThread->setWhichCamera(rightCameraWhich);
}

void MainWindow::CloseWindows()
{
    //cameraFourInThread->exit(0);
    caleThread->ClearScreen();
    ::usleep(100000);
    caleThread->quit();
    caleThread->exit(0);
//    caleThread->ClearScreen();

//    caleThread->quit();
//    caleThread->exit(0);
//    caleThread->~G2DThread();
    this->close();
    exit(0);
//   this->~MainWindow();
}

MainWindow::~MainWindow()
{
    caleThread->ClearScreen();
    ::usleep(100000);
    caleThread->quit();
    caleThread->exit(0);
//    caleThread->ClearScreen();
//    caleThread->~G2DThread();
    this->close();
    exit(0);
}
