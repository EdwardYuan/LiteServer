#pragma once

#include "timer/q_timer.h"
#include "timer/global_timer_mgr.h"
#include "tool/macro_tool.h"
#include "./lua/lua_param.h"

class TestTimerCallBack : public IQTimerCallBack
{
public:
	TestTimerCallBack(std::string name)
		:m_iCount(0)
		, m_sTimerName(name)
	{}
	virtual void Run()
	{
		++m_iCount;
		LuaParam param[2];
		LuaParam out;
		out.SetType(SD_DOUBLE);
		param[0] = m_iCount;
		param[1] = 0.99;
		Application::GetInstance().GetLuaMgr()->RunFunction("TestFunc", param, 2, &out, 1);

		double ret = (double)out;
		LOG_DEBUG("LUA Func Ret {0}", ret);
	}

	void SetName(std::string name) { m_sTimerName = name; }

private:
	int32 m_iCount;
	std::string m_sTimerName;
};


void test_timer()
{
	Application::GetInstance().GetEventMgr()->GetTimerMgr()->AddTimer(100, 5, new TestTimerCallBack("Timer 5"));
	//Application::GetInstance().GetEventMgr()->GetTimerMgr()->AddTimer(1000, -1, new TestTimerCallBack("Timer X"));
	//Application::GetInstance().GetEventMgr()->GetTimerMgr()->AddTimer(100, 30, new TestTimerCallBack("Timer S"));
}

