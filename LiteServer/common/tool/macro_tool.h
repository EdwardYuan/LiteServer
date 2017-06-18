#ifndef MACRO_TOOL_H
#define MACRO_TOOL_H

#include "log_mgr.h"

//#define SAFE_DELETE(x)		{if(x){delete (x);(x)=0;}}
//#define SAFE_RELEASE(x)		{if(x){(x)->Release();(x)=0;}}
//#define SAFE_RESTORE(x)		{if(x){(x)->Restore();(x)=0;}}

#define SAFE_DELETE(x)\
do\
{\
	if(x){delete (x);(x)=0;}\
}while(0);

#define SAFE_RELEASE(x)\
do\
{\
	if(x){(x)->Release();(x)=0;}\
}while(0);

#define SAFE_RESTORE(x)\
do\
{\
	if(x){(x)->Restore();(x)=0;}\
}while(0);


#define LOG_DEBUG(...)\
do\
{\
	LogMgr::GetInstance().GetLogger()->debug(__VA_ARGS__);\
	LogMgr::GetInstance().GetConsoleLogger()->debug(__VA_ARGS__);\
}while(0);

#define LOG_INFO(...)\
do\
{\
LogMgr::GetInstance().GetLogger()->info(__VA_ARGS__);\
LogMgr::GetInstance().GetConsoleLogger()->info(__VA_ARGS__);\
}while(0);


#define LOG_WARN(...)\
do\
{\
LogMgr::GetInstance().GetLogger()->warn(__VA_ARGS__);\
LogMgr::GetInstance().GetConsoleLogger()->warn(__VA_ARGS__);\
}while(0);

#define LOG_ERROR(...)\
do\
{\
LogMgr::GetInstance().GetLogger()->error("{0}:{1}:{2}", __FUNCTION__, __FILE__, __LINE__); \
LogMgr::GetInstance().GetLogger()->error(__VA_ARGS__);\
LogMgr::GetInstance().GetConsoleLogger()->error("{0}:{1}:{2}", __FUNCTION__, __FILE__, __LINE__); \
LogMgr::GetInstance().GetConsoleLogger()->error(__VA_ARGS__);\
}while(0);


#endif
