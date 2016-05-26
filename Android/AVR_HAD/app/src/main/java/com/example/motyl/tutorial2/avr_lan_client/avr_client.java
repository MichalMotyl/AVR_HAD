package com.example.motyl.tutorial2.avr_lan_client;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

import com.example.motyl.tutorial2.MainActivity;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Created by motyl on 19.03.16.
 */
public class avr_client {

    private static final int AVR_RX_PORT = 8888;
    private static final int AVR_TX_PORT = 8889;

    private static final byte CMD_SET_OUTPUT = 0x01;
    private static final byte CMD_GET_OUTPUT = 0x02;
    private static final byte CMD_SEND_CONFIG = 0x03;
    private static final byte CMD_SAVE_CONFIG = 0x04;
    private static final byte CMD_READ_CONFIG = 0x05;
    private static final byte CMD_GET_FEATURE = 0x06;
    private static final byte CMD_GET_FEATURE_NUMBER = 0x07;
    private static final byte CMD_HANDSHAKE = 0x08;
    //private static final byte CMD_REPLY_PREFIX = 0xaa;

    private static final String AVR_CLIENT_DBG = "MM_DEBUG";

    private int num_of_devices = 0;
    private boolean data_ok = false;

    List<had_device>    had_devices_list;

    private byte tx_buf[] = new byte[64];
    private byte rx_buf[] = new byte[64];

    public class had_device{

        public had_device()
        {
            dev_sock = new AvrSocket();
            dev_configuration = new dev_config();
            dev_features = new ArrayList<dev_feature>();
        };

        public AvrSocket dev_sock;
        public dev_config dev_configuration;
        public List<dev_feature> dev_features;

    }


    public avr_client(){
        had_devices_list = new ArrayList<had_device>();
    }

    private class AvrSocket {
        String ip_addr = null;
        DatagramSocket txSock = null;
        DatagramSocket rxSock = null;
    }

    public class dev_feature {
        public byte feature_type;
        public byte feature_id;
        public String feature_name;

        public dev_feature() {
            feature_type = -1;
            feature_id = -1;
            //feature_name = new byte[32];
        }

    }

    public class dev_config {
        byte dhcp;
        byte[] static_ip;
        byte[] gateway;
        byte[] netmask;
        byte[] host_name;
        byte[] mac;

        byte features_cnt = 0;
        dev_feature[] cap;

        public dev_config() {
            dhcp = 0;
            static_ip = new byte[4];
            gateway = new byte[4];
            netmask = new byte[4];
            host_name = new byte[32];
            mac = new byte[6];
        }

        public int getDataSize() {
            return (static_ip.length + static_ip.length + gateway.length + netmask.length + host_name.length + mac.length);
        }

    }

    public boolean isDataReady(){
        return data_ok;
    }
    public had_device getDevice(int idx){
        return had_devices_list.get(idx);
    };

    public void connect2AllDevices() {

        tx_buf[0] = 0x55;
        tx_buf[1] = CMD_HANDSHAKE;

        if (num_of_devices == 0) {
            Log.i(AVR_CLIENT_DBG, "connect2AllDevices:list is empty!!!");
            return;
        }

        new Thread(){
            public void run(){

                    for(int i = 0;i<(num_of_devices);i++)
                    {
                        AvrSocket tmp_avr_socket = had_devices_list.get(i).dev_sock;

                        try {
                            tmp_avr_socket.txSock = new DatagramSocket();

                        } catch (IOException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }

                            try {
                                tmp_avr_socket.rxSock = new DatagramSocket(AVR_TX_PORT);

                            } catch (IOException e) {
                                e.printStackTrace();
                            }

                        if(doHandShake(had_devices_list.get(i))==true){
                            Log.i(AVR_CLIENT_DBG, "Got hanshake from  " + tmp_avr_socket.ip_addr);

                            int fcnt = getDeviceFeaturesNum(had_devices_list.get(i));
                            had_device dev_tmp = had_devices_list.get(i);
                            //get device config and features
                            if(fcnt>0){
                                Log.i(AVR_CLIENT_DBG, "getDeviceFeaturesNum " + fcnt);

                                for(int j = 0;j<fcnt;j++)
                                {
                                    try {
                                        cmdGetFeature(dev_tmp,(byte)j);
                                    } catch (UnsupportedEncodingException e) {
                                        e.printStackTrace();
                                    }
                                }
                            }

                        }
                        else {

                        }
                    }

                data_ok = true;
            }
        }.start();
    }

