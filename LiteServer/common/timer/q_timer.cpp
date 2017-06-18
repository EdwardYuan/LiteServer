#include "q_timer.h"
#include "global_timer_mgr.h"

int32 QTimer::GetPeriod()
{ 
	return m_dwPeriod;
}

void QTimer::OnTimer()
{
	m_pCallBack->Run();
	++m_dwCurRunTimes;
	if (m_dwTimes > 0 && m_dwCurRunTimes > m_dwTimes)
	{
		Destroy();
		m_pTimerMgr->RemoveTimer(m_dwID);
	}

}

void QTimer::Destroy()
{
	delete m_pCallBack;
}

void QTimer::Release()
{
	delete this;
}

void QTimer::SetTimer(GlobalTimerMgr *pTimerMgr, int32 period, int32 times, uint32 timerID, IQTimerCallBack *pCallBack)
{
	m_dwPeriod = period;
	m_dwTimes = times;
	m_dwCurRunTimes = 0;
	m_dwID = timerID;
	m_pCallBack = pCallBack;
	m_pTimerMgr = pTimerMgr;
}

