#include "UNIT_MQTT.h"


void UNIT_MQTT::Init(HardwareSerial *serial, int baud, uint8_t RX, uint8_t TX) {
    _serial = serial;
    _serial->begin(baud, SERIAL_8N1, RX, TX);
}

String UNIT_MQTT::waitMsg(unsigned long time) {
    String restr="";
    unsigned long start = millis();
    while(1){
        if(Serial2.available() || (millis() - start) < time) {
            String str = Serial2.readString();
            restr += str;
        }else{
            break;
        }
    }
    return restr;
}

void UNIT_MQTT::sendMsg(String command) {
    _serial->print(command);
    delay(10);
}

void UNIT_MQTT::subscribe(SubscribeTopic Topic) {
    String readstr;
    sendMsg("AT+MQSUBSCRIBE="+ Topic.No + ",1,\"" + Topic.Topic + "\","+ Topic.QoS + "\r\n");
    readstr = waitMsg(200);
}

void UNIT_MQTT::publish(PublishTopic Topic) {
    String readstr;
    sendMsg("AT+MQPUBLISH=\""+ Topic.Topic + "\",\"" + Topic.Data + "\","+ Topic.QoS + "\r\n");
    readstr = waitMsg(200);
}

void UNIT_MQTT::startMQTT() {
    String readstr;
    sendMsg("AT+MQSTART\r\n");
}

bool UNIT_MQTT::isConnectedLAN() {
    String readstr;
    sendMsg("AT+NETIP?\r\n");
    readstr = waitMsg(50);
    if(readstr.indexOf("0.0.0.0") == -1 && readstr.indexOf("192.168") !=-1 ){
        return true;
    }else{
        return false;
    }
}

bool UNIT_MQTT::isConnectedMQTT() {
    String readstr;
    sendMsg("AT+MQSTATUS?\r\n");
    readstr = waitMsg(100);
    if(readstr.indexOf("+MQSTATUS=OK:1") != -1 ) {
       return true;
    }else{
       return false;
    }
}

bool UNIT_MQTT::receiveMessage() {
    String readstr;
    readstr = waitMsg(1);
    if(readstr.indexOf("+MQRECV:") != -1 ) {
       payload.Topic = readstr.substring(
           readstr.indexOf("+MQRECV:") + 9, 
           readstr.indexOf(",") - 1 
       );
       payload.Len = readstr.substring(
           readstr.indexOf("\",") + 2,
           readstr.indexOf(",\"")
       ).toInt();
       payload.Data = readstr.substring(
           readstr.indexOf(",\"") + 2,
           readstr.lastIndexOf("\"")
       );
       return true;
    }else{
       return false;
    }
}

bool UNIT_MQTT::configSave() {
    String readstr;
    sendMsg("AT+SAVE\r\n");
    sendMsg("AT+RESET\r\n");
    while(!isConnectedLAN());
}

bool UNIT_MQTT::configMQTT(
    String host,
    String port,
    String clientId,
    String user,
    String pwd,
    String keepalive
) {
    String readstr;
    sendMsg("AT+MQSTOP\r\n");
    sendMsg("AT+MQCLIENTID=\"" + clientId + "\"\r\n");
    sendMsg("AT+MQSERVER=\"" + host + "\"," + port + "\r\n");
    sendMsg("AT+MQUSERPWD=\""+ user +"\",\"" + pwd + "\"\r\n");
    sendMsg("AT+MQKEEP=" + keepalive + "\r\n");
}
