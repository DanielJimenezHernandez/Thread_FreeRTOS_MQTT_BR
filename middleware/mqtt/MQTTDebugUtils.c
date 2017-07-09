#include <shell.h>

void MQTTPrintSerializedPacket(unsigned char * buffer,int bufferLen,const char * description){
	int i;

	shell_printf("%s:",description);
	for (i = 0; i < bufferLen; i++){
		shell_printf("[0x%02x]",buffer[i]);
	}
	shell_printf("\n");
}

