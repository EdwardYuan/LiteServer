#include "libevent_mgr.h"
#include "net_obj.h"
#include "../tool/macro_tool.h"
#include "../timer/global_timer_mgr.h"

static int get_data_lite(struct bufferevent *bev, NetObj * pNetObj)
{
	struct evbuffer *input = bufferevent_get_input(bev);
	MsgHeader * header = pNetObj->GetMsgHeader();
	int32 iCurDataPos = pNetObj->GetCurDataPos();
	if (iCurDataPos >= PACKET_HEADER_SIZE)
	{
		size_t buffer_size = evbuffer_get_length(input);
		if (buffer_size <= 0)
		{
			return 1;
		}

		int32 iGotData = iCurDataPos - PACKET_HEADER_SIZE;
		int32 iLeftData = header->datalen - iGotData;

		if ((int)buffer_size < iLeftData)
		{
			return 1;
		}
		else
		{
			uint8           data[PACKET_MAX_DATA_SIZE];
			size_t len = bufferevent_read(bev, data + iGotData, iLeftData);
			pNetObj->AddCurDataPos(len);
			int ret = pNetObj->OnRecv(data, header->datalen);
			pNetObj->ResetCurDataPos();
			if (ret != 0)
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		size_t buffer_size = evbuffer_get_length(input);
		if (buffer_size < PACKET_HEADER_SIZE)
		{
			return 1;
		}

		size_t len = bufferevent_read(bev, header, PACKET_HEADER_SIZE);
		pNetObj->AddCurDataPos(len);
		header->NetWorkToHost();
		buffer_size = evbuffer_get_length(input);
		if (buffer_size <= 0)
		{
			return 1;
		}

		if (buffer_size < header->datalen)
		{
			return 1;
		}
		else
		{
			uint8           data[PACKET_MAX_DATA_SIZE];
			len = bufferevent_read(bev, data, header->datalen);
			pNetObj->AddCurDataPos(len);
			int ret = pNetObj->OnRecv(data, header->datalen);
			pNetObj->ResetCurDataPos();
			if (ret != 0)
			{
				return -1;
			}
			else
			{
				return 0;
			}

		}
	}

	return false;
}

static void conn_readcb(struct bufferevent *bev, void *user_data)
{
	NetObj * pNetObj = (NetObj *)user_data;
	int getData = 1;
	do
	{
		getData = get_data_lite(bev, pNetObj);
		if (-1 == getData)
		{
			pNetObj->OnDisconnect();
			if (pNetObj->GetListenerID() >= 0)
			{
				NetListener *listener = pNetObj->GetEventMgr()->GetListener(pNetObj->GetListenerID());
				if (NULL != listener)
				{
					listener->OnConnectChange(-1);
					listener->GetListenerInfo()->pNetObjFactory->FreeNetObj(pNetObj);
				}
			}
			else
			{
				delete pNetObj;
			}
			
			bufferevent_free(bev);
		}
	} while (0 == getData);
	
}

static void conn_writecb(struct bufferevent *bev, void *user_data)
{
	NetObj * pNetObj = (NetObj *)user_data;
}

static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	NetObj * pNetObj = (NetObj *)user_data;
	if ((events & BEV_EVENT_EOF) || events & BEV_EVENT_ERROR)
	{
		pNetObj->OnDisconnect();
		if (pNetObj->GetListenerID() >= 0)
		{
			NetListener *listener = pNetObj->GetEventMgr()->GetListener(pNetObj->GetListenerID());
			if (NULL != listener)
			{
				listener->OnConnectChange(-1);
				listener->GetListenerInfo()->pNetObjFactory->FreeNetObj(pNetObj);
			}
		}
		else
		{
			delete pNetObj;
		}
		bufferevent_free(bev);
	}
	else if (events & BEV_EVENT_CONNECTED)
	{
		pNetObj->OnConnect();
	}	
	
}
static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
{
	ListenerInfo *info = (struct ListenerInfo *)user_data;		
	struct event_base *base = info->pEventBase;
	struct sockaddr_in *addr = (struct sockaddr_in *)sa;
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev)
	{
		LOG_ERROR("Error constructing bufferevent!");
		return;
	}

	NetObj * obj = info->pNetObjFactory->NewNetObj();
	obj->SetIP(addr->sin_addr.S_un.S_addr);
	obj->SetSockID(fd);
	obj->SetListenerID(info->iListenerID);
	obj->SetBufferEvent(bev);
	obj->SetSockAddr(sa);
	obj->SetMsgSink(info->pMsgSink);
	obj->SetEventMgr(info->pEventMgr);
	obj->OnAccept();
	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, obj);
	bufferevent_enable(bev, EV_READ | EV_PERSIST);

	info->iCurConnection += 1;
	if (info->iCurConnection > info->iMaxConnection)
	{
		LOG_ERROR("Max Connection {0}", info->iMaxConnection);
		obj->OnDisconnect();
		info->pNetObjFactory->FreeNetObj(obj);	
		bufferevent_free(bev);
		info->iCurConnection -= 1;
	}
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = (struct event_base *)user_data;
}

static void timer_cb(evutil_socket_t fd, short event, void *params)
{
	
	QTimer *timer = (QTimer *)params;
	struct event *ev = timer->GetTimerEvent();
	bool isOver = timer->IsRemoveNextTime();
	timer->OnTimer();
	if (isOver)
	{
		evtimer_del(ev);
	}
	
}

