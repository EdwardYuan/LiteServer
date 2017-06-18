#ifndef LOG_MGR_H
#define LOG_MGR_H

#include "spdlog/spdlog.h"
#include "../base/none_copy_class.h"

class LogMgr
{
public:
	static LogMgr& GetInstance()
	{
		static LogMgr m_instance;
		return m_instance;
	}
	~LogMgr();

public:
	std::shared_ptr<spdlog::logger> GetLogger(){ return m_logger; }
	std::shared_ptr<spdlog::logger> GetConsoleLogger(){ return m_console_logger; }

private:
	LogMgr();
	bool Init();
private:
	std::shared_ptr<spdlog::logger> m_logger;
	std::shared_ptr<spdlog::logger> m_console_logger;
};

#endif

