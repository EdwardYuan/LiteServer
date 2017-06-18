#ifndef GLOBAL_TIMER_MGR_H
#define GLOBAL_TIMER_MGR_H

#include "../base/type_def.h"
#include "../tool/recycle_pool.h"
#include <queue>
#include <map>

class QTimer;
class IQTimerCallBack;
class LibEventMgr;

typedef TRecyclePool<QTimer, 10000> QTimerPool;

class GlobalTimerMgr
{
public:
	GlobalTimerMgr(LibEventMgr *pEventMgr);
	~GlobalTimerMgr();

	void Init();
	QTimer * AddTimer(int32 period, int32 times, IQTimerCallBack *pCallBack);
	void RemoveTimer(uint32 timerID);
	uint32 GetNewTimerID();

	

private:
	uint32 m_dwCurMaxID;
	std::queue<uint32> m_queTimerIDFactory;
	std::map<uint32, QTimer *> m_mapTimerList;
	QTimerPool m_TimerPool;
	LibEventMgr *m_pEventMgr;
};


#endif	//GLOBAL_TIMER_MGR_H


