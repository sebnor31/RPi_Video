#include "voicemgr.h"

VoiceMgr::VoiceMgr(QObject *parent) : QObject(parent)
{
}

void VoiceMgr::start()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataReceive()));
    connect(socket, &QTcpSocket::stateChanged, this, &VoiceMgr::stateChanged);
//    connect(this, &MainWindow::updatedStream, this, &MainWindow::processStream);

    socket->connectToHost("169.254.0.2", 8000);
    socket->write("GET /audio_1\r\n\r\n");
}

void VoiceMgr::stateChanged(QAbstractSocket::SocketState state)
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

void VoiceMgr::dataReceive()
{
    emit logMsg(QString("Bytes: %1").arg(socket->bytesAvailable()));
    socket->readAll();
//    stream.append(socket->readAll());
//    emit updatedStream();
}

VoiceMgr::~VoiceMgr()
{
    socket->disconnectFromHost();
    emit finished();
}
