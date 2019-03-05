#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPicture>
#include <QPaintEvent>
#include <QDebug>
#include <QMutex>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPushButton>
#include "v4l2_work_fourin.h"

#include "g2d.h"
#include "g2d_yuyv_bgra.h"

#include "g2d_thread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void CloseWindows();
    void TopFisheyeView();
    void BottomFisheyeView();
    void LeftFisheyeView();
    void RightFisheyeView();

private:
    int srcWidth = 1280;
    int srcheight = 800;

    int imageWidth = 420;
    int imageHeight = 720;

    G2DThread *caleThread;

    QPushButton *exitButton;
    QPushButton * topButton;
    QPushButton * bottomButton;
    QPushButton * leftButton;
    QPushButton * rightButton;
};

#endif // MAINWINDOW_H
