#include "log_mgr.h"

LogMgr::LogMgr()
{
	spdlog::set_level(spdlog::level::trace);
	spdlog::set_async_mode(1024);
	spdlog::set_pattern("[%H:%M:%S %z] [%l] %v");
	m_logger = spdlog::daily_logger_mt("daily_logger", "logs/log", 0, 0);
	m_logger->flush_on(spdlog::level::trace);
	m_console_logger = spdlog::stdout_color_mt("console");
}

LogMgr::~LogMgr()
{

}

bool LogMgr::Init()
{
	return true;
}

