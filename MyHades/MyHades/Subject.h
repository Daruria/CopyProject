#pragma once

class CObserver;
class CSubject
{
private:
	CSubject();
public:
	virtual ~CSubject();

public:
	void Notify(int iMsg, void* pData);
	void Subscribe(CObserver* pObserver);
	void Unsubscribe(CObserver* pObserver);

public:
	static CSubject* Create() { return new CSubject; }

private:
	list<CObserver*>	m_ObserverList;
};

