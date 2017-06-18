#ifndef ID_FACTORY_H
#define ID_FACTORY_H

#include <queue>

template<class IDType>
class IDFactory
{
public:
	IDFactory() :m_dwCurMaxID(0){}
	~IDFactory(){}

	IDType GetNewID()
	{
		IDType newID = 0;
		if (m_queIDFactory.empty())
		{
			++m_dwCurMaxID;
			newID = m_dwCurMaxID;
		}
		else
		{
			newID = m_queIDFactory.front();
			m_queIDFactory.pop();
		}

		return newID;
	}
	void FreeID(IDType id)
	{
		m_queIDFactory.push(id);
	}
private:
	IDType m_dwCurMaxID;
	std::queue<IDType> m_queIDFactory;
};


#endif

