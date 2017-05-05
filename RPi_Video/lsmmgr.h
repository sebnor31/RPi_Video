#ifndef LSMMGR_H
#define LSMMGR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

class LsmMgr : public QObject
{
    Q_OBJECT

private:
    QTimer *timer;

    // Network
    QNetworkAccessManager *manager;
    QNetworkRequest request;

public:
    explicit LsmMgr(QObject *parent = 0);
    ~LsmMgr();

signals:
    void logMsg(QString msg);
    void finished();

public slots:
    void start();

private slots:
    void readData();
    void logNetError(QNetworkReply::NetworkError err);
    void dataReceive(QNetworkReply *reply);
};

#endif // LSMMGR_H
