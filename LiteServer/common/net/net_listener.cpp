#include <event2/listener.h>

#include "net_listener.h"
#include "net_obj.h"

NetListener::NetListener()
	:m_stListener(NULL)
	, m_pMsgSink(NULL)
	, m_stListenInfo(NULL)
{

}

NetListener::~NetListener()
{

}

void NetListener::SetEVListener(struct evconnlistener * Listener)
{ 
	m_stListener = Listener; 
}

void NetListener::SetMsgSink(IMsgRecvSink * pMsgSink)
{ 
	m_pMsgSink = pMsgSink;
}

void NetListener::SetListenerInfo(ListenerInfo * stListenInfo)
{ 
	m_stListenInfo = stListenInfo;
}

void NetListener::OnConnectChange(int32 num)
{ 
	m_stListenInfo->iCurConnection += num;
}

void NetListener::Release()
{
	evconnlistener_free(m_stListener);
	delete m_pMsgSink;
	delete m_stListenInfo;
}





