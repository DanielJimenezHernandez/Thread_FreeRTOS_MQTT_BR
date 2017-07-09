/* Methods to connect to an UDP bridge to transfer MQTT packets*/
#include <sockets.h>
#include <network_utils.h>
#include <shell.h>
#include <app_socket_utils.h>
#include "MQTTPacket.h"

/*IP Version */
#define UNSPEC       0   /*!< Unspecified sockets */
#define INET         2   /*!< Internet IP Protocol */
#define INET6        10  /*!< IP version 6 */

int32_t sockFd;

extern taskMsgQueue_t *mpAppThreadMsgQueue;

void MQTTConnectUDP(int udpPort, char * udpAddr){
#if 0
    uint32_t connectResult,ptonResult;

    sockaddrIn_t   localInfo;
    sockaddrIn_t   remoteInfo;
    ipAddr_t 	   remoteAddr;
    char ipaddress[128];
/*
    ptonResult = pton(AF_INET,"255.255.255.255",&remoteAddr);
    shell_printf("pton Result:%d",ptonResult);
*/
    remoteAddr.addr8[15] = 0x05;
    remoteAddr.addr8[14] = 0x01;
    remoteAddr.addr8[13] = 0xA8;
    remoteAddr.addr8[12] = 0xC0;
    ntop(AF_INET,&remoteAddr,ipaddress,128);
    shell_printf("Address from ntop:%s\n",ipaddress);

    shell_write("Opening Socket... ");


	(&localInfo)->sin_family = AF_INET;
	(&localInfo)->sin_port = 1234;
	 IP_AddrCopy(&(&localInfo)->sin_addr, &inaddr_any);

	 /* Create socket */
	 sockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if(sockFd)
        {

		uint32_t bindErr;

		/* Bind socket to local information */
		bindErr = bind(sockFd, (sockaddrStorage_t*)&localInfo, sizeof(sockaddrStorage_t));

		switch(bindErr)
		{
			case gBsdsSockSuccess_c:
				/* Set remote information for easier send */

				((sockaddrIn_t*)&remoteInfo)->sin_port = udpPort;
				IP_AddrCopy(&(&remoteInfo)->sin_addr, &remoteAddr);

				/* Set Remote information */
				connectResult = connect(sockFd, (sockaddrStorage_t*)&remoteInfo, sizeof(sockaddrStorage_t));
				if (0 == connectResult){
					shell_printf("Connected to remote server...\n");
				}
				else{
					shell_printf("ERROR Connecting to remote server..\n");
				}
				break;
			default:
				shutdown(sockFd, 0);
				shell_write("ERROR\n\rBind went wrong");
				break;
		}

         } /* socket was created successfully */
         else
         {
            shell_write("ERROR\n\rCreating socket failed");
         }
#endif

       // APP_InitUserSockets(mpAppThreadMsgQueue);
}

void MQTTUdpSend(uint8_t *pdata, uint8_t datalen){
	int result;
	result = send(sockFd,pdata,datalen,0);
	shell_printf("Result of send:%d\n",result);
}
