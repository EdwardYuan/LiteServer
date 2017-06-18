#ifndef MSG_PACKAGE_H
#define MSG_PACKAGE_H

#include "../base/type_def.h"

#ifndef _WIN32
#include <arpa/inet.h>
#else
#include <winsock.h>
#endif



#pragma  pack(1)
struct MsgHeader {
	uint16		encrypt;	  //是否加密	
	uint16		cmd;      	  //消息号
	uint16       seq;		  //流水号
	uint32       crc;		  //校验
	uint16       datalen;     //数据长度	
	void NetWorkToHost()
	{
		encrypt = ntohs(encrypt);
		cmd = ntohs(cmd);
		seq = ntohs(seq);
		datalen = ntohs(datalen);
		crc = ntohl(crc);
	}
	void HostToNetWork()
	{
		encrypt = htons(encrypt);
		cmd = htons(cmd);
		seq = htons(seq);
		datalen = htons(datalen);
		crc = htonl(crc);
	}
};

#define PACKET_MAX_SIZE             (1024 * 64)
#define PACKET_HEADER_SIZE          sizeof(MsgHeader)
#define PACKET_MAX_DATA_SIZE		(PACKET_MAX_SIZE - PACKET_HEADER_SIZE)

typedef struct {
	MsgHeader		header;
	uint8           data[PACKET_MAX_DATA_SIZE];
} MsgPackage;

#pragma pack()


#endif // MSG_PACKAGE_H



