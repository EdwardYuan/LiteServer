#include "commonlib.h"
#include "msgdefine.h"


#include "test_log.h"
#include "test_aes.h"
#include "test_listener.h"
#include "test_timer.h"
#include "test_openssl.h"
#include "test_sql.h"



#pragma comment(lib,"common.lib")
#pragma comment(lib,"msgdefine.lib")


#pragma comment(lib,"libevent_core.lib")
#pragma comment(lib,"libprotobuf.lib")
#pragma comment(lib,"lua53.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "mysqlpp_d.lib")
#ifdef _WIN32
#pragma comment(lib,"Ws2_32.lib")
#endif




int main(int argc, char **argv)
{
	LOG_ERROR("Server Start");
	
	//test_spdlog();
	Json::Value root;//test json;
	pbmsg::AddressBook addr;//test protobuf;
	test_encrypt_cbc();//test aes;
	//lua_State *L = luaL_newstate();//test lua;
	//if (L == NULL) return -1;
	//luaL_openlibs(L);	
	//int bRet = luaL_loadfile(L, "config/main.lua");
	//if (bRet) return -2;
	//bRet = lua_pcall(L, 0, 0, 0);
	//if (bRet) return -3;
	//lua_close(L);
	testRSA();	
	test_timer();
	if (0 != test_listener())
	{
		LOG_ERROR("test_listener err");
		return -4;
	}
	
	if (!Application::GetInstance().GetLuaMgr()->LoadFile("script.lua"))
	{
		LOG_ERROR("Load script.lua fail");
	}

	test_sql();
	
	LOG_DEBUG("Begin event loop");
	Application::GetInstance().Start();

	return 0;
}





