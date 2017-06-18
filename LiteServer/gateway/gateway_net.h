#ifndef GATEWAY_NET_H
#define GATEWAY_NET_H

#include "net/net_obj.h"
#include "net/msg_package.h"
#include "tool/id_factory.h"

class ClientMsgSink : public IMsgRecvSink
{
public:
	virtual int OnRecvMsg(NetObj* pNetObj, const uint8* data, uint16 data_len){ LOG_INFO("ClientSink {0}", data); return 0; }
};

class ClientNetObj : public NetObj
{
public:
	virtual void OnAccept();
	virtual void OnDisconnect();

private:
	uint32 m_dwClientID;
};

typedef TRecyclePool<ClientNetObj, 1000> ClientNetObjRecyclePool;


class ClientNetObjFactory : public INetObjFactory
{
public:
	virtual NetObj *NewNetObj()
	{
		NetObj * obj = m_ObjPool.PopEntity();
		if (NULL == obj)
		{
			obj = new ClientNetObj;
		}

		obj->Init();
		return obj;
	}
	virtual void FreeNetObj(NetObj * pNetObj)
	{
		if (NULL != pNetObj)
		{
			m_ObjPool.PushEntity((ClientNetObj *)pNetObj);
		}
	}

private:
	ClientNetObjRecyclePool m_ObjPool;
};


#endif //GATEWAY_NET_H




