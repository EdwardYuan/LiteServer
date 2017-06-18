#ifndef GATEWAY_NET_MGR_H
#define GATEWAY_NET_MGR_H

#include "gateway_net.h"

typedef IDFactory<uint32> ClientIDPool;
class GatewayNetMgr : NoneCopyClass
{
public:
	static GatewayNetMgr& GetInstance()
	{
		static GatewayNetMgr m_instance;
		return m_instance;
	}
		
	~GatewayNetMgr();
	bool Init();

	uint32 GetClientID(){ return m_ClientIDPool.GetNewID(); }
	void FreeClientID(uint32 id){ if (0 == id) return;	m_ClientIDPool.FreeID(id); }
private:
	GatewayNetMgr();
	ClientIDPool m_ClientIDPool;
};



#endif //GATEWAY_NET_MGR_H




