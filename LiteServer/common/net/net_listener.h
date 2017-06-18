#ifndef NET_LISTENER_H
#define NET_LISTENER_H

#include <event2/util.h>
#include "../base/type_def.h"

class IMsgRecvSink;
class INetObjFactory;
class LibEventMgr;

struct ListernConf
{
	ListernConf()
		: iPort(0)
		, iMaxConnection(0)
		, pMsgSink(NULL)
		, pNetObjFactory(NULL)
	{

	}
	int32 iPort;
	int32 iMaxConnection;
	IMsgRecvSink * pMsgSink;
	INetObjFactory *pNetObjFactory;
};

struct ListenerInfo
{
	ListenerInfo()
		:iListenerID(0)
		, iCurConnection(0)
		, iMaxConnection(0)
		, pEventBase(NULL)
		, pMsgSink(NULL)
		, pNetObjFactory(NULL)
		, pEventMgr(NULL)
	{

	}
	int32 iListenerID;
	int32 iCurConnection;
	int32 iMaxConnection;
	struct event_base * pEventBase;
	IMsgRecvSink * pMsgSink;
	INetObjFactory *pNetObjFactory;
	LibEventMgr *pEventMgr;
};

class NetListener
{
public:
	NetListener();
	~NetListener();
	void SetEVListener(struct evconnlistener * Listener);
	void SetMsgSink(IMsgRecvSink * pMsgSink);
	void SetListenerInfo(ListenerInfo * stListenInfo);
	ListenerInfo *GetListenerInfo(){ return m_stListenInfo; }
	void OnConnectChange(int32 num);
	void Release();
private:
	struct evconnlistener * m_stListener;
	IMsgRecvSink * m_pMsgSink;
	ListenerInfo * m_stListenInfo;
};

#endif



