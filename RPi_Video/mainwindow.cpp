#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
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
}

void MainWindow::replyAvail(QNetworkReply *reply)
{
    ui->logger->append("Read success!");

    if (reply->error() == QNetworkReply::NoError) {
            //success
            ui->logger->append(QString("Bytes Avail: %1").arg(reply->bytesAvailable()));

            QByteArray data = reply->readAll();
            int startIdx = data.indexOf(QByteArray("\xff\xd8"));
            int stopIdx = data.indexOf(QByteArray("\xff\xd9"));

            ui->logger->append(QString("JPG: Start=%1, Stop=%2").arg(startIdx).arg(stopIdx));
            cv::Mat jpg = cv::imdecode(std::vector<uchar>(data.begin(), data.end()), CV_LOAD_IMAGE_COLOR);
            cv::cvtColor(jpg, jpg, CV_BGR2RGB);
            QImage videoImg = QImage((uchar*)jpg.data, jpg.cols, jpg.rows, static_cast<int>(jpg.step), QImage::Format_RGB888);
            QPixmap videoPixmap = QPixmap::fromImage(videoImg);

            ui->videoFeed->setPixmap(videoPixmap);
        }
    else {
        //failure
         ui->logger->append("Failure: " + reply->errorString());
    }

    reply->deleteLater();
}

void MainWindow::tcpDataReceive()
{
    ui->logger->append(QString("Bytes Avail: %1").arg(socket->bytesAvailable()));

    // Find the indexes of start/stop of jpg dataload
    // Then use imdecode to generate a Mat object for that image
    // Transform that Mat to a QPixmap and display

    // NOTE: How to read sequentially with proper fps, particularly when saving to file
    // NOTE: When read, is it possible to get a timestamp for each frame?
//    QByteArray data = QByteArray::fromHex(tcpSocket->readAll());


}

void MainWindow::on_refreshBtn_clicked()
{
//   mgr->get(QNetworkRequest(QUrl("http://169.254.0.2:8080/?action=snapshot")));

   socket->connectToHost("169.254.0.2", 8080);
   socket->write("GET /?action=stream\r\n\r\n");
}

MainWindow::~MainWindow()
{
    delete ui;
}
