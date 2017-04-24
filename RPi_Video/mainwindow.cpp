#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyAvail(QNetworkReply*)));

    // the HTTP request
//        QNetworkRequest req( QUrl( QString("http://ip.jsontest.com/") ) );
    QNetworkRequest req( QUrl( QString("http://169.254.0.2:8080/?action=snapshot") ) );
    mgr.get(req);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::replyAvail(QNetworkReply *reply)
{
    ui->logger->append("Read success!");

    if (reply->error() == QNetworkReply::NoError) {
            //success
            ui->logger->append("Success: " + reply->readAll());
        }
    else {
        //failure
         ui->logger->append("Failure: " + reply->errorString());
    }

    reply->deleteLater();
}
