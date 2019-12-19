#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include "vlc/vlc.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_fileDialog = new QFileDialog(this);
    m_fileDialog->setWindowTitle(tr("Open Video"));
    m_fileDialog->setDirectory(".");
    m_fileDialog->setNameFilter(tr("Video Files(*.mp4 *.mp3 *flv *3gp *rmvb)"));
    m_fileDialog->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::load(const QString &filepath)
{
    if(filepath.isEmpty()){
        qDebug() << "filepath isEmpty";
        return false;
    }
    if(!QFile::exists(filepath)){
        qDebug() << "QFile no exists";
        return false;
    }
    bool bload = loadVlc(QDir::toNativeSeparators(filepath).toUtf8().constData());
    if(!bload){
        qDebug() << "bload no isEmpty";
        return false;
    }
    return true;
}

void MainWindow::play()
{
    if(libvlc_Ended == libvlc_media_get_state(m_media)){
        stop();//vlc这个设定太奇怪了，当自然播放结束，重新播放还得主动调用一次stop();
    }
    libvlc_media_player_play(m_mediaPlayer);
}

void MainWindow::pause()
{
    if((bool)libvlc_media_player_is_playing(m_mediaPlayer)){
        m_bPlaying = false;
        libvlc_media_player_set_pause(m_mediaPlayer, 1);//暂停
    }
}

void MainWindow::stop()
{
    m_bPlaying = false;
    libvlc_media_player_stop(m_mediaPlayer);
}

bool MainWindow::loadVlc(const QString &filepath)
{
    m_vlcInstance = libvlc_new(0, NULL);
    m_media = libvlc_media_new_path(m_vlcInstance, filepath.toUtf8().data());
    if(!m_media){
        freeVlc();
        return m_media;
    }
    qDebug() << "loadVlc" << qPrintable(filepath);
    m_mediaPlayer = libvlc_media_player_new_from_media(m_media);
    if(!m_mediaPlayer){
        freeVlc();
        return false;
    }
    libvlc_media_parse(m_media);
    libvlc_event_manager_t *em = libvlc_media_player_event_manager(m_mediaPlayer);
    libvlc_event_attach(em, libvlc_MediaPlayerTimeChanged, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerEndReached, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerStopped, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerPlaying, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerPaused, vlcEvents, this);

    libvlc_media_player_set_hwnd(m_mediaPlayer, (void *)ui->label->winId());
    return true;
}
void MainWindow::vlcEvents(const libvlc_event_t *ev, void *param)
{
    Q_UNUSED(param);
    switch (ev->type) {
    case libvlc_MediaPlayerTimeChanged:
        break;
    case libvlc_MediaPlayerEndReached://不能在回调里调用stop();否则会卡死
        break;
    case libvlc_MediaPlayerStopped:
        qDebug() << "libvlc_MediaPlayerStopped";
        break;
    case libvlc_MediaPlayerPlaying:
        qDebug() << "libvlc_MediaPlayerPlay";
        break;
    case libvlc_MediaPlayerPaused:
        qDebug() << "libvlc_MediaPlayerPaused";
        break;
    }
}
void MainWindow::freeVlc()
{
    if(m_media){
        libvlc_media_release(m_media);
        m_media = Q_NULLPTR;
    }
    if(m_mediaPlayer){
        libvlc_media_player_stop(m_mediaPlayer);
        libvlc_media_player_release(m_mediaPlayer);
        m_mediaPlayer = Q_NULLPTR;
    }
    if(m_vlcInstance){
        libvlc_release(m_vlcInstance);
        m_vlcInstance = Q_NULLPTR;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "play") {
        play();
        ui->pushButton->setText("pause");
    } else if(ui->pushButton->text() == "pause") {
        pause();
        ui->pushButton->setText("play");
    }
}

void MainWindow::on_pB_Stop_clicked()
{
    stop();
    ui->pushButton->setText("play");
}

void MainWindow::on_pB_Open_clicked()
{
    if(m_vlcInstance) {
        freeVlc();
    }
    m_fileDialog->show();
    if(m_fileDialog->exec() == QDialog::Accepted) {
        QString path = m_fileDialog->selectedFiles()[0];
        ui->lB_Path->setText(path);
        load(path);
    }
}
