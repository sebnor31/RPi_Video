#include "videomgr.h"
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

VideoMgr::VideoMgr(QObject *parent) : QObject(parent)
{
    frameCnter = 0;
    stream.clear();
}

void VideoMgr::start()
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &VideoMgr::dataReceive);
    connect(this, &VideoMgr::updatedStream, this, &VideoMgr::processStream);
    connect(socket, &QTcpSocket::stateChanged, this, &VideoMgr::stateChanged);

    socket->connectToHost("192.168.7.2", 8080);
    socket->write("GET /?action=stream\r\n\r\n");

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &VideoMgr::updateFps);
}

void VideoMgr::stateChanged(QAbstractSocket::SocketState state)
{
    QString msg = "";

    if (state == QAbstractSocket::UnconnectedState){
        msg = "Unconnected";
    }
    else if (state == QAbstractSocket::HostLookupState){
        msg = "Host name lookup";
    }
    else if (state == QAbstractSocket::ConnectingState){
        msg = "Estalishing connection...";
    }
    else if (state == QAbstractSocket::ConnectedState){
        msg = "Connection established with host";
    }
    else if (state == QAbstractSocket::BoundState){
        msg = "Socket bound to an address and port";
    }
    else if (state == QAbstractSocket::ClosingState){
        msg = "Closing connection";
    }
    else {
        msg = "ERROR: State not recognized";
    }

    emit logMsg(msg);
}

void VideoMgr::dataReceive()
{
    stream.append(socket->readAll());
    emit updatedStream();
}

void VideoMgr::processStream()
{
    int startIdx = stream.indexOf(QByteArray("\xff\xd8"));
    int stopIdx = stream.indexOf(QByteArray("\xff\xd9"));

    if (startIdx != -1 && stopIdx != -1){
        emit logMsg(QString("JPG: Start=%1, Stop=%2").arg(startIdx).arg(stopIdx));

        // Extract the first frame and Convert from QByteArray to cv::Mat object
        QByteArray frame = stream.mid(startIdx, (stopIdx - startIdx) + 2);
        cv::Mat matFrame(1, frame.size(), CV_8UC1, frame.data());

        // Decode data to a JPG frame
        cv::Mat jpg = cv::imdecode(matFrame, CV_LOAD_IMAGE_COLOR);
        cv::cvtColor(jpg, jpg, CV_BGR2RGB);     // OpenCV uses BGR by default, need to convert to RGB

        // Display the JPG image to our GUI video feed
        QImage videoImg = QImage((uchar*)jpg.data, jpg.cols, jpg.rows, static_cast<int>(jpg.step), QImage::Format_RGB888);
        emit newFrameSig(QPixmap::fromImage(videoImg));

        // Remove this frame from our stream container to reduce memory consumption
        stream.remove(0, stopIdx+1);

        // Start FPS estimation only when a frame has been found for the 1st time
        if (!timer->isActive()){
            timer->start();
        }
        frameCnter++;
    }

}

void VideoMgr::updateFps()
{
    emit newFpsSig(frameCnter);
    frameCnter = 0;
}

VideoMgr::~VideoMgr()
{
    timer->stop();
    socket->disconnectFromHost();
    emit finished();
}
