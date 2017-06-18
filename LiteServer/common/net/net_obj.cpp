#include "net_obj.h"
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>

NetObj::NetObj()
	:m_iListenerID(0)
	, m_dwSockID(0)
	, m_dwIP(0)
	, m_pBufferEvent(NULL)
	, m_MsgSink(NULL)
	, m_EventMgr(NULL)
	, m_iCurDataPos(0)
{

}

void NetObj::Init()
{
	m_iListenerID = -1;
	m_dwSockID = 0;
	m_dwIP = 0;
	m_iCurDataPos = 0;
	m_pBufferEvent = NULL;
	m_MsgSink = NULL;
	m_EventMgr = NULL;
	m_iCurDataPos = 0;
}

bool NetObj::Send(uint8 *pMsg, uint16 wSize)
{
	if (-1 != bufferevent_write(m_pBufferEvent, pMsg, wSize))
	{
		return true;
	}
	
	return false;
}

void NetObj::OnAccept()
{
	LOG_DEBUG("Accepted ! {0}", m_dwSockID);
}


void NetObj::OnDisconnect()
{
	LOG_DEBUG("Disconnected ! {0}", m_dwSockID);
}

int NetObj::OnRecv(uint8 *pMsg, uint16 wSize)
{
	//if (m_MsgPackData.header.crc != 1) return -1;
	//if (m_MsgHeader.crc != 1) return -1;
	m_MsgSink->OnRecvMsg(this, pMsg, wSize);
	return 0;
}

void NetObj::OnConnect()
{
	LOG_DEBUG("connect succ! {0}", m_dwSockID);
}

