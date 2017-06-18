#include "gateway_net_mgr.h"
#include "commonlib.h"

GatewayNetMgr::GatewayNetMgr()
{
}

GatewayNetMgr::~GatewayNetMgr()
{
}

bool GatewayNetMgr::Init()
{
	struct ListernConf stListenConf;
	IMsgRecvSink *sink = new ClientMsgSink;
	stListenConf.iPort = 7777;
	stListenConf.iMaxConnection = 50000;
	stListenConf.pMsgSink = sink;
	stListenConf.pNetObjFactory = new ClientNetObjFactory();
	if (!Application::GetInstance().GetEventMgr()->AddListener(&stListenConf))
	{
		LOG_ERROR("AddListener failed on port {0}", stListenConf.iPort);
		delete stListenConf.pMsgSink;
		delete stListenConf.pNetObjFactory;
		return false;
	}

	return true;
}



