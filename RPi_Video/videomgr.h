#ifndef VIDEOMGR_H
#define VIDEOMGR_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QPixmap>

class VideoMgr : public QObject
{
    Q_OBJECT

private:
    // Network
    QTcpSocket *socket;
    QByteArray stream;

    // FPS
    quint16 frameCnter;
    QTimer *timer;

public:
    explicit VideoMgr(QObject *parent = 0);
    ~VideoMgr();

signals:
    void logMsg(QString msg);
    void newFrameSig(QPixmap frame);
    void newFpsSig(quint16 fps);
    void updatedStream();
    void finished();

public slots:
    void start();

private slots:
    void stateChanged(QAbstractSocket::SocketState state);
    void dataReceive();
    void processStream();
    void updateFps();
};

#endif // VIDEOMGR_H
