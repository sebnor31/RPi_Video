#ifndef VOICEMGR_H
#define VOICEMGR_H

#include <QObject>
#include <QTcpSocket>

class VoiceMgr : public QObject
{
    Q_OBJECT

private:
    // Network
    QTcpSocket *socket;
    QByteArray stream;

public:
    explicit VoiceMgr(QObject *parent = 0);
    ~VoiceMgr();

signals:
    void logMsg(QString msg);
    void finished();

public slots:
    void start();

private slots:
    void stateChanged(QAbstractSocket::SocketState state);
    void dataReceive();
};

#endif // VOICEMGR_H
