#ifndef LUA_MGR_H
#define LUA_MGR_H

#include "lua_param.h"
#include "lua/lua.hpp"

class LuaMgr
{
public:
	LuaMgr();
	~LuaMgr();

	bool Init();
	bool Free();

	bool LoadFile(const char* szFileName);
	bool RunMemory(const char* pLuaData, int nDataLen);
	bool RunFunction(const char* szFunName, LuaParam* pInParam, int nInNum, LuaParam* pOutParam, int nOutNum);

private:
	lua_State*				m_pLuaState;
};

#endif



