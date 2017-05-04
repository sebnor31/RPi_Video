#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

void MainWindow::on_startBtn_clicked()
{
    // Video
    videoTh = new QThread(this);
    video = new VideoMgr();
    video->moveToThread(videoTh);

    connect(videoTh, &QThread::started, video, &VideoMgr::start);
    connect(video, &VideoMgr::newFrameSig, this, &MainWindow::updateVideoFeed);
    connect(video, &VideoMgr::newFpsSig, this, &MainWindow::updateFps);
    connect(video, &VideoMgr::logMsg, this, &MainWindow::logToVideo);
    connect(videoTh, &QThread::finished, video, &VideoMgr::deleteLater);
    connect(video, &VideoMgr::finished, videoTh, &QThread::deleteLater);

    videoTh->start();


//    // Audio
//    voiceTh = new QThread(this);
//    voice = new VoiceMgr();
//    voice->moveToThread(voiceTh);

//    connect(voiceTh, &QThread::started, voice, &VoiceMgr::start);
//    connect(voice, &VoiceMgr::logMsg, this, &MainWindow::logToAudio);
//    connect(voiceTh, &QThread::finished, voice, &VoiceMgr::deleteLater);
//    connect(voice, &VoiceMgr::finished, voiceTh, &QThread::deleteLater);

//    voiceTh->start();

    // LSM9D
    lsmTh = new QThread(this);
    lsm = new LsmMgr();
    lsm->moveToThread(lsmTh);

    connect(lsmTh, &QThread::started, lsm, &LsmMgr::start);
    connect(lsm, &LsmMgr::logMsg, this, &MainWindow::logToAudio);
    connect(lsmTh, &QThread::finished, lsm, &LsmMgr::deleteLater);
    connect(lsm, &LsmMgr::finished, lsmTh, &QThread::deleteLater);

    lsmTh->start();

    // Disable start
    ui->startBtn->setEnabled(false);
}

void MainWindow::updateFps(quint16 fps)
{
    ui->fpsInd->setText(QString::number(fps));
}

void MainWindow::logToVideo(QString msg)
{
    ui->videoLogger->append(msg);
}

void MainWindow::logToAudio(QString msg)
{
    ui->audioLogger->append(msg);
}

void MainWindow::updateVideoFeed(QPixmap frame)
{
    ui->videoFeed->setPixmap(frame);
}

MainWindow::~MainWindow()
{
    videoTh->quit();
    voiceTh->quit();

    videoTh->wait();
    voiceTh->wait();

    delete ui;
}


