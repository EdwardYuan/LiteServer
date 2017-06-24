#include "lua_mgr.h"
#include "../tool/macro_tool.h"

LuaMgr::LuaMgr()
{
	Init();
}

LuaMgr::~LuaMgr()
{

}

bool LuaMgr::Init()
{
	m_pLuaState = luaL_newstate();
	if (m_pLuaState == NULL)
	{
		return false;
	}

	luaL_openlibs(m_pLuaState);

	return true;
}

bool LuaMgr::Free()
{
	if (m_pLuaState != NULL)
	{
		lua_close(m_pLuaState);
		m_pLuaState = NULL;
	}

	return true;
}

bool LuaMgr::LoadFile(const char* szFileName)
{
	if (szFileName == NULL)
	{
		return false;
	}
	int top = lua_gettop(m_pLuaState);

	try
	{
		int nResult = luaL_loadfile(m_pLuaState, szFileName);
		if (nResult == 0)
		{
			nResult = lua_pcall(m_pLuaState, 0, 0, 0);
			if (nResult == 0)
			{
				lua_settop(m_pLuaState, top);
				return true;
			}
		}
	}
	catch (...)
	{

	}

	const char* pszErrInfor = lua_tostring(m_pLuaState, -1);
	LOG_ERROR("Lua加载文件{0}失败，原因：{1}", szFileName, pszErrInfor);
	lua_settop(m_pLuaState, top);
	return false;
}

bool LuaMgr::RunMemory(const char* pLuaData, int nDataLen)
{
	if (pLuaData == NULL || nDataLen <= 0)
	{
		return false;
	}

	int top = lua_gettop(m_pLuaState);
	int nResult = luaL_loadbuffer(m_pLuaState, pLuaData, nDataLen, "bc__MemoryLua__20015");
	if (nResult == 0)
	{
		nResult = lua_pcall(m_pLuaState, 0, 0, 0);
		if (nResult == 0)
		{
			lua_settop(m_pLuaState, top);
			return true;
		}
	}

	const char* pszErrInfor = lua_tostring(m_pLuaState, -1);
	LOG_ERROR("Lua执行内存lua失败，原因：{0}", pszErrInfor);
	lua_settop(m_pLuaState, top);
	return false;
}

bool LuaMgr::RunFunction(const char* szFunName, LuaParam* pInParam, int nInNum, LuaParam* pOutParam, int nOutNum)
{
	int top = lua_gettop(m_pLuaState);
	try
	{
		lua_getglobal(m_pLuaState, szFunName);
		if (!lua_isfunction(m_pLuaState, -1))
		{
			LOG_ERROR("Lua调用函数{0}失败, 找不到该函数", szFunName);
			lua_settop(m_pLuaState, top);
			return false;
		}

		for (int i = 0; i < nInNum; i++)
		{
			int nValueType = pInParam[i].GetType();
			switch (nValueType)
			{
			case SD_NUMBER:
			{
				lua_pushinteger(m_pLuaState, (int64)pInParam[i]);
			}
				break;
			case SD_DOUBLE:
			{
				lua_pushnumber(m_pLuaState, (double)pInParam[i]);
			}
				break;
			case SD_STRING:
			{
				lua_pushstring(m_pLuaState, (const char*)pInParam[i]);
			}
				break;
			case SD_POINTER:
			{
				lua_pushlightuserdata(m_pLuaState, (void *)pInParam[i]);
			}
				break;
			default:
			{
				LOG_ERROR("Lua调用函数{0}失败，输入参数{1}类型错误", szFunName, i);
				lua_settop(m_pLuaState, top);
				return false;
			}
			}
		}

		// 调用执行
		int nResult = lua_pcall(m_pLuaState, nInNum, nOutNum, 0);
		if (nResult == 0)
		{
			int n = 0;
			for (n = nOutNum - 1; n >= 0; n--)
			{
				int nValueType = pOutParam[n].GetType();
				switch (nValueType)
				{
				case SD_NUMBER:
				{
					pOutParam[n] = (int64)lua_tonumber(m_pLuaState, -1);
					lua_pop(m_pLuaState, 1);
				}
					break;
				case SD_DOUBLE:
				{
					pOutParam[n] = (double)lua_tonumber(m_pLuaState, -1);
					lua_pop(m_pLuaState, 1);
				}
					break;
				case SD_STRING:
				{
					pOutParam[n] = (const char*)lua_tostring(m_pLuaState, -1);
					lua_pop(m_pLuaState, 1);
				}
					break;
				case SD_POINTER:
				{
					pOutParam[n] = (void*)lua_topointer(m_pLuaState, -1);
					lua_pop(m_pLuaState, 1);
				}
					break;
				default:
				{
					LOG_ERROR("Lua调用函数{0}失败，输出参数{1}类型错误", szFunName, n);
					lua_settop(m_pLuaState, top);
					return false;
				}
				}
			}

			lua_settop(m_pLuaState, top);
			return true;
		}

	}
	catch (...)
	{

	}

	const char* pszErrInfor = lua_tostring(m_pLuaState, -1);
	LOG_ERROR("Lua调用函数{0}失败，原因：{1}", szFunName, pszErrInfor);
	lua_settop(m_pLuaState, top);
	return false;
}



