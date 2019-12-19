/*-------------------------------------------------
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>

namespace Ui {
class MainWindow;
}
struct libvlc_media_track_info_t;
struct libvlc_media_t;
struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool load(const QString &filepath);//加载
    void play();//播放
    void pause();//暂停
    void stop();//停止
    bool loadVlc(const QString &filepath);//加载
    void freeVlc();//释放

protected:
    static void vlcEvents(const libvlc_event_t *ev, void *param);//VLC事件
private slots:
    void on_pushButton_clicked();//播放/暂停
    void on_pB_Stop_clicked();//停止
    void on_pB_Open_clicked();//打开文件

private:
    Ui::MainWindow *ui;

    libvlc_instance_t *m_vlcInstance = Q_NULLPTR;
    libvlc_media_t *m_media = Q_NULLPTR;
    libvlc_media_player_t *m_mediaPlayer = Q_NULLPTR;

    QFileDialog *m_fileDialog = Q_NULLPTR;
    QTimer *m_timer = Q_NULLPTR;

    QMutex m_mutex;
    qint64 m_duration = 0;
    bool m_bPlaying = false;
};

#endif // MAINWINDOW_H
