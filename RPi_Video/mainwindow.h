#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QThread>

#include "videomgr.h"
#include "voicemgr.h"
#include "lsmmgr.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Video
    QThread *videoTh;
    VideoMgr *video;

    // Audio
    QThread *voiceTh;
    VoiceMgr *voice;

    // LSM9D
    QThread *lsmTh;
    LsmMgr *lsm;

private slots:
    void on_startBtn_clicked();
    void updateFps(quint16 fps);
    void logToVideo(QString msg);
    void logToAudio(QString msg);
    void updateVideoFeed(QPixmap frame);
};

#endif // MAINWINDOW_H
