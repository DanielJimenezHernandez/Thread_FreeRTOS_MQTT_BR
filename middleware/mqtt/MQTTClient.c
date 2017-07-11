/* Methods to connect to an UDP bridge to transfer MQTT packets*/
#include <sockets.h>
#include <network_utils.h>
#include <shell.h>
#include <app_socket_utils.h>
#include "EmbeddedTypes.h"
#include <string.h>
#include <stdio.h>
#include "MQTTPacket.h"
#include "MQTTClient.h"
#include "Utils.h"

/*IP Version */
#define DEVICE_ID "FRDM-K64F"
#define BUFLEN 512

int32_t sockFd;
int8_t sockID;

sMQTTParams MQTTParams;
extern taskMsgQueue_t *mpAppThreadMsgQueue;
appSockCmdParams_t *pAppSockCmdParams;
int MQTTState = INIT;

/* Buffers for selialized data*/
unsigned char MQTTPublishPacket[BUFLEN];
char connectPack[25] = {0x10,0x17,0x00,0x06,0x4d,0x51,0x49,0x73,0x64,0x70,0x03,
						0x02,0x00,0x00,0x00,0x09,0x46,0x52,0x44,0x4d,0x2d,0x4b,
						0x36,0x34,0x46};

/* Private functions */

eState state = INIT;


void MQTTInit(void){
	APP_InitUserSockets(mpAppThreadMsgQueue,&sockID);
	pAppSockCmdParams = MEM_BufferAlloc(sizeof(appSockCmdParams_t));
}

void MQTTOpenUdp(int udpPort, char * udpAddr){

	FLib_MemSet(pAppSockCmdParams, 0, sizeof(appSockCmdParams_t));

	/* Check number of arguments according to the shellComm table */
	pAppSockCmdParams->appSocketsCmd = gSockOpenUdp;
	/* socket open udp */
	uint32_t result = -1;

	pAppSockCmdParams->appSocketsTrans = gSockUdp;
	/* Set local information */
	pAppSockCmdParams->ipVersion = AF_INET;

	/* Set remote information for easier send */
	if(pAppSockCmdParams->ipVersion == AF_INET6)
	{
		pAppSockCmdParams->sin6_port = udpPort;
		result = pton(AF_INET6,udpAddr, &pAppSockCmdParams->sin6_addr);
	}
	else
	{
		pAppSockCmdParams->sin_port = udpPort;
		result = pton(AF_INET, udpAddr, &pAppSockCmdParams->sin_addr);
	}

	if(result == -1)
	{
		shell_write("IP address has a wrong format");
		SHELL_NEWLINE();
	}

    App_SocketHandleSendAsync(pAppSockCmdParams);
}


void MQTTRxCallback(void *param){
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)param;
    sockaddrIn_t *pRemAddr = (sockaddrIn_t*)(&pSessionPacket->remAddr);
    int packetType;
    packetType = pSessionPacket->pData[0] >> 4;

    if (packetType == CONNACK){
    	MQTTState = CONNECTED;
    }
    else if(packetType == 0xff && pSessionPacket->pData[1] == 0x01){
    	MQTTState = DISCONNECTED;
    }

}


unsigned char testbuffer[6] = {'H','e','l','l','o','\0'};
int tempFlag =0;
void MQTTSendTest(sMQTTParams *params){

        if (MQTTState != CONNECTED && params->cmd == mqttConnect){
        	MQTTSendConnect();
        }
        else if(MQTTState == CONNECTED && params->cmd == mqttPublish){
        	MQTTSendPublish(params->topic,params->payload, params->payloadLen);
        }
}

void MQTTSendPublish(char * topic,char * payload, int payloadLen){
	FLib_MemSet(pAppSockCmdParams, 0, sizeof(appSockCmdParams_t));
	int len_publish;

	MQTTPacket_publishData default_options_publish = MQTTPacket_publishData_initializer;
	default_options_publish.topicName.cstring = topic;
	default_options_publish.payload = (unsigned char*)payload;
	default_options_publish.payloadlen = payloadLen;
	len_publish = MQTTSerialize_publish_opt(MQTTPublishPacket,BUFLEN,&default_options_publish);

	pAppSockCmdParams->appSocketsCmd = gSockSend;
	/* Get socket id */
	pAppSockCmdParams->sock32 = sockID;
	pAppSockCmdParams->pData = MQTTPublishPacket;
	pAppSockCmdParams->dataLen = len_publish;

	App_SocketHandleSendAsync(pAppSockCmdParams);
}

void MQTTSendConnect(void){
	FLib_MemSet(pAppSockCmdParams, 0, sizeof(appSockCmdParams_t));
	int len_connect = 25;

	pAppSockCmdParams->appSocketsCmd = gSockSend;
	/* Get socket id */
	pAppSockCmdParams->sock32 = sockID;
	pAppSockCmdParams->pData = connectPack;
	pAppSockCmdParams->dataLen = len_connect;

	App_SocketHandleSendAsync(pAppSockCmdParams);
}

