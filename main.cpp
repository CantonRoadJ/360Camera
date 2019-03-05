#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <unistd.h>
#include <QImage>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    QSplashScreen *splash = new QSplashScreen;
    splash->setBaseSize(1280,720);
    splash->setFixedSize(1280,720);
    splash->setPixmap(QPixmap(":/背景3.png"));
    splash->setPixmap(QPixmap(":/背景3.png"));
    splash->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::WindowFullscreenButtonHint);
    splash->showFullScreen();
    QCoreApplication::processEvents();

    MainWindow w;
    w.setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint |Qt::WindowFullscreenButtonHint);

//    QPalette palette;
//    palette.setBrush(QPalette::Background, QBrush(QImage(":/背景3.png")));
//    w.setPalette(palette);

    w.showFullScreen();

    splash->finish(&w);
    delete splash;

    QCoreApplication::processEvents();

    return a.exec();
}
