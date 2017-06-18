
#ifndef PROTOBUF_PKG_H_
#define PROTOBUF_PKG_H_


#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include "svrlib.h"
#include "network/NetworkObject.h"

using namespace svrlib;
using namespace Network;
#define ENABLE_CHECK_CRC
#define ENABLE_CHECK_WPE

#pragma  pack(1)
struct packet_header_t {
	uint16		encrypt;	  //是否加密	
	uint16		cmd;      	  //消息id
	uint16       seq;			//序列号
	uint32       crc;			//CRC	
	uint16       datalen;     // 消息数据长度(不包括包头)	
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

#define PACKET_MAX_SIZE             (1024*64)
#define PACKET_HEADER_SIZE          sizeof(packet_header_t)
#define PACKET_MAX_DATA_SIZE		(PACKET_MAX_SIZE - PACKET_HEADER_SIZE)

typedef struct {
	packet_header_t		header;
	uint8               protobuf[PACKET_MAX_DATA_SIZE];
} packet_protobuf;

#pragma pack()

#ifndef PARSE_MSG_FROM_ARRAY
#define PARSE_MSG_FROM_ARRAY(msg)					\
	if (!msg.ParseFromArray(pkt_buf, buf_len)) {	\
		return -2;									\
	}												

#endif // PARSE_MSG_FROM_ARRAY

int	  GetProtobufPacketLenWithoutTrans(const uint8_t * pData, uint16_t wLen); 
int	  GetProtobufPacketLen(const uint8_t * pData, uint16_t wLen); 
bool  SendProtobufMsg(NetworkObject* pNetObj,const google::protobuf::Message* msg,uint16 msg_type, uint16 compress = 0);
bool  SendProtobufMsg(NetworkObject* pNetObj,const void* msg, uint16 msg_len, uint16 msg_type, uint16 compress = 0);
bool SendJsonMsg(NetworkObject* pNetObj,const google::protobuf::Message* msg,uint16 msg_type, uint16 encode = 0);
//   消息接收器接口
class IProtobufClientMsgRecvSink
{
public:
	//  收到客户端消息时回调
	virtual int OnRecvClientMsg(NetworkObject* pNetObj, const uint8* pkt_buf, uint16 buf_len,uint16 msg_type, void *param = NULL) = 0;	

};	
class  CProtobufMsgHanlde
{
public:
	CProtobufMsgHanlde(){m_pDefaultSink = NULL;}

	virtual  void	InitListener() = 0;
	int		OnHandleClientMsg(NetworkObject* pNetObj,const uint8_t *pData, size_t uiDataLen)
	{
		if(pData == NULL)
			return -1;	
		packet_header_t * head = (packet_header_t *)pData;
		if(head->cmd < m_vtMsgTab.size() && m_vtMsgTab[head->cmd] != NULL)
		{
			return m_vtMsgTab[head->cmd]->OnRecvClientMsg(pNetObj,pData+PACKET_HEADER_SIZE,head->datalen,head->cmd);		
		}
		if (NULL != m_pDefaultSink){
			return m_pDefaultSink->OnRecvClientMsg(pNetObj,pData+PACKET_HEADER_SIZE,head->datalen,head->cmd);
		}
		LOG_ERROR("消息未注册:%d",head->cmd);
		return 0;
	}
	int		OnRecvClientMsg(NetworkObject* pNetObj,const uint8_t *msg_data, size_t msg_len, size_t msg_no, void* param)
	{
		if(msg_data == NULL)
			return -1;	
		if(msg_no < m_vtMsgTab.size() && m_vtMsgTab[msg_no] != NULL)
		{
			return m_vtMsgTab[msg_no]->OnRecvClientMsg(pNetObj,msg_data,msg_len,msg_no, param);		
		}
		if (NULL != m_pDefaultSink){
			return m_pDefaultSink->OnRecvClientMsg(pNetObj,msg_data, msg_len, msg_no, param);
		}
		LOG_ERROR("消息未注册:%d", msg_no);
		return 0;
	}

  	bool	AddMsgListener(IProtobufClientMsgRecvSink* pSink,uint16 msg_type)
  	{
	  	if(m_vtMsgTab.size() <= msg_type){
			m_vtMsgTab.resize(msg_type + 1);
		}		
		if(m_vtMsgTab[msg_type] != NULL){
			assert(false);
			return false;
		}
		m_vtMsgTab[msg_type] = pSink;
		return true;		
  	}
  	bool	SetDefaultMsgListener(IProtobufClientMsgRecvSink* pSink)
	{
		m_pDefaultSink = pSink;
		return true;
	}
private:
	typedef vector<IProtobufClientMsgRecvSink*>  TMsgHandleVec;	
	IProtobufClientMsgRecvSink* m_pDefaultSink;
	TMsgHandleVec m_vtMsgTab;	


};














#endif // PROTOBUF_PKG_H_




