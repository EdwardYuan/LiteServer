#include "commonlib.h"
#include "msgdefine.h"

#pragma comment(lib,"common.lib")
#pragma comment(lib,"msgdefine.lib")

#pragma comment(lib,"libevent_core.lib")
#pragma comment(lib,"libprotobuf.lib")
#pragma comment(lib,"lua53.lib")
#ifdef _WIN32
#pragma comment(lib,"Ws2_32.lib")
#endif

#include "gateway_net_mgr.h"

bool Application::Init()
{
	if (!GatewayNetMgr::GetInstance().Init())
	{
		return false;
	}

	return true;
}

bool Application::PreStop()
{
	return true;
}

bool Application::Stop()
{
	return true;
}

bool Application::OnConfigChange()
{
	return true;
}

void Application::Update()
{

}

class UpdateTimerCallBack : public IQTimerCallBack
{
public:

	virtual void Run()
	{
		Application::GetInstance().Update();
	}

};

#define SERVER_UPDATE_INTERNER 50

int main(int argc, char **argv)
{
	if (!Application::GetInstance().Init())
	{
		return -1;
	}

	if (!Application::GetInstance().GetEventMgr()->GetTimerMgr()->AddTimer(SERVER_UPDATE_INTERNER, -1, new UpdateTimerCallBack()))
	{
		return -1;
	}	

	LOG_INFO("Server Start");
	if (!Application::GetInstance().Start())
	{
		return -1;
	}
	
	return 0;
}

