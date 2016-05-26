#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;

QByteArray macString2Array(QString mac)
{
    QByteArray tmp;
    char t[6];

    sscanf(mac.toStdString().c_str(),"%x:%x:%x:%x:%x:%x",&t[0],&t[1],&t[2],&t[3],&t[4],&t[5]);

//    for(int i = 0;i<6;i++)
//    {
//        printf("dd:%x\n",t[i]);
//    }

    tmp = t;

    return tmp;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TxUdpSocket = new QUdpSocket(this);
    RxUdpSocket = new QUdpSocket(this);

    //TxUdpSocket2 = new QUdpSocket(this);
   // RxUdpSocket2 = new QUdpSocket(this);


    connected = false;
    output_data = 0;

    responseTimer = new QTimer(this);
    responseTimer->setSingleShot(true);

    connect(responseTimer, SIGNAL(timeout()), this, SLOT(timer_event()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_Exit_triggered()
{
    RxUdpSocket->disconnectFromHost();
    QApplication::exit();
}



/**
 * @brief MainWindow::sendDatagram
 */
void MainWindow::sendDatagram()
{
    if(!connected) return;

    responseTimer->start();

    //char data[64] = {"motyl"};
    char data[64] = {0};
    data[0] = output_data;
    TxUdpSocket->writeDatagram(data,64,QHostAddress(connection_address), 8888);
}

/**
 * @brief MainWindow::getCurrentOutputValue
 */
void MainWindow::getCurrentOutputValue()
{

}

void MainWindow::wait4Response()
{

}


/**
 * @brief MainWindow::readConfig :Read current configuration from AVR
 *
 */
void MainWindow::readConfig()
{
    if(connected)
    {
        //send command READ CONFIG
        char data[64] = {0};
        data[0] = 0x55;
        data[1] = CMD_READ_CONFIG;
        TxUdpSocket->writeDatagram(data,64, QHostAddress(connection_address), 8888);
    }
}

void MainWindow::readPendingDatagrams()
{
    cout << "readPendingDatagrams\n" << endl;

        while (RxUdpSocket->hasPendingDatagrams()) {

            while(responseTimer->isActive())responseTimer->stop();

            QByteArray datagram;
            uint8_t *data;
            datagram.resize(RxUdpSocket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            RxUdpSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
            data = (uint8_t*)datagram.data();
            memcpy(&current_config,&data[2],51);
            struct sys_config *conf = &current_config ;

            if(data[0]==0xAA)
            {
                if(data[1]==CMD_READ_CONFIG)
                {
                    cout << "Got response for CMD_READ_CONFIG..." << endl;

                    QString mac_tmp;
                    mac_tmp = QString::number(conf->mac[0], 16);
                    mac_tmp.append(":");
                    mac_tmp.append(QString::number(conf->mac[1], 16));
                    mac_tmp.append(":");
                    mac_tmp.append(QString::number(conf->mac[2], 16));
                    mac_tmp.append(":");
                    mac_tmp.append(QString::number(conf->mac[3], 16));
                    mac_tmp.append(":");
                    mac_tmp.append(QString::number(conf->mac[4], 16));
                    mac_tmp.append(":");
                    mac_tmp.append(QString::number(conf->mac[5], 16));
                    ui->mac->setText(mac_tmp);

                    if(!conf->dhcp)
                    {
                        ui->staticIP->setChecked(true);
                        QString tmp;
                        tmp = QString::number(conf->static_ip[0]) + "." + QString::number(conf->static_ip[1]) + "." + QString::number(conf->static_ip[2]) + "." + QString::number(conf->static_ip[3]);
                        ui->lineEdit->setText(tmp);

                        tmp = QString::number(conf->netmask[0]) + "." + QString::number(conf->netmask[1]) + "." + QString::number(conf->netmask[2]) + "." + QString::number(conf->netmask[3]);
                        ui->lineEdit_2->setText(tmp);

                        tmp = QString::number(conf->gateway[0]) + "." + QString::number(conf->gateway[1]) + "." + QString::number(conf->gateway[2]) + "." + QString::number(conf->gateway[3]);
                        ui->lineEdit_3->setText(tmp);

                        ui->dhcp_name->setText((char*)conf->host_name);
                        ui->dhcp_name_label->setDisabled(true);
                    }
                    else
                    {
                        ui->dhcp->setChecked(true);
                        ui->dhcp_name_label->setDisabled(false);
                    }
                }
            }
        }
}


void MainWindow::on_Output1_toggled(bool checked)
{
    cout << "on_Output1_toggled" << endl;
    if(checked)
        output_data |= 0x01;
    else
        output_data &= ~0x01;

    sendDatagram();
}

void MainWindow::on_Output2_toggled(bool checked)
{
    cout << "on_Output2_toggled" << endl;
    if(checked)
        output_data |= 0x02;
    else
        output_data &= ~0x02;

    sendDatagram();
}

void MainWindow::on_Output3_toggled(bool checked)
{
    cout << "on_Output3_toggled" << endl;
    if(checked)
        output_data |= 0x04;
    else
        output_data &= ~0x04;

    sendDatagram();
}

void MainWindow::on_Output4_toggled(bool checked)
{
    cout << "on_Output4_toggled" << endl;
    if(checked)
        output_data |= 0x08;
    else
        output_data &= ~0x08;

    sendDatagram();
}

void MainWindow::timer_event()
{
    //ui->HostName->setText("Send error - No ACK");
}



void MainWindow::on_action_Connect_triggered()
{
        if(connected==false)
        {
            bool ok;
            QString ip = QInputDialog::getText(this, tr("Connect with device"),
                                                     tr("IP:"), QLineEdit::Normal,
                                                     "192.168.1.2", &ok);
           if (ok && !ip.isEmpty())
           {
                //connection_address = new QHostAddress(ip);
               connection_address = ip;
           }
           else
           {
                return;
           }


            if(RxUdpSocket->bind(QHostAddress::AnyIPv4, ATMEGA_PORT_CONTROL_PORT_TX))
            {
                connect(RxUdpSocket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
                ui->statusBar->showMessage("Connected");

                connected = true;
            }
            else
            {
                connected = false;
            }
        }
        else
        {
            RxUdpSocket->disconnectFromHost();
            //RxUdpSocket2->disconnectFromHost();

            ui->statusBar->showMessage("Disconnected");

            connected = false;
        }

        readConfig();
}


void MainWindow::on_staticIP_clicked()
{
    ui->label->setDisabled(false);
    ui->label_2->setDisabled(false);
    ui->label_3->setDisabled(false);

    ui->lineEdit->setDisabled(false);
    ui->lineEdit_2->setDisabled(false);
    ui->lineEdit_3->setDisabled(false);
}

void MainWindow::on_dhcp_clicked()
{
    ui->label->setDisabled(true);
    ui->label_2->setDisabled(true);
    ui->label_3->setDisabled(true);

    ui->lineEdit->setDisabled(true);
    ui->lineEdit_2->setDisabled(true);
    ui->lineEdit_3->setDisabled(true);
}

void MainWindow::on_actionSave_Config_triggered()
{
    QByteArray config_data;

    if(!connected) return;

    if(ui->staticIP->isChecked())
    {//save static config
        static_ip_addr = new QHostAddress(ui->lineEdit->text());
        static_ip_gateway = new QHostAddress(ui->lineEdit_3->text());
        static_ip_netmask = new QHostAddress(ui->lineEdit_2->text());

//        memcpy(&current_config,&data[2],51);
//        struct sys_config *conf = &current_config ;

        config_data.append(0x55);
        config_data.append(CMD_SEND_CONFIG);//cmd
        config_data.append(12);
        config_data.append(static_ip_addr->toIPv4Address());
        config_data.append(static_ip_netmask->toIPv4Address());
        config_data.append(static_ip_gateway->toIPv4Address());
        config_data.append(macString2Array(ui->mac->text()));

        //cout << "ip:" << static_ip_addr->toIPv4Address() << " netmask:" << ui->lineEdit_2->text().toStdString() << endl;

        TxUdpSocket->writeDatagram(config_data,64, QHostAddress(connection_address), ATMEGA_PORT_CONTROL_PORT_RX);

        delete static_ip_addr;
        delete static_ip_gateway;
        delete static_ip_netmask;
    }
    else
    {//use dhcp config
        config_data.append(0x55);
        config_data.append(CMD_SEND_CONFIG);//cmd
        config_data.append(1);
        config_data.append(0xDD);//DHCP
        TxUdpSocket->writeDatagram(config_data,64, QHostAddress(connection_address), ATMEGA_PORT_CONTROL_PORT_RX);
    }
}


