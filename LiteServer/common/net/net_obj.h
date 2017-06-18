#ifndef NET_OBJ_H
#define NET_OBJ_H

#include <event2/util.h>

#include "../base/type_def.h"
#include "../tool/recycle_pool.h"
#include "../base/none_copy_class.h"
#include "msg_package.h"


class NetObj;
class LibEventMgr;

class IMsgRecvSink : NoneCopyClass
{
public:
	//  收到消息时回调
	virtual int OnRecvMsg(NetObj* pNetObj, const uint8* data, uint16 data_len) = 0;
};

class NetObj
{
public:
	NetObj();
	virtual ~NetObj(){}

public:	
	void Init();
	bool Send(uint8 *pMsg, uint16 wSize);		
	virtual void	OnAccept();
	virtual void	OnDisconnect();
	virtual	int 	OnRecv(uint8 *pMsg, uint16 wSize);
	virtual void	OnConnect();
	virtual void Release() { delete this; }
	
public:
	inline uint32 GetIP(){ return m_dwIP; }
	inline void SetIP(uint32 ip){ m_dwIP = ip; }
	inline uint32 GetSockID(){ return m_dwSockID; }	
	inline void SetSockID(uint32 sockID){ m_dwSockID = sockID; }
	inline int32 GetListenerID(){ return m_iListenerID; }
	inline void SetListenerID(int32 listener_id){ m_iListenerID = listener_id; }
	inline struct bufferevent * GetBufferEvent(){ return m_pBufferEvent; }
	inline void SetBufferEvent(struct bufferevent *bev){ m_pBufferEvent = bev; }
	inline void SetSockAddr(const struct sockaddr *sa){ m_stAddr = *sa; }
	inline struct sockaddr * GetSockAddr(){ return &m_stAddr; }
	inline void SetMsgSink(IMsgRecvSink *msg_sink){ m_MsgSink = msg_sink; }
	inline LibEventMgr * GetEventMgr(){ return m_EventMgr; }
	inline void SetEventMgr(LibEventMgr *event_mgr){ m_EventMgr = event_mgr; }

	inline int32 GetCurDataPos() { return m_iCurDataPos; }
	inline void AddCurDataPos(int32 pos) { m_iCurDataPos += pos; }
	inline void ResetCurDataPos() { m_iCurDataPos = 0; }
	//inline MsgPackage *GetPackage(){ return &m_MsgPackData; }
	inline MsgHeader *GetMsgHeader(){ return &m_MsgHeader; }
private:
	int32 m_iListenerID;
	uint32 m_dwSockID;
	uint32 m_dwIP;
	struct bufferevent *m_pBufferEvent;
	struct sockaddr m_stAddr;
	IMsgRecvSink *m_MsgSink;
	LibEventMgr *m_EventMgr;
	//MsgPackage m_MsgPackData;
	MsgHeader m_MsgHeader;
	int32 m_iCurDataPos;
};

class INetObjFactory
{
public:
	virtual NetObj *NewNetObj() = 0;
	virtual void FreeNetObj(NetObj * pNetObj) = 0;
};

#endif	//NET_OBJ_H

