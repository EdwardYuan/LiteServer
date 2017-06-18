#ifndef LUA_PARAM_H
#define LUA_PARAM_H

#include "../base/type_def.h"
#include <cstring>

// 脚本数据类型定义
enum
{
	SD_NUMBER = 0,				// 数字类型
	SD_DOUBLE,					// 浮点数据类型
	SD_STRING,					// 字符串类型
	SD_POINTER,					// 用户定义数据类型
	SD_MAXID,					// 最大类型
};

// Lua参数对象定义
class LuaParam
{
	enum { TEXT_SIZE = 256 };
	int64		m_nNumber;				// 数值
	double		m_dNumber;				// 浮点数
	size_t		m_nTextLen;				// 动态字符串长度
	char*		m_pszText;				// 动态字符串
	char		m_szText[TEXT_SIZE];		// 静态字符串
	void*		m_pUserData;				// 用户定义数据类型
	int			m_nValueType;				// 参数类型

public:
	LuaParam()
	{
		m_nValueType = SD_NUMBER;
		m_nNumber = 0;
		m_dNumber = 0.0f;
		m_nTextLen = 0;
		m_pszText = NULL;
		m_pUserData = NULL;
		memset(m_szText, 0, sizeof(m_szText));
	}

	~LuaParam()
	{
		if (m_pszText != NULL)
		{
			delete[] m_pszText;
			m_pszText = NULL;
		}
	}

	bool SetType(int nType)
	{
		if (nType < 0 || nType >= SD_MAXID)
		{
			return false;
		}
		m_nValueType = nType;
		return true;
	}

	int GetType()
	{
		return m_nValueType;
	}

	void operator = (int nValue)
	{
		m_nValueType = SD_NUMBER;
		m_nNumber = nValue;
	}
	void operator = (ulong nValue)
	{
		m_nValueType = SD_NUMBER;
		m_nNumber = nValue;
	}
	void operator = (int64 nValue)
	{
		m_nValueType = SD_NUMBER;
		m_nNumber = nValue;
	}
	void operator = (uint64 nValue)
	{
		m_nValueType = SD_NUMBER;
		m_nNumber = nValue;
	}
	void operator = (double nValue)
	{
		m_nValueType = SD_DOUBLE;
		m_dNumber = nValue;
	}
	void operator = (const char* str)
	{
		m_nValueType = SD_STRING;
		if (str == NULL)
		{
			return;
		}
		if (m_pszText != NULL)
		{
			delete[] m_pszText;
			m_pszText = NULL;
		}
		size_t len = strlen(str);
		if (len >= TEXT_SIZE)
		{
			m_pszText = new char[len+1];
			strncpy(m_pszText, str, len + 1);
		}
		else
		{
			strncpy(m_szText, str, sizeof(m_szText));
		}
		m_nTextLen = len;
	}

	void operator = (void * pValue)
	{
		m_nValueType = SD_POINTER;
		m_pUserData = pValue;
	}

	operator int(void)
	{
		return (int)m_nNumber;
	}

	operator ulong(void)
	{
		return (ulong)m_nNumber;
	}

	operator int64(void)
	{
		return m_nNumber;
	}

	operator double(void)
	{
		return m_dNumber;
	}

	operator const char*(void)
	{
		return (m_pszText == NULL) ? m_szText : m_pszText;
	}

	operator void *(void)
	{
		return m_pUserData;
	}
};

#endif



