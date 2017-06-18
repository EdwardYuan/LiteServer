#pragma once

#include "net/net_obj.h"
#include "net/msg_package.h"


class TestSink : public IMsgRecvSink
{
public:
	virtual int OnRecvMsg(NetObj* pNetObj, const uint8* data, uint16 data_len){ LOG_INFO("Sink 1 {0}", data);  return 0; }
};

class TestSink2 : public IMsgRecvSink
{
public:
	virtual int OnRecvMsg(NetObj* pNetObj, const uint8* data, uint16 data_len){ LOG_INFO("Sink 2 {0}", data); return 0; }
};

class ClientNetObj : public NetObj
{
public:

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

int test_listener()
{
	MsgPackage pack;
	pack.header.HostToNetWork();

	struct ListernConf stListenConf;
	IMsgRecvSink *sink = new TestSink;
	stListenConf.iPort = 5555;
	stListenConf.iMaxConnection = 10;
	stListenConf.pMsgSink = sink;
	stListenConf.pNetObjFactory = new ClientNetObjFactory();
	if (!Application::GetInstance().GetEventMgr()->AddListener(&stListenConf))
	{
		LOG_ERROR("AddListener failed on port {0}", 5555);
		delete sink;
		return -4;
	}

	sink = new TestSink2;
	stListenConf.iPort = 5556;
	stListenConf.iMaxConnection = 20;
	stListenConf.pMsgSink = sink;
	stListenConf.pNetObjFactory = new ClientNetObjFactory();
	if (!Application::GetInstance().GetEventMgr()->AddListener(&stListenConf))
	{
		LOG_ERROR("AddListener failed on port {0}", 5556);
		delete sink;
		return -4;
	}

	sink = new TestSink2;
	NetObj *pConnectObj = new NetObj();
	if (!Application::GetInstance().GetEventMgr()->Connect("127.0.0.1", 5556, pConnectObj, sink))
	{
		
		delete sink;
		delete pConnectObj;
		LOG_ERROR("Connect failed on port {0}", 5556);
	}

	MsgPackage pkg;
	pkg.header.crc = 1;
	pkg.header.cmd = 1;
	pkg.header.encrypt = 0;
	pkg.header.datalen = 4;
	pkg.data[0] = 65;
	pkg.data[1] = 66;
	pkg.data[2] = 67;
	pkg.data[3] = 0;
	pkg.header.HostToNetWork();
	pConnectObj->Send((uint8 *)&pkg, sizeof(pkg.header) + 5);

	return 0;
}

