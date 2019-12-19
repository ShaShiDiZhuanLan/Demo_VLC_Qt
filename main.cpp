/*-----------Qt+VLC简单的使用显示视频Demo--------------
#
# Project created by QtCreator
# Author: 沙振宇
# CreateTime: 2017-05-23
# UpdateTime: 2019-12-19
# Info: Qt+VLC简单的使用显示视频Demo
# Url:https://shazhenyu.blog.csdn.net/article/details/72673677
# Github:https://github.com/ShaShiDiZhuanLan/Demo_VLC_Qt
#
#-------------------------------------------------*/
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
