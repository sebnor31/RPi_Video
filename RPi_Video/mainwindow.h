#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Network
    QTcpSocket *socket;
    QByteArray stream;

    // FPS
    quint16 frameCnter;
    QTimer *timer;

private slots:
    void tcpDataReceive();
    void processStream();
    void on_startBtn_clicked();
    void updateFps();

signals:
    void updatedStream();
};

#endif // MAINWINDOW_H
