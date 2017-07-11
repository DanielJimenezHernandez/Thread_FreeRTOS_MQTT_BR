#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

typedef enum state{
	INIT = 0x01,
	CONNECTING,
	CONNECTED,
	PUBLISHED,
	DISCONNECTED
}eState;

typedef enum MQTTCMD{
	mqttConnect = 0x01,
	mqttDisconnect,
	mqttPublish,
	mqttSubscribe
}eMQTTCMD;

typedef struct{
	eMQTTCMD cmd;
	int port;
	int payloadLen;
	char * payload;
	char * topic;
	char * serverAddr;

}sMQTTParams;

/* Public Functions */
void MQTTInit(void);
void MQTTOpenUdp(int udpPort, char * udpAddr);
void MQTTSendTest(sMQTTParams *params);
void MQTTSendPublish(char * topic,char * payload, int payloadLen);
void MQTTSendConnect(void);


#endif /*MQTTCLIENT_H*/
