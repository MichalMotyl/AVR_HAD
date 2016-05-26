#include "avrclient.h"
#include <QDataStream>
#include <QDateTime>

AvrClient::AvrClient(QObject *parent) : QObject(parent)
{

    udpSocket = new QUdpSocket(this);
    udpSocket_read = new QUdpSocket(this);

    if(udpSocket_read->bind(QHostAddress::Any, 8889))
    {
        connect(udpSocket_read, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
        cout <<"bind ok?"<<endl;
    }




    main_loop_timer = new QTimer(this);
    main_loop_timer->setInterval(3000);
    main_loop_timer->setSingleShot(false);

    connect(main_loop_timer, SIGNAL(timeout()), this, SLOT(timer_event()));
    main_loop_timer->start();
}

void AvrClient::sendDatagram()
{
    char data[64] = {"motyl"};

    udpSocket->writeDatagram(data,64, QHostAddress("192.168.1.103"), 8888);
}

void AvrClient::timer_event()
{

    if(!wait4reply)
    {
        cout << "send datagram" << endl;
        sendDatagram();
        wait4reply = true;
        // Rx connection: check we are not already bound
        if (udpSocket->state() != udpSocket->BoundState)
        {
            cout << "dfdfd"<< endl;
            // Rx not in bound state, attempt to bind
        //    udpSocket->bind(QHostAddress("192.168.1.103"), 8889);
     //       connect(udpSocket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
        }
    }
    else
    {

    }
}

void AvrClient::readPendingDatagrams()
{
    printf("readPendingDatagrams\n");
    while (udpSocket_read->hasPendingDatagrams()) {
        printf("got data?\n");
        QByteArray datagram;
        datagram.resize(udpSocket_read->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket_read->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        printf("datagram.data():%s senderPort:%d\n",datagram.data(),senderPort);
        wait4reply = false;

        //processTheDatagram(datagram);
    }
}
