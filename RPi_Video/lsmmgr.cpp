#include "lsmmgr.h"


LsmMgr::LsmMgr(QObject *parent) : QObject(parent)
{

}

void LsmMgr::start()
{
    QUrl rpi("http://169.254.0.2:8800/");
    emit logMsg(QString("Connecting to %1").arg(rpi.toString()));

    manager = new QNetworkAccessManager(this);
    request.setUrl(rpi);
    connect(manager, &QNetworkAccessManager::finished, this, &LsmMgr::dataReceive);

    timer = new QTimer(this);
    timer->setInterval(20);
    connect(timer, &QTimer::timeout, this, &LsmMgr::readData);
    timer->start();
}

void LsmMgr::readData()
{
    manager->get(request);
}

void LsmMgr::dataReceive(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError){
        logNetError(reply->error());
    }
    else{
        QByteArray resp = reply->readAll();
        emit logMsg(QString(resp));
    }

    reply->deleteLater();
}


void LsmMgr::logNetError(QNetworkReply::NetworkError err)
{
    QString msg = QString("Network Error: %1").arg(err);
    emit logMsg(msg);
}


LsmMgr::~LsmMgr()
{
    timer->stop();
    emit logMsg("Exiting LSM Thread");
    emit finished();
}