LibEventMgr::LibEventMgr()
{
	m_bInitFlag = false;
	m_pEventBase = NULL;
	m_vecListener.clear();
	m_pSignalEvent = NULL;
	m_pTimerMgr = NULL;

	Init();
}

LibEventMgr::~LibEventMgr()
{
	Free();
}

void LibEventMgr::Free()
{
	for (size_t i = 0; i < m_vecListener.size(); i++)
	{
		m_vecListener[i].Release();
	}
	m_vecListener.clear();
	event_free(m_pSignalEvent);
	event_base_free(m_pEventBase);
	m_bInitFlag = false;
}

bool LibEventMgr::Init()
{

	if (m_bInitFlag) return false;

	m_pTimerMgr = new GlobalTimerMgr(this);

#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	m_pEventBase = event_base_new();
	if (!m_pEventBase) 
	{
		LOG_ERROR("Could not initialize libevent!");
		return false;
	}

	m_pSignalEvent = evsignal_new(m_pEventBase, SIGINT, signal_cb, (void *)m_pEventBase);

	if (!m_pSignalEvent || event_add(m_pSignalEvent, NULL)<0) 
	{
		LOG_ERROR("Could not create/add a signal event!");
		return false;
	}

	m_bInitFlag = true;
	return true;
}

bool LibEventMgr::Connect(const char *ip, uint16 port, NetObj *pNetObj, IMsgRecvSink *pMsgSink)
{
	if (NULL == pNetObj) return false;
	if (NULL == pMsgSink) return false;

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	evutil_inet_pton(AF_INET, ip, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(port);

	evutil_socket_t sockfd = ::socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		return false;
	}
	evutil_make_socket_nonblocking(sockfd);
	struct bufferevent *bev = bufferevent_socket_new(m_pEventBase, sockfd, BEV_OPT_CLOSE_ON_FREE);
	if (NULL == bev)
	{
		LOG_ERROR("bufferevent_socket_new error on connect {0}", ip);
		return false;
	}

	struct sockaddr *addr = (struct sockaddr *)&serverAddr;

	pNetObj->SetIP(serverAddr.sin_addr.S_un.S_addr);
	pNetObj->SetSockID(sockfd);
	pNetObj->SetListenerID(-1);
	pNetObj->SetBufferEvent(bev);
	pNetObj->SetSockAddr(addr);
	pNetObj->SetMsgSink(pMsgSink);
	pNetObj->SetEventMgr(this);

	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, pNetObj);
	if (-1 == bufferevent_enable(bev, EV_READ | EV_WRITE | EV_PERSIST))
	{
		LOG_ERROR("bufferevent_enable error on connect {0}", ip);
		bufferevent_free(bev);
		return false;
	}

	if (-1 == bufferevent_socket_connect(bev, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		LOG_ERROR("bufferevent_socket_connect error on connect {0}", ip);
		bufferevent_free(bev);
		return false;
	}

	return true;
}

bool LibEventMgr::AddListener(const  struct ListernConf *lis_conf)
{
	if (NULL == lis_conf->pMsgSink) return false;
	if (!m_bInitFlag) return false;
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(lis_conf->iPort);

	ListenerInfo *info = new ListenerInfo();
	info->iMaxConnection = lis_conf->iMaxConnection;
	info->pEventBase = m_pEventBase;
	info->pMsgSink = lis_conf->pMsgSink;
	info->iListenerID = m_vecListener.size();
	info->pNetObjFactory = lis_conf->pNetObjFactory;
	info->pEventMgr = this;
	struct evconnlistener *listener = NULL;
	listener = evconnlistener_new_bind(m_pEventBase, listener_cb, (void *)info, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&sin, sizeof(sin));
	if (!listener) 
	{
		LOG_ERROR("Could not create a listener!");
		return false;
	}
	NetListener lis;
	lis.SetEVListener(listener);
	lis.SetMsgSink(lis_conf->pMsgSink);
	lis.SetListenerInfo(info);
	m_vecListener.push_back(lis);
	return true;
}

NetListener *LibEventMgr::GetListener(uint32 listener_id)
{
	if (listener_id >= m_vecListener.size()) return NULL;
	return &m_vecListener[listener_id];
}

bool LibEventMgr::Dispatch()
{
	if (!m_bInitFlag) return false;
	event_base_dispatch(m_pEventBase);
	return true;
}

NetObj *LibEventMgr::OnAccept(evutil_socket_t fd, struct sockaddr *sa, int socklen)
{
	struct sockaddr_in *addr = (struct sockaddr_in *)sa;
	LOG_DEBUG("client {0}, From IP : {1}\n", fd, inet_ntoa(addr->sin_addr));
	return NULL;
}

bool LibEventMgr::AddTimer(QTimer *timer)
{
	int32 period = timer->GetPeriod();
	struct event *timeout = NULL;
	struct timeval tv = { period / 1000, (period % 1000) * 1000 };
	timeout = event_new(m_pEventBase, -1, EV_PERSIST, timer_cb, (void *)timer);
	if (NULL == timeout) return false;	
	int ret = evtimer_add(timeout, &tv);
	if (0 != ret) return false;
	timer->SetTimerEvent(timeout);
	return true;
}








