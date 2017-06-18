#pragma once

#include "tool/macro_tool.h"
#include <mysql++/mysql++.h>

int test_sql()
{
	mysqlpp::Connection conn(false);
	if (conn.connect("testdb", "127.0.0.1",	"root", "root123")) 
	{
		mysqlpp::Query query = conn.query("select * from user");
		if (mysqlpp::StoreQueryResult res = query.store()) 
		{
			LOG_DEBUG("Got result Succ");
			//mysqlpp::StoreQueryResult::const_iterator it;
			//for (it = res.begin(); it != res.end(); ++it) 
			//{
			//	mysqlpp::Row row = *it;
			//	LOG_WARN("Got line {0}, {1}", row[0], (const char*)row[1]);
			//}
			for (size_t i = 0; i < res.num_rows(); ++i)
			{
				LOG_WARN("Got line {0}, {1}", res[i]["id"], (const char*)res[i]["name"]);
			}
		}
		else 
		{
			LOG_ERROR("Got result ERR {0}", query.error());
			return 1;
		}

		return 0;
	}
	else 
	{
		LOG_ERROR("connect faild {0}", conn.error());
		return 1;
	}

}