#include "gateway_net.h"
#include "gateway_net_mgr.h"

void ClientNetObj::OnAccept()
{ 
	m_dwClientID = GatewayNetMgr::GetInstance().GetClientID();
	LOG_DEBUG("Client OnConnect {0}", m_dwClientID);
}

void ClientNetObj::OnDisconnect()
{ 
	GatewayNetMgr::GetInstance().FreeClientID(m_dwClientID);
	LOG_DEBUG("Client DisConnect {0}", m_dwClientID);
}

