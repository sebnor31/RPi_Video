#ifndef LSMMGR_H
#define LSMMGR_H

#include <QObject>
#include <QUdpSocket>

class LsmMgr : public QObject
{
    Q_OBJECT

private:
    // Network
    QUdpSocket *socket;
    QByteArray stream;

public:
    explicit LsmMgr(QObject *parent = 0);
    ~LsmMgr();

signals:
    void logMsg(QString msg);
    void finished();

public slots:
    void start();

private slots:
    void stateChanged(QAbstractSocket::SocketState state);
    void dataReceive();
};

#endif // LSMMGR_H
