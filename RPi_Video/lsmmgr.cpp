#include "lsmmgr.h"
#include <QNetworkDatagram>

LsmMgr::LsmMgr(QObject *parent) : QObject(parent)
{

}

void LsmMgr::start()
{
    emit logMsg("Connecting to 169.254.0.2:5005");

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress("169.254.0.2"), 5005);

    connect(socket, &QUdpSocket::readyRead, this, &LsmMgr::dataReceive);
    connect(socket, &QUdpSocket::stateChanged, this, &LsmMgr::stateChanged);
    emit logMsg("Waiting for Data....");

}

void LsmMgr::stateChanged(QAbstractSocket::SocketState state)
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
        socket->write("GET /server_version.xsl\r\n\r\n");
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

void LsmMgr::dataReceive()
{
    while (socket->hasPendingDatagrams()) {
            QNetworkDatagram datagram = socket->receiveDatagram();
            emit logMsg(QString("Received: %1").arg(datagram.data().size()));
        }
}

LsmMgr::~LsmMgr()
{
    socket->disconnectFromHost();
    emit logMsg("Exiting LSM Thread");
    emit finished();
}
