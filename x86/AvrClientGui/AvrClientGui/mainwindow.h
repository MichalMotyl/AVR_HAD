#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSocketNotifier>
#include <QUdpSocket>
#include <QHostInfo>
#include <QTimer>
#include <QHostInfo>
#include <QInputDialog>

#define GNUC_PACKED __attribute__((packed))

struct message2avr{
    uint8_t cmd;
    uint8_t value;
}GNUC_PACKED;

struct sys_config
{
        uint8_t dhcp;
        uint8_t static_ip[4];
        uint8_t gateway[4];
        uint8_t netmask[4];
        uint8_t host_name[32];
        uint8_t mac[6];
} GNUC_PACKED;

#define CMD_SET_OUTPUT          0x01
#define CMD_GET_OUTPUT          0x02
#define CMD_SEND_CONFIG         0x03
#define CMD_SAVE_CONFIG         0x04
#define CMD_READ_CONFIG         0x05
#define CMD_GET_FEATURE         0x06
#define CMD_GET_FEATURE_NUMBER  0x07
#define CMD_HANDSHAKE           0x08


#define ATMEGA_PORT_CONTROL_PORT_RX 8888
#define ATMEGA_PORT_CONTROL_PORT_TX 8889

#define ATMEGA_PORT_DATA_PORT_RX 8890
#define ATMEGA_PORT_DATA_PORT_TX 8891

/**
 *
 * Command format
 * 1. [0x55][CMD][LEN][DATA][CRC]
 * CMD 0x01 ->set output value
 * CMD 0x02 ->send configuration data
 * CMD 0x03 ->save configuration to eeprom
 *
 * 2.ACK format allways 4 bytes lenght
 * [0xAA]['A'][['C']['K']
 *
 */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Exit_triggered();
    void readPendingDatagrams();
  //  void on_pushButton_2_pressed();
    void on_Output1_toggled(bool checked);
    void on_Output2_toggled(bool checked);
    void on_Output3_toggled(bool checked);
    void on_Output4_toggled(bool checked);
    void timer_event(void);
    void on_action_Connect_triggered();
    void on_staticIP_clicked();
    void on_dhcp_clicked();

    void on_actionSave_Config_triggered();

private:
    Ui::MainWindow *ui;
    QUdpSocket *TxUdpSocket = NULL;
    QUdpSocket *RxUdpSocket = NULL;

//    QUdpSocket *TxUdpSocket2 = NULL;
//    QUdpSocket *RxUdpSocket2 = NULL;

    QTimer  *responseTimer = NULL;
    bool connected;
    uint8_t output_data;
    QString connection_address;

    QHostAddress *static_ip_addr = NULL;
    QHostAddress *static_ip_gateway = NULL;
    QHostAddress *static_ip_netmask = NULL;

    struct sys_config current_config ;

    void sendDatagram();
    void getCurrentOutputValue();
    void wait4Response();
    void readConfig();

};

#endif // MAINWINDOW_H