    public void readDevicesFromList(MainActivity activity) {
        num_of_devices = 0;
        String file = "com.tutorial2.app.settings";

        SharedPreferences settings = activity.getSharedPreferences(file, Context.MODE_APPEND);

        num_of_devices = settings.getInt("num_of_devices", 0);

        if (num_of_devices == 0) {
            Log.i(AVR_CLIENT_DBG, "list is empty!!!");
            return;
        } else {
            Log.i(AVR_CLIENT_DBG, "list is not empty!!! " + num_of_devices);
        }

        int tmp = 0;

        for (int i = 1; i < (num_of_devices + 1); i++) {
            String tmp_name = "DEV" + i;

            had_device tmp_data = new had_device();

            tmp_data.dev_sock.ip_addr = settings.getString(tmp_name, null);
            had_devices_list.add(tmp_data);

            Log.i(AVR_CLIENT_DBG, "get dev " + tmp_name + had_devices_list.get(tmp).dev_sock.ip_addr);
            tmp++;
        }
    }

    public int getDevicesNum(){
        return num_of_devices;
    }


    private boolean doHandShake(had_device dev){
        boolean res = false;
        res = sendMsg(CMD_GET_FEATURE_NUMBER,rx_buf,dev.dev_sock);
        if(res==true){
            if(rx_buf[0]==(byte)0xAA && rx_buf[0]==CMD_HANDSHAKE) return true;
        }

            return res;
    };

    boolean getDeviceConfig(had_device dev){

        return false;
    };

    int getDeviceFeaturesNum(had_device dev){
        boolean res = false;

        res = sendMsg(CMD_GET_FEATURE_NUMBER,rx_buf,dev.dev_sock);
        if(res==true){
            return rx_buf[2];
        }

        return -1;
    };

    public boolean getOutputStatus(had_device dev,int output_idx){
        boolean res = false;
        //tx_buf[2] = idx;

        res = sendMsg(CMD_GET_OUTPUT,rx_buf,dev.dev_sock);

        return false;
    };

    public void SetOutput(had_device dev,int output_idx,boolean val){

        return;
    };

    boolean cmdGetFeature(had_device dev,byte idx) throws UnsupportedEncodingException {
        boolean res = false;
        tx_buf[2] = idx;
        Log.i(AVR_CLIENT_DBG, "cmdGetFeature:");
        res = sendMsg(CMD_GET_FEATURE,rx_buf,dev.dev_sock);

        if(res==true){
            dev_feature f = new dev_feature();
            ByteBuffer bb = ByteBuffer.allocate(32);
            Arrays.fill(bb.array(), (byte) 0);
            bb.put(rx_buf, 4, 32);

            f.feature_type = rx_buf[2];
            f.feature_id = rx_buf[3];
            f.feature_name = new String(bb.array(), "ASCII");

            Log.i(AVR_CLIENT_DBG, "cmdGetFeature: add feature with name " + f.feature_name);
            dev.dev_features.add(f);

            return true;
        }

        return false;
    };

    boolean sendMsg(byte cmd,byte rxBuff[],AvrSocket sock){

        tx_buf[0] = 0x55;
        tx_buf[1] = cmd;

        if(sock==null || rxBuff==null) return false;

        for (int i = 0; i <rxBuff.length ; i++) {
            rxBuff[i] = 0;
        }

        try
        {
            DatagramPacket p = new DatagramPacket(tx_buf, tx_buf.length,InetAddress.getByName(sock.ip_addr),AVR_RX_PORT);
            sock.txSock.send(p);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }


        try {

            DatagramPacket reply = new DatagramPacket(rx_buf, rx_buf.length);

            sock.rxSock.receive(reply);

        } catch (IOException ex) {
            Logger.getLogger(this.toString()).log(Level.SEVERE, null, ex);
        }
        finally {

            if(rxBuff[0]==(byte)0xAA && rxBuff[1]==cmd)
            {
                return true;
            }
            else
                return false;
        }
    };
}
