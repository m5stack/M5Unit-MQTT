#include "UNIT_MQTT.h"

UNIT_MQTT unit;
void setup()
{
    Serial.begin(115200);
    //INIT UNIT MQTT
    unit.Init(&Serial2, 9600, 16, 17);
    Serial.println("Waiting LAN Connect");
    while (!unit.isConnectedLAN())Serial.print('.');

    Serial.println("LAN Connected");

    Serial.println("Config MQTT");

    unit.configMQTT(
        "mqtt.m5stack.com",//host
        "1883",//port
        "client",//client id
        "user",//user name
        "pwd",//password
        "60"//keepalive
    );

    Serial.println("Subcribe Topic");
    unit.subcribe({
        "1",//No 1~-4
        "UNIT_MQTT_TOPIC_1",//Topic
        "2"//QoS
    });

    unit.subcribe({
        "2",
        "UNIT_MQTT_TOPIC_2",
        "2"
    });

    Serial.println("Save config and reset");
    unit.configSave();
    unit.startMQTT();
    Serial.println("Start MQTT Connect");
    while(!unit.isConnectedMQTT());
    Serial.println("MQTT Server Connected");
}

void loop()
{
    if(unit.isConnectedMQTT())
    {   
        String readstr = unit.waitMsg(1000);
        Serial.print(readstr);
        unit.publish({
            "UNIT_MQTT_TOPIC_1",//Topic
            "Hello UNIT MQTT: "+String(millis()),//Data
            "2"//QoS
        });
    }else{
        Serial.print("MQTT disconnect");
    }
}


