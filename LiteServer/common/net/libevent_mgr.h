#pragma once

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef _WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <vector>
#include "../base/type_def.h"
#include "net_listener.h"
#include "../timer/q_timer.h"
#include "../tool/recycle_pool.h"
#include "msg_package.h"

class NetObj;
class QTimer;
class GlobalTimerMgr;

class LibEventMgr
{
public:
	LibEventMgr();
	~LibEventMgr();

	bool Init();
	void Free();
	bool Connect(const char *ip, uint16 port, NetObj *pNetObj, IMsgRecvSink *pMsgSink);
	bool AddListener(const  struct ListernConf *lis_conf);	
	NetListener *GetListener(uint32 listener_id);	
	NetObj *OnAccept(evutil_socket_t fd, struct sockaddr *sa, int socklen);
	bool AddTimer(QTimer *timer);
	bool Dispatch();
	
public:	
	GlobalTimerMgr *GetTimerMgr(){ return m_pTimerMgr; }
private:
	bool m_bInitFlag;	
	struct event_base * m_pEventBase;
	std::vector<NetListener> m_vecListener;
	struct event * m_pSignalEvent;
	GlobalTimerMgr *m_pTimerMgr;
};




