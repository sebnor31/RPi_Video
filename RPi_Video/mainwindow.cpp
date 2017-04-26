#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    mgr = new QNetworkAccessManager(this);
//    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyAvail(QNetworkReply*)));

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(tcpDataReceive()));
    connect(this, &MainWindow::updatedStream, this, &MainWindow::processStream);
}

void MainWindow::on_startBtn_clicked()
{
    socket->connectToHost("169.254.0.2", 8080);
    socket->write("GET /?action=stream\r\n\r\n");

    ui->startBtn->setEnabled(false);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFps);
}

void MainWindow::updateFps()
{
    ui->fpsInd->setText(QString::number(frameCnter));
    frameCnter = 0;
}

void MainWindow::tcpDataReceive()
{
    stream.append(socket->readAll());
    emit updatedStream();
}

void MainWindow::processStream()
{
    int startIdx = stream.indexOf(QByteArray("\xff\xd8"));
    int stopIdx = stream.indexOf(QByteArray("\xff\xd9"));

    if (startIdx != -1 && stopIdx != -1){
        ui->logger->append(QString("JPG: Start=%1, Stop=%2").arg(startIdx).arg(stopIdx));

        // Extract the first frame and Convert from QByteArray to cv::Mat object
        QByteArray frame = stream.mid(startIdx, (stopIdx - startIdx) + 2);
        cv::Mat matFrame(1, frame.size(), CV_8UC1, frame.data());

        // Decode data to a JPG frame
        cv::Mat jpg = cv::imdecode(matFrame, CV_LOAD_IMAGE_COLOR);
        cv::cvtColor(jpg, jpg, CV_BGR2RGB);     // OpenCV uses BGR by default, need to convert to RGB

        // Display the JPG image to our GUI video feed
        QImage videoImg = QImage((uchar*)jpg.data, jpg.cols, jpg.rows, static_cast<int>(jpg.step), QImage::Format_RGB888);
        QPixmap videoPixmap = QPixmap::fromImage(videoImg);
        ui->videoFeed->setPixmap(videoPixmap);

        // Remove this frame from our stream container to reduce memory consumption
        stream.remove(0, stopIdx+1);

        // Start FPS estimation only when a frame has been found for the 1st time
        if (!timer->isActive()){
            timer->start();
        }
        frameCnter++;
    }
}

MainWindow::~MainWindow()
{
    timer->stop();
    socket->disconnectFromHost();
    delete ui;
}


