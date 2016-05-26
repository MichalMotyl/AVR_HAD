#ifndef AVRCLIENT_H
#define AVRCLIENT_H

#include <iostream>

#include <QObject>
#include <QTimer>
#include <QSocketNotifier>
#include <QUdpSocket>

using namespace std;


class AvrClient : public QObject
{
    Q_OBJECT
public:
    explicit AvrClient(QObject *parent = 0);
    void sendDatagram();

signals:

public slots:
    void Run() { cout << "Hello World!" << endl; }
    void timer_event();
    void readPendingDatagrams();
private:
    QTimer *main_loop_timer;
    bool wait4reply = false;
    QUdpSocket *udpSocket;
    QUdpSocket *udpSocket_read;
};

#endif // AVRCLIENT_H
