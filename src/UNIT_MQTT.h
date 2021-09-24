
#include <Arduino.h>


struct SubcribeTopic
{
	String No;
	String Topic;
	String QoS;
};

struct PublishTopic
{
	String Topic;
	String Data;
	String QoS;
};

class UNIT_MQTT
{
    private:
        HardwareSerial *_serial;
    public:
        void Init(HardwareSerial *serial = &Serial2, int baud=9600, uint8_t RX = 16, uint8_t TX = 17);
        bool isConnectedLAN();
        bool configMQTT(
            String host = "host",
            String port = "port",
            String clientId = "client id",
            String user = "user",
            String pwd = "pwd",
            String keepalive = "60"
        );
        void subcribe(SubcribeTopic Topic);
        void publish(PublishTopic Topic);
        bool configSave();
        bool isConnectedMQTT();
        void startMQTT();
        String waitMsg(unsigned long time);
        void sendMsg(String command);
};