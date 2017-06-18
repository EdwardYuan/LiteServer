#ifndef APPLICATION_H
#define APPLICATION_H

#include "../base/none_copy_class.h"
#include "../net/libevent_mgr.h"
#include "../lua/lua_mgr.h"

class Application : NoneCopyClass
{
public:	
	virtual ~Application(){}

	static Application& GetInstance()
	{
		static Application m_instance;
		return m_instance;
	}

public:
	bool Start()
	{		
		return m_pEventMgr->Dispatch();
	}	

	LibEventMgr *GetEventMgr(){ return m_pEventMgr; }
	LuaMgr *GetLuaMgr(){ return m_pLuaMgr; }

	bool PreInit();
	bool Init();	
	bool PreStop();
	bool Stop();
	bool OnConfigChange();
	void Update();
private:
	Application()
	{ 
		m_pEventMgr = new LibEventMgr();
		m_pLuaMgr = new LuaMgr();
	}
	LibEventMgr *m_pEventMgr;
	LuaMgr *m_pLuaMgr;
};


#endif //APPLICATION_H

