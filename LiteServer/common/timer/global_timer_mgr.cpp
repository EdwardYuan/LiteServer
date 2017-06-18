#include "global_timer_mgr.h"
#include "../net/libevent_mgr.h"
#include "q_timer.h"

GlobalTimerMgr::GlobalTimerMgr(LibEventMgr *pEventMgr)
	:m_dwCurMaxID(0)
	, m_pEventMgr(pEventMgr)
{

}

GlobalTimerMgr::~GlobalTimerMgr()
{

}

void GlobalTimerMgr::Init()
{
	m_dwCurMaxID = 0;
	std::queue<uint32> tmpQue;
	m_queTimerIDFactory.swap(tmpQue);
	m_mapTimerList.clear();
	m_pEventMgr = NULL;
}

QTimer * GlobalTimerMgr::AddTimer(int32 period, int32 times, IQTimerCallBack *pCallBack)
{
	if (NULL == pCallBack)
	{
		return NULL;
	}

	uint32 newID = GetNewTimerID();
	QTimer *timer = m_TimerPool.PopEntity();
	if (NULL == timer)
	{
		timer = new QTimer(this, period, times, newID, pCallBack);
	}
	else
	{
		timer->SetTimer(this, period, times, newID, pCallBack);
	}
		
	if (!m_pEventMgr->AddTimer(timer))
	{
		timer->Destroy();
		m_TimerPool.PushEntity(timer);
		return NULL;
	}

	m_mapTimerList[newID] = timer;

	return timer;
}

void GlobalTimerMgr::RemoveTimer(uint32 timerID)
{
	m_mapTimerList.erase(timerID);
	m_queTimerIDFactory.push(timerID);
}

uint32 GlobalTimerMgr::GetNewTimerID()
{
	uint32 newID = 0;
	if (m_queTimerIDFactory.empty())
	{
		++m_dwCurMaxID;
		newID = m_dwCurMaxID;
	}
	else
	{
		newID = m_queTimerIDFactory.front();
		m_queTimerIDFactory.pop();
	}

	return newID;
}


