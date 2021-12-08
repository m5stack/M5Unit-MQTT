
#include <Arduino.h>


struct SubscribeTopic
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

struct PayloadTopic
{
	String Topic;
	int Len;
	String Data;
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
        void subscribe(SubscribeTopic Topic);
        void publish(PublishTopic Topic);
        bool configSave();
        bool isConnectedMQTT();
        bool receiveMessage();
        void startMQTT();
        String waitMsg(unsigned long time);
        void sendMsg(String command);
    public:
        PayloadTopic payload;
};