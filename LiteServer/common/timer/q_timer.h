#ifndef Q_TIMER_H
#define Q_TIMER_H

#include "../base/type_def.h"
#include "../base/none_copy_class.h"
#include <event2/util.h>

class GlobalTimerMgr;

class IQTimerCallBack
{
public:
	virtual void Run() = 0;
};

class QTimer : NoneCopyClass
{
public:
	QTimer();	
	QTimer(GlobalTimerMgr *pTimerMgr, int32 period, int32 times, uint32 timerID, IQTimerCallBack *pCallBack)
		:m_dwPeriod(period)
		,m_dwTimes(times)
		,m_dwCurRunTimes(0)
		, m_dwID(timerID)
		,m_pCallBack(pCallBack)
		,m_pTimerMgr(pTimerMgr)
	{
	}
	~QTimer(){}

public:
	void SetTimer(GlobalTimerMgr *pTimerMgr, int32 period, int32 times, uint32 timerID, IQTimerCallBack *pCallBack);
	void SetTimerEvent(struct event *ev) { m_TimerEvent = ev; }
	struct event *  GetTimerEvent() { return m_TimerEvent; }
	bool IsRemoveNextTime() { return (m_dwTimes > 0) && (m_dwCurRunTimes + 1 >= m_dwTimes); };
	int32 GetPeriod();
	void OnTimer();
	void Destroy();
	void Release();

private:
	int32 m_dwPeriod;
	int32 m_dwTimes;	
	int32 m_dwCurRunTimes;
	uint32 m_dwID;
	IQTimerCallBack *m_pCallBack;
	struct event * m_TimerEvent;
	GlobalTimerMgr *m_pTimerMgr;
};

#endif	//Q_TIMER_H

